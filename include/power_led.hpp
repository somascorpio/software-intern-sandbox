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
    buf.insert(buf.begin(),buf.size());//integers never exceed 255 bytes
    
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

Buf packIntArray(int nums[], int size){
    Buf buf;
    buf.push_back('A');
    buf.push_back(size);
    for(int i = 0; i < size ; i++){
        Buf addee = packInt(nums[i]);
        buf.insert(buf.end(), addee.begin(), addee.end());
    }
    return buf;
    
}

Buf packDoubleArray(double nums[], int size, int doubleSize){
    Buf buf;
    buf.push_back('A');
    buf.push_back(size);
    for(int i = 0; i < size; i++){
        Buf addee = packDouble(nums[i], doubleSize);
        buf.insert(buf.end(), addee.begin(), addee.end());
    }
    return buf;
}

int unpackInt(Buf buf){
    int size = buf[1];
    int inty = 0;
    for(int i = 2;i < 2+size; ++i){
        inty |= buf[i] << (8*(i-2));
    }

    return inty;
}
double unpackDouble(const Buf& buf){
    int size = buf[1];
    //int count = 10;
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
/*
void unpackIntArray(Buf buf, int* arr){
    int size = buf[1];

    int nxtIndx = 2;
    for(int i = 0; i < size; i++){
        std::vector<int> temp = unpackIntHelper(buf, nxtIndx);
        nxtIndx = temp[1]; arr[i] = temp[0];
    }
}

void unpackDoubleArray(Buf buf, int* arr){
    int size = buf[1];

    int nxtIndx = 2;
    int* arr = new int[size];
    for(int i = 0; i < size; i++){
        std::vector<double> temp = unpackDoubleHelper(buf, nxtIndx);
        nxtIndx = temp[1]; //arr
    }
}
*/
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

    buf.insert(buf.begin(),(uint8_t)buf.size());
    buf.insert(buf.begin(),{0x62,0x65,0x67,0x69,0x6E,0x4D,0x73,0x67});
    return buf;

}

std::vector<int> unpackIntHelper(Buf& buf, int index_look){
    Buf tempBuf(0);
    int tempSize;
    int endIndex;
    if(buf[index_look] == 'I'){
        index_look++;
        tempSize = buf[index_look];
        tempBuf.push_back('I'); tempBuf.push_back(tempSize);
        endIndex = index_look;
        for(int i = index_look + 1; i < index_look+1+tempSize; i++){
            tempBuf.push_back(buf[i]);
            endIndex++; 
        }
    }
    return {unpackInt(tempBuf), endIndex+1};
}
std::vector<double> unpackDoubleHelper(Buf& buf, int index_look){
    Buf tempBuf(0);
    int tempSize;
    int endIndex;
    if(buf[index_look] == 'D'){
        index_look++;
        tempSize = buf[index_look];
        tempBuf.push_back('D'); tempBuf.push_back(tempSize);
        endIndex = index_look;
        for(int i = index_look+1; i < index_look + 1 + tempSize; i++){
            tempBuf.push_back(buf[i]);
            endIndex++;
        }
    }
    return{unpackDouble(tempBuf), (double)endIndex+1};
}
struct PowerLed unpack(Buf& buf){
    //works for magic bytes that have a size of 8 -- this specific packing function works for a PowerLed struct.
    PowerLed powerled;
    Buf magic_byte(buf.begin(), buf.begin()+8);
    if(magic_byte != Buf{0x62,0x65,0x67,0x69,0x6E,0x4D,0x73,0x67}){
        std::cout << "This is not a valid PowerLed message." << std::endl;
        return powerled;
    }
    int size = buf[8]; //works for messages with a size < 256

    int next_index;

    std::vector<int> temp_int = unpackIntHelper(buf, 9);
    powerled.power = temp_int[0]; next_index = temp_int[1];

    std::vector<double> temp_double = unpackDoubleHelper(buf, next_index);
    powerled.voltage = temp_double[0]; next_index = temp_double[1];

    for(int j = 0; j < 3; j++){
        powerled.rgb[j] = unpackIntHelper(buf, next_index)[0];
        next_index = unpackIntHelper(buf, next_index)[1];
    }

    return powerled;

    }