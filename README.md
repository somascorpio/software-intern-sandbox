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
01 31 36 30
Deserialized power: 1

Deserialized voltage: 1.6
```


#### Serialization Information (IMPORTANT)

Data is packed into a byte buffer, which is a vector with a size of 4 of uint8_t types (unsigned 8 bit integer types).
Byte representation (4 bytes total):


| data type | byte index(ces) | size (bytes) | notes |
|:---------:|:---------------:|:------------:|:-------------------------------------------|
| integer   | 0               | 1            | The inputted integer must be either 0 (stored as 0x00) or 1 (stored as 0x01), **representing  power: "on" or "off"**. All inputs >0 will be stored as 0x01 and otherwise will be stored as 0x00 |
| double    | 1-3             | 3            | The inputted double must be within **range of 1.60-3.50 to the hundredth's place, representing voltage**. This is stored as ASCII alphabet representations of each digit in the double (1 --> '1' --> 0x31) (ex. 1.6 in the buffer: [...,'1','6','0'], meaning we can only store 3 digit doubles inputted in the format: 0.00. Inputs >9.99 will lead to parsing errors. Trailing digits past the hundredth's place will not be considered in packing, no rounding will take place. | 

*See [examples of bad inputs](examples-of-bad-inputs)*




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

To use the serialization logic in our project, your own projects must include the **include/power_led.hpp** file.

```cpp
#include "power_led.hpp"
```

#### An example of using the packing function:

```cpp
PowerLed message = {1.35, 1}; //1.35v, on
Buf buffer = pack(message); //Buf is a placeholder for std::vector<uint8_t>

std::cout << buffer << std::endl; //displays byte data in hexadecimal form
```

##### Output:

```
01 31 33 35
```


#### An example of using the unpacking function:

```cpp
   PowerLed message = {1.35, 1}; //1.35v, on
   Buf buffer = pack(message);

   PowerLed deserialized = unpack(buffer); //new struct

   std::cout << buffer << std::endl; //displays byte data in hexadecimal form
   std::cout << "\nDeserialized Voltage: " << deserialized.voltage << std::endl; //displays voltage of unpacked message
   std::cout << "\nDeserialized Power: " << deserialized.power << std::endl; //displays power of unpacked message
```

##### Output:

```
01 31 33 35

Deserialized Voltage: 1.35

Deserialized Power: 1
```


#### Examples of bad inputs:

Valid inputted numbers for the message can be seen in [Serialization Information under User Guide](#serialization-information-important).

Bad inputs for voltage can result in inaccurate data depending on the compiler and your version of C++. For example, with G++ version 13.3.0, the following code snippets will output bad data:

###### Example 1:

```cpp
PowerLed message = {11.6, 30}; //11.6v (not within 0.00-9.99 range), 30 is not 0 or 1 and will output as 1 or "on"
Buf buffer = pack(message);
PowerLed deserialized = unpack(buffer);

std::cout << buffer << std::endl;
std::cout << "\nDeserialized Voltage: " << deserialized.voltage << std::endl;
std::cout << "\nDeserialized Power: " << deserialized.power << std::endl;
```

##### Output:

```
01 3b 35 39

Deserialized Voltage: 11.59

Deserialized Power: 1
```

##### Example 2:

```cpp
PowerLed message = {1.666, -1}; //1.66v (trailing digits past the hundredth's place), -1 is not 0 or 1 and will output as 0 or "off"
Buf buffer = pack(message);
PowerLed deserialized = unpack(buffer);

std::cout << buffer << std::endl;
std::cout << "\nDeserialized Voltage: " << deserialized.voltage << std::endl;
std::cout << "\nDeserialized Power: " << deserialized.power << std::endl;
```

##### Output:

```
00 31 36 36

Deserialized Voltage: 1.66

Deserialized Power: 0
```

*Note: Bad inputs can be disastrous depending on what you're using your project for, please look at the previously linked [Serialization Information](#serialization-information-important) to correctly use our project.*
