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
}

Buf pack(struct PowerLed& msg){
    Buf buf(7);

    uint8_t byte_one;
    byte_one = msg.power > 0 ? 1 : 0;//on or off
    Buf thingy = packInt(byte_one);
    buf[0] = thingy[0];

    double vltge = msg.voltage;
    int tempy = (int)vltge;
    buf[1] = '0' + (int)vltge; //we don't store the decimal point because we know exactly where it is in the range-- 1.6v-3.5v

    vltge -= tempy; //removes numbers after decimal point
    for(int i = 2;i < 4; i++){
        buf[i] = '0' + (int)(vltge*10);
        vltge = (vltge*10) - (int)(vltge*10);
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
    
    double vltge = 0.0; //voltage deserialization
    int divisor = 1;
    for(int i = 1;i < 4; i++){
        vltge+=(double)(buf[i]-48)/divisor;
        divisor *= 10;
    }
    powerled.voltage = vltge;

    for(int j = 4;j < 7; j++){
        powerled.rgb[j-4] = (int)buf[j];
    }

    return powerled;
}