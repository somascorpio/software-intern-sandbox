# Software Intern Sandbox Repo


This repo is for us software interns to learn how to actually develop software version control, test automation, pipelines, packaging, etc.


We will be using communication with embedded devices through  serialization to learn these processes.


The current goal of the project is to pack and unpack data to theoretically light up a LED on a connected embedded device. 


## Installation


Make sure you have:

1. An IDE ([VS Code](https://code.visualstudio.com/download) is recommended)
2. [Git](https://github.com/git-guides/install-git)
3. [A C++ Compiler](https://code.visualstudio.com/docs/languages/cpp)
4. [CMake](https://cmake.org/download/)
5. [WSL](https://learn.microsoft.com/en-us/windows/wsl/install) (if using Windows)

*Note: If using WSL, you will need to install a C++ compiler and CMake for your Linux distro which can be done by running:*

```
sudo apt update
sudo apt install cmake
sudo apt install build-essential
```




## User Guide

To be able to run examples/power_led.cpp from **CMake**, you must first build the entire project [see build directions](#build).

Enter a **WSL** terminal or other form of **Linux** to complete the rest, and enter these commands:

```
cd build
cmake --build .
./power_led
```

This is what should be outputted:

```
62 65 67 69 6e 4d 73 67 14 49 01 01 44 04 31 2e 36 30 41 03 49 01 ff 49 01 7d 49 01 00 
Deserialized power:1

Deserialized voltage: 1.6

Deserialized color: 
        255
        125
        0

Testing serialization with doubles/floats > 9:
        10.99 --> 44 06 31 30 2e 39 39 30 --> 10.99

Testing serialization with ints > 255:
        1000 --> 49 02 e8 03 --> 1000

Testing serialization with double arrays:
        { | 1.5 | 6.7 | 9 | } --> 41 03 44 04 31 2e 35 30 44 04 36 2e 37 30 44 04 39 2e 30 30 --> { | 1.5 | 6.7 | 9 | }
```


#### Serialization Information (IMPORTANT)

Data is packed into a byte buffer, which is a vector with a size of 4 of uint8_t types (unsigned 8 bit integer types).
Byte representation (27 bytes total):


| data type | byte index(ces) | size (bytes) | notes |
|:---------:|:--------------- |:-------------|:-------------------------------------------|
| magic byte | 0-7            | 8            | Denotes the beginning of our message, always stored in 8 bytes as "beginmsg", each character represented in a byte. |
| integer   | 8-10            | 3            | The inputted integer must be either 0 (stored as ``` {0x49, 0x01, 0x00) ``` or 1 ``` (stored as {0x49, 0x01, 0x01) ```, **representing  power: "on" or "off"**. All inputs >0 will be defaulted to being stored as 1 and inputs <0 will default to 0. All integers are denoted with an 'I' (0x49) and then the length. Bitshifting is used to store integers >255 |
| double    | 11-16           | 6            | The inputted double must be within **range of 1.60-3.50 to the hundredth's place, representing voltage**. Doubles >9.99 (if storing to the hundredths place, stored as ``` {0x44, 0x04, 0x39, 0x2E, 0x39, 0x39} ```) can still be stored. Each digit is stored as the ASCII alphabet value (ex. 1 will be represented with 0x31), the decimal point will also be stored as 0x2E. Doubles are denoted with a 'D' (0x44) and followed by their length, which includes the decimal point. When packing a double, one can choose the desired length that is stored, all digits past that length are dropped and the double is truncated. |
| integer array | 16-26       | 11           | The inputted array **represents RGB values for the LED, from 0-255**. The array is denoted as 'A' (0x41) and is followed by the length of the inputted array (in this case, 3) {. It is then followed by packed integers of a length of one byte. The program is able to pack and unpack integers of indeterminable length, but the indices of our packed PowerLed message will not pertain to those seen on this table. If the data is used for setting RGB values, an error may occur. | 


*See [examples of bad inputs](#examples-of-bad-inputs)*




## Build

To build this project in **CMake**, **WSL** or some form of **Linux** must be used to ensure it builds correctly. *Do not use QuickStart in VS Code*. Use these commands in a WSL terminal to build your project:

```
cmake -S . -B build
cmake --build build
```

To run tests on the project use this command in a WSL terminal:

```
ctest --test-dir build
```


*To create your own tests, add more into tests/test_power_led.cpp*




## Examples

To use the serialization logic in our project, your own projects must include the **include/power_led.hpp** file. To create a message other than PowerLed, you must create your own packing and unpacking logic that utilizes the pack and unpack functions for each data type.

```cpp
#include "power_led.hpp"
```

#### An example of using the packing function:

```cpp
PowerLed message = {1.6,1,{255,125,0}}; //1.6v-3.5v, "on", RGB value: {255, 125, 0}
Buf buf = pack(message); //Buf is a placeholder for std::vector<uint8_t>
printBuff(buf); //prints the data out.
```

##### Output:

```
62 65 67 69 6e 4d 73 67 14 49 01 01 44 04 31 2e 36 30 41 03 49 01 ff 49 01 7d 49 01 00
```


#### An example of using the unpacking function:

```cpp
PowerLed message = {1.6,1,{255,125,0}}; //"on", 1.6v-3.5v
Buf buf = pack(message); //storing an integer in a vector of 4 bytes of data (serializing)
PowerLed deserialized = unpack(buf); //deserializing the byte vector back into an integer stored in a PowerLed struct
printBuff(buf); //prints the 4 bytes of data out
std::cout << "\nDeserialized power:" << deserialized.power << std::endl; //prints the deserialized integer value
std::cout << "\nDeserialized voltage: " << deserialized.voltage << std::endl << "\nDeserialized color: " << std::endl;
    
for(int i = 0;i < 3; i++){
   std::cout << "\t" << std::dec << (int)deserialized.rgb[i] << std::endl;
}

```

##### Output:

```
62 65 67 69 6e 4d 73 67 14 49 01 01 44 04 31 2e 36 30 41 03 49 01 ff 49 01 7d 49 01 00 
Deserialized power:1

Deserialized voltage: 1.6

Deserialized color: 
        255
        125
        0
```


#### An example of packing and unpacking double arrays:

```cpp
double arrTest1[3] = {1.5,6.7,9.0};
Buf test3 = packDoubleArray(arrTest1,3,4);
std::cout << "\nTesting serialization with double arrays:\n" << "\t{ | ";
for(int i = 0; i < 3; i++){
   std::cout << arrTest1[i] << " | "; 
}
std::cout << "}" << " --> ";
printBuff(test3);
double unpackedArray1[3];
unpackDoubleArray(test3, unpackedArray1); //changes unpackedArray1, array has to be inputted by user 
std::cout << "--> { | ";
for(int j = 0; j < 3; j++){
   std::cout << unpackedArray1[j] << " | "; 
}
std::cout << "}" << std::endl;
```

##### Output:

```
Testing serialization with double arrays:
        { | 1.5 | 6.7 | 9 | } --> 41 03 44 04 31 2e 35 30 44 04 36 2e 37 30 44 04 39 2e 30 30 --> { | 1.5 | 6.7 | 9 | }
```


#### An example of packing maximum integer and double values:

```cpp
#include <climits>
//{...}
int num = INT_MAX;
Buf buf = packInt(num);
int num2 = unpackInt(buf);
std::cout << num2==num << std::endl;

double num3 = DBL_MAX;
Buf buf = packDouble(num3, sizeof(num3));
int num4 = unpackDouble(buf);
std::cout << num4-num3 < 0.0000000001 << std::endl;
```

##### Output:

```
true
true
```


#### Examples of bad inputs:

Valid inputted numbers for the message can be seen in [Serialization Information under User Guide](#serialization-information-important).

Bad inputs for voltage can result in inaccurate data depending on the compiler and your version of C++. For example, with G++ version 13.3.0, the following code snippets will output bad data or default to good data:

##### Example 1:

```cpp
PowerLed message = {11.65, 30, {255, 255, 256}}; //doubles are stored with 3 digits in our main packing logic, 11.65 will be stored as 11.6
//30 is not 0 or 1 and will output as 1 or "on"
//256 will be able to be stored, may cause errors when using RGB values for yourself
Buf buffer = pack(message);
PowerLed deserialized = unpack(buffer);

PrintBuff(buffer);
std::cout << "\nDeserialized Voltage: " << deserialized.voltage << std::endl;
std::cout << "\nDeserialized voltage: " << deserialized.voltage << std::endl << "\nDeserialized color: " << std::endl;
for(int i = 0;i < 3; i++){
   std::cout << "\t" << std::dec << (int)deserialized.rgb[i] << std::endl;
}
```

##### Output:

```
62 65 67 69 6e 4d 73 67 14 49 01 01 44 04 31 31 2E 36 41 03 49 01 FF 49 01 FF 49 02 00 01  

Deserialized Voltage: 11.6

Deserialized Power: 1

Deserialized color:
   255
   255
   256
```

##### Example 2:

```cpp
int arr[256]; //arrays with a length >255 will default to a size of zero, but numbers will still be stored and cause an error with memory allocation
//this is consistent with double and integer arrays
Buf buffer = packIntArray(arr, 256);
printBuff(buffer);

```

##### Output:

```
Segmentation fault (core dumped)
```

##### Example 3:

```cpp
PowerLed message = {-10.0, -100, {-23, -44, -6}; //all values <0 will default to 0
Buf buffer = pack(message);
PowerLed deserialized = unpack(buffer);

PrintBuff(buffer);
std::cout << "\nDeserialized Voltage: " << deserialized.voltage << std::endl;
std::cout << "\nDeserialized voltage: " << deserialized.voltage << std::endl << "\nDeserialized color: " << std::endl;
for(int i = 0;i < 3; i++){
   std::cout << "\t" << std::dec << (int)deserialized.rgb[i] << std::endl;
}
```

##### Output:

```
62 65 67 69 6e 4d 73 67 14 49 01 00 44 04 2E 00 00 00 41 03 49 01 00 49 01 00 49 01 00

Deserialized Voltage: 0.0

Deserialized Power: 0

Deserialized color:
   0
   0
   0
```

*Note: Bad inputs can be disastrous depending on what you're using your project for, please look at the previously linked [Serialization Information](#serialization-information-important) to correctly use our project.*


