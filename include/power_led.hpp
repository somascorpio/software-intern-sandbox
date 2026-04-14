#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>

using Buf = std::vector<uint8_t>;

struct PowerLed{
    double voltage;
    int power; //what we're serializing and deserializing
    int rgb[3];
};

void printBuff(Buf& buf){
    for(const auto& b:buf){
        int temp = b;
        std::cout << std::hex << std::setfill('0') << std::setw(2) << temp << " ";
    }
}

Buf packInt(int num){
    Buf buf(8);
    
    for (int i = 0; i < 8 && num > 0; i++ && num >> 8) {
        buf[i] = (num & 0xFF);
    }

    return buf;
}

Buf packDouble(double num,int size){
    int count = size;
    Buf buf;
    int befPoint = (int)num;
    while(befPoint>0){
        buf.insert(buf.begin(),'0'+befPoint%10);
        befPoint/=10;
        count--;
    }
    buf.push_back('.');
    count--;
    double aftPoint = num-(int)num;
    for(int i = 0;i < count; i++){
        buf.push_back('0'+(int)(aftPoint*10));
        aftPoint = (aftPoint*10) - (int)(aftPoint*10);
    }
    return buf;
}

int unpackInt(Buf buf){
    int inty = 0;
    int multiplier = 1;
    for(int i = 0;i < buf.size(); ++i){
        inty += buf[i]*multiplier;
        multiplier*=256;
    }
    return inty;
}
double unpackDouble(Buf buf){
    Buf buffy(buf.size());
    buffy = buf;
    int count = 10;
    int dub1 = 0;
    int n = 0;
    char digit = buffy[n];
    while(digit!='.'){
        dub1 = dub1*10 + ((int)digit-48);
        n++;
        digit = buffy[n];
    }
    int divisor = 10;
    double dub2 = dub1;
    for(int i = n+1;i < buffy.size(); i++){
        dub2 += (double)(buffy[i]-48)/divisor;
        divisor*=10;
    }

    return dub2;
}

Buf pack(struct PowerLed& msg){
    Buf buf(8);

    uint8_t byte_one;
    byte_one = msg.power > 0 ? 1 : 0;//on or off
    Buf inty = packInt(byte_one);
    buf[0] = unpackInt(inty);

    Buf dubby = packDouble(msg.voltage, 4);
    for(int i = 1;i < 5; i++){
        buf[i] = dubby[i-1];
    }

    for(int j = 5;j < 8; j++){
        Buf tempRGB = packInt(msg.rgb[j-5]);
        buf[j] = unpackInt(tempRGB);
    }

    return buf;

}

struct PowerLed unpack(Buf& buf){
    PowerLed powerled;

    int pwr; //power deserialization
    pwr = buf[0] > 0 ? 1 : 0; 
    powerled.power = pwr;

    Buf::const_iterator first = buf.begin()+1;
    Buf::const_iterator last = buf.begin()+5;
    Buf vltgeBuf(first,last);

    powerled.voltage = unpackDouble(vltgeBuf);

    for(int j = 5;j < 8; j++){
        powerled.rgb[j-5] = (int)buf[j];
    }

    return powerled;
}