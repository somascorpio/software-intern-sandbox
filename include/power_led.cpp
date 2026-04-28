#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include "power_led.hpp"

int main(){
    PowerLed message = {1.6,1,{255,125,0}}; //"on", 1.6v-3.5v
    Buf buf = pack(message); //storing an integer in a vector of 4 bytes of data (serializing)
    //PowerLed deserialized = unpack(buf); //deserializing the byte vector back into an integer stored in a PowerLed struct
    printBuff(buf); //prints the 4 bytes of data out
    //std::cout << "\nDeserialized power:" << deserialized.power << std::endl; //prints the deserialized integer value
    //std::cout << "\nDeserialized voltage: " << deserialized.voltage << std::endl << "\nDeserialized color: " << std::endl;
    /*
    for(int i = 0;i < 3; i++){
        std::cout << "\t" << std::dec << (int)deserialized.rgb[i] << std::endl;
    }

    Buf test = packDouble(10.99,6);
    double num = unpackDouble(test);
    std::cout << "\nTesting serialization with doubles/floats > 9:\n" << "\t" << 10.99 << " --> ";
    printBuff(test);
    std::cout << "--> " << num << std::endl;

    Buf test2 = packInt(1000);
    int num2 = unpackInt(test2);
    std::cout << "\nTesting serialization with ints > 255:\n" << "\t" << std::dec << 1000 << " --> ";
    printBuff(test2);
    std::cout << "--> " << std::dec << num2 << std::endl;
*/
Buf test = {0x49,0x01,0xff,0x2e};
unpackIntHelper(test,0);
    return 0;
}