#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

struct PowerLed{
    int power; //what we're serializing and deserializing
};


void printBuff(vector<uint8_t>& buf){
    for(auto b:buf){
        int temp = b;
        cout<<temp<<",";
    }
}

vector<uint8_t> pack(struct PowerLed& msg){
    vector<uint8_t> buf(sizeof(msg));
    memcpy(buf.data(),&msg,sizeof(msg)); //converting struct to byte vector
    return buf;
}

struct PowerLed unpack(vector<uint8_t>& buf){
    PowerLed powerled;
    memcpy(&powerled,buf.data(),sizeof(powerled));
    return powerled; // converting byte vector into struct
}
int main(){
    PowerLed message = {1}; //"on"
    vector<uint8_t> buf = pack(message); //storing an integer in a vector of 4 bytes of data (serializing)
    PowerLed deserialized = unpack(buf); //deserializing the byte vector back into an integer stored in a PowerLed struct
    printBuff(buf); //prints the 4 bytes of data out
    cout << "\nDeserialized power:" << deserialized.power << endl; //prints the deserialized integer value
    return 0;
}