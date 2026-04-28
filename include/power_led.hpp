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
    Buf buf{};
    int num1 = num;
    for (int i = 0; i < 8 && num1 > 0;i++){
        buf.push_back((uint8_t)(num1 & 0xFF));
        num1 = num1 >> 8;
    }
    if(num == 0){buf.push_back(0);}
    std::cout<<buf.size()<<std::endl;
    buf.insert(buf.begin(),buf.size());
    
    buf.insert(buf.begin(),'I');

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

    buf.insert(buf.begin(),size);
    buf.insert(buf.begin(),'D');
    return buf;
}

int unpackInt(Buf buf){
    int size = buf[1];
    int inty = 0;
    for(int i = 2;i < 2+size; ++i){
        inty |= buf[i] << (8*i);
    }

    return inty;
}
double unpackDouble(Buf buf){
    int size = buf[2];
    int count = 10;
    int dub1 = 0;
    int n = 2;
    char digit = buf[n];
    while(digit!='.'){
        dub1 = dub1*10 + ((int)digit-48);
        n++;
        digit = buf[n];
    }
    int divisor = 10;
    double dub2 = dub1;
    for(int i = n+1;i < 2+size; i++){
        dub2 += (double)(buf[i]-48)/divisor;
        divisor*=10;
    }

    return dub2;
}

Buf pack(struct PowerLed& msg){
    Buf buf(0);

    uint8_t byte_one;
    byte_one = msg.power > 0 ? 1 : 0;//on or off
    Buf byte_one_vector = packInt(byte_one);
    buf.insert(buf.end(), byte_one_vector.begin(), byte_one_vector.end());

    double voltage_bytes = msg.voltage;
    if((int)voltage_bytes > 9){voltage_bytes = 9.99;}
    if((int)voltage_bytes < 0){voltage_bytes = 0.00;}

    Buf voltage_bytes_vector = packDouble(voltage_bytes, 4);

    buf.insert(buf.end(),voltage_bytes_vector.begin(),voltage_bytes_vector.end());

    for(int i = 0; i < 3; i++){
        Buf tempRGB = packInt(msg.rgb[i] < 256 ? msg.rgb[i] : 255);
        buf.insert(buf.end(),tempRGB.begin(),tempRGB.end());
    }

    buf.insert(buf.begin(),buf.size());
    buf.insert(buf.begin(),{0x62,0x65,0x67,0x69,0x6E,0x4D,0x73,0x67});
    return buf;

}

std::vector<int> unpackIntHelper(Buf& buf, int index_look){
    Buf tempBuf(0);
    int tempSize;
    int endIndex;
    std::cout<<buf[index_look]<<std::endl;
    if(buf[index_look] == 'I'){
        index_look++;
        tempSize = buf[index_look];
        int endIndex = index_look;
        for(int i = index_look; i < index_look+tempSize; i++){
            tempBuf.push_back(buf[i]);
            endIndex++; 
        }
    }
    printBuff(tempBuf);
    std::cout<<std::endl<<tempSize<<std::endl;
    return {0,0};
}

std::vector<double> unpackDoubleHelper(Buf& buf, int index_look){
    Buf tempBuf(0);
    int tempSize;
    int endIndex;
    if(buf[index_look] == 'D'){
        index_look++;
        tempSize = buf[index_look];
        for(int i = index_look; i < index_look + tempSize; i++){
            tempBuf.push_back(buf[i]);
            endIndex++;
        }
    }
    return{unpackDouble(tempBuf), (double)endIndex};
}
struct PowerLed unpack(Buf& buf){
    //works for magic bytes that have a size of 8
    PowerLed powerled;
    Buf magic_byte(buf.begin(), buf.begin()+8);
    int size = buf[8]; //works for messages with a size < 256

    int next_index;

    std::vector<int> temp_int = unpackIntHelper(buf, 9);
    powerled.power = temp_int[0]; next_index = temp_int[1];

    std::vector<double> temp_double = unpackDoubleHelper(buf, next_index);
    powerled.voltage = temp_double[0]; next_index = temp_double[1];

    for(int j = 0; j < 3; j++){
        powerled.rgb[j+next_index] = unpackIntHelper(buf, next_index)[0];
    }

    return powerled;
    /*
    PowerLed powerled;

    Buf buffy = {buf[0]};
    int pwr = unpackInt(buffy) > 0 ? 1 : 0; //power deserialization
    powerled.power = pwr;

    Buf::const_iterator first = buf.begin()+1;
    Buf::const_iterator last = buf.begin()+5;
    Buf vltgeBuf(first,last);

    powerled.voltage = unpackDouble(vltgeBuf);

    for(int j = 5;j < 8; j++){
        Buf intBuf = {buf[j]};
        powerled.rgb[j-5] = unpackInt(intBuf);
    }

    return powerled;
    */
}