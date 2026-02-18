#include <iostream>
#include <vector>
#include <bit>
#include <cstdint>
#include <cmath>
using Buf = std::vector<uint8_t>;

struct PowerLed{
    double voltage;
    int power; //what we're serializing and deserializing
};


void printBuff(Buf& buf){
    for(const auto& b:buf){
        int temp = b;
        std::cout<<temp<<",";
    }
}

Buf pack(struct PowerLed& msg){
    Buf buf(4);

    uint8_t byte_one;
    byte_one = msg.power > 0 ? 1 : 0;//on or off
    buf[0] = byte_one;

    double vltge = msg.voltage;
    int tempy = (int)vltge;
    buf[1] = '0' + (int)vltge; //we don't store the decimal point because we know exactly where it is in the range-- 1.6v-3.5v

    vltge -= tempy; //removes numbers after decimal point
    for(int i = 2;i<buf.size();i++){
        buf[i] = '0' + (int)(vltge*10);
        vltge = (vltge*10) - (int)(vltge*10);
}
    return buf;

}

struct PowerLed unpack(Buf& buf){
    PowerLed powerled;

    int pwr;
    pwr = buf[0] > 0 ? 1 : 0; 
    powerled.power = pwr;
    
    double vltge = 0.0;
    for(int i = 1;i<buf.size();i++){
        vltge+=(double)(buf[i]-48)*(pow(10,(1-i)));
    }
    powerled.voltage = vltge;

    return powerled;
}
int main(){
    PowerLed message = {1.6,1}; //"on", 1.6v-3.5v
    Buf buf = pack(message); //storing an integer in a vector of 4 bytes of data (serializing)
    PowerLed deserialized = unpack(buf); //deserializing the byte vector back into an integer stored in a PowerLed struct
    printBuff(buf); //prints the 4 bytes of data out
    std::cout << "\nDeserialized power:" << deserialized.power << std::endl; //prints the deserialized integer value
    std::cout << "\nDeserialized voltage: " << deserialized.voltage << std::endl;
    return 0;
}