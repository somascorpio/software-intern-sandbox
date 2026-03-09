# Software Intern Sandbox Repo


This repo is for us software interns to learn how to actually develop software version control, test automation, pipelines, packaging, etc.


We will be using communication with embedded devices through  serialization to learn these processes.


The current goal of the project is to pack and unpack data to theoretically light up a LED on a connected embedded device. 


## Installation


1. C++ Compiler
2. CMake
3. WSL
4. Clone Repo




## User Guide


To be able to run power_led.cpp in your chosen IDE, there are steps for each compiler to include the header files. These are steps for popular compilers:


#### G++ 


Run in terminal:

```
g++ example/power_led.cpp -I./include -o program 
```


#### Clang


Run in terminal:
```
clang example/power_led.cpp -Iinclude -o app
```


#### Microsoft Visual C++


1. Right-click on the project, and select Properties. 
2. Select Configuration Properties -> C/C++ -> General. 
3. Set the path under Additional Include Directories: [path to \include]


#### Serialization Information (IMPORTANT)


Data is packed into a byte buffer, which is a vector with a size of 4 of uint8_t types (unsigned 8 bit integer types).
Byte representation (4 bytes total):


| data type | byte index(ces) | size (bytes) | notes |
|:---------:|:---------------:|:------------:|:-------------------------------------------|
| integer   | 0               | 1            | The inputted integer must be either 0 (stored as 0x00) or 1 (stored as 0x01), **representing  power: "on" or "off"**. All inputs >0 will be stored as 0x01 and otherwise will be stored as 0x00 |
| double    | 1-3             | 3            | The inputted double must be within **range of 1.60-3.50 to the hundredth's place, representing voltage**. This is stored as ASCII alphabet representations of each digit in the double (1 --> '1' --> 0x31) (ex. 1.6 in the buffer: [...,'1','6','0'], meaning we can only store 3 digit doubles inputted in the format: 0.00. Inputs >9.99 will lead to parsing errors. Trailing digits past the hundredth's place will not be considered in packing, no rounding will take place. | 




## Build


1. cmake -S . -B build
2. –cmake --build build
3. ctest –test-dir build


```cpp
//TBD
```

