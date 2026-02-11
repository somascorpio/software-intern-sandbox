#include <iostream>
#include <vector>
#include <cstdint>
using Buf = std::vector<uint8_t>;

struct PowerLed{
    int power; //what we're serializing and deserializing
};


void printBuff(Buf& buf){
    for(const auto& b:buf){
        int temp = b;
        std::cout<<temp<<",";
    }
}

Buf pack(struct PowerLed& msg){
    Buf buf(0);
    uint8_t byte_one;
    byte_one = msg.power > 0 ? 1 : 0;//on or off
    buf.push_back(byte_one);
    return buf;

}

struct PowerLed unpack(Buf& buf){
    PowerLed powerled;
    int pwr;
    pwr = buf[0] > 0 ? 1 : 0; 
    powerled.power = pwr;
    return powerled;
}
int main(){
    PowerLed message = {1}; //"on"
    Buf buf = pack(message); //storing an integer in a vector of 4 bytes of data (serializing)
    PowerLed deserialized = unpack(buf); //deserializing the byte vector back into an integer stored in a PowerLed struct
    printBuff(buf); //prints the 4 bytes of data out
    std::cout << "\nDeserialized power:" << deserialized.power << std::endl; //prints the deserialized integer value
    return 0;
}