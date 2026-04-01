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
    /*
    double loops = (double)num/(double)255;
    if((int)loops < loops || loops == 0){
        loops=(int)loops+1;
    }
    else{
        loops = (int)loops;
    }

    Buf buf((int)loops);
    int temp = num;
    for(int i = 0;i <(int)loops; ++i){
        if(temp<255){
            buf[i] = (uint8_t)temp;
        }
        else{
            buf[i] = (uint8_t)255;
            temp-=255;
        }
    }

    return buf;
    */

    Buf buf;
    for(;num > 0; num>>8){
        buf.push_back(num & 0xFF);
    }

    return buf;
}

Buf packDouble(double& num){
    Buf buf;
    int befPoint = (int)num;
    while(befPoint>0){
        buf.push_back('0'+befPoint%10);
        befPoint/=10;
    }
    buf.push_back('.');
    double aftPoint = num-(int)num;
    while(aftPoint>0.000001){
        buf.push_back('0'+(int)(aftPoint*10));
        aftPoint = (aftPoint*10) - (int)(aftPoint*10);
    }
    return buf;
}

double unpackDouble(Buf buf){
    int dub1 = 0;
    int n = 0;
    char digit = buf[n];
    while(digit!='.'){
        dub1 = dub1*10 + (int)digit;
        n++;
        digit = buf[n];
    }
    int divisor = 10;
    double dub2 = dub1;
    for(int i = n+1;i < buf.size(); i++){
        dub2 += (double)(buf[i]-49)/divisor;
        divisor*=10;
    }

    return dub2;
}

Buf pack(struct PowerLed& msg){
    Buf buf(8);

    uint8_t byte_one;
    byte_one = msg.power > 0 ? 1 : 0;//on or off
    Buf inty = packInt(byte_one);
    buf[0] = inty[0];

    Buf dubby = packDouble(msg.voltage);
    for(int i = 1;i < 4; i++){
        buf[i] = dubby[i-1];
    }

    for(int j = 4;j < 7; j++){
        buf[j] = packInt(msg.rgb[j-4])[0];
    }

    return buf;

}

struct PowerLed unpack(Buf& buf){
    PowerLed powerled;

    int pwr; //power deserialization
    pwr = buf[0] > 0 ? 1 : 0; 
    powerled.power = pwr;

    Buf vltgeBuf(buf.begin()+1,buf.begin()+4);
    powerled.voltage = unpackDouble(vltgeBuf);

    for(int j = 4;j < 7; j++){
        powerled.rgb[j-4] = (int)buf[j];
    }

    return powerled;
}