#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include "include/power_led.hpp"

int main(){
    PowerLed message = {1.6,1}; //"on", 1.6v-3.5v
    Buf buf = pack(message); //storing an integer in a vector of 4 bytes of data (serializing)
    PowerLed deserialized = unpack(buf); //deserializing the byte vector back into an integer stored in a PowerLed struct
    printBuff(buf); //prints the 4 bytes of data out
    std::cout << "\nDeserialized power:" << deserialized.power << std::endl; //prints the deserialized integer value
    std::cout << "\nDeserialized voltage: " << deserialized.voltage << std::endl;
    return 0;
}