#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

struct PowerLed{
    int power;
};


void printBuff(vector<uint8_t>& buf){
    for(auto b:buf){
        int temp = b;
        cout<<temp<<",";
//make sure it's cast as an integer before printing 
    }
}

vector<uint8_t> pack(struct PowerLed& msg){
    vector<uint8_t> buf(sizeof(msg));
    memcpy(buf.data(),&msg,sizeof(msg));
    return buf;
}

struct PowerLed unpack(vector<uint8_t>& buf){
    PowerLed powerled;
    memcpy(&powerled,buf.data(),sizeof(powerled));
    return powerled;
}
int main(){
    PowerLed message = {1};
    vector<uint8_t> buf = pack(message);
    PowerLed deserialized = unpack(buf);
    printBuff(buf);
    cout << "\nDeserialized power:" << deserialized.power << endl;
    return 0;
}