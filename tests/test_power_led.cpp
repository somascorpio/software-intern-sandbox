#include <gtest/gtest.h>
#include "power_led.hpp"
#include <climits>

//integer tests
TEST(PowerLedTest, packIntWorksCorrectly){
    int num = 1000;
    Buf buf = packInt(num);

    EXPECT_EQ(buf[0], 'I');
    EXPECT_EQ(buf[1], 0x02);
    EXPECT_EQ(buf[2], 0xE8);
    EXPECT_EQ(buf[3], 0x03);
}

TEST(PowerLedTest, negativeIntDoesNotWork){
    int num = -1;
    Buf buf = packInt(num);
    int num2 = unpackInt(buf);

    EXPECT_EQ(num2, 0);
}

TEST(PowerLedTest, largeIntegerTest){
    int num = INT_MAX;
    Buf buf = packInt(num);
    int num2 = unpackInt(buf);

    EXPECT_EQ(num2, num);

}

TEST(PowerLedTest, zeroIntegerTest){
    Buf buf = packInt(0);
    int num = unpackInt(buf);

    EXPECT_EQ(num, 0); //num should be equal to zero with integers < 0

}

TEST(PowerLedTest, unpackIntWorksCorrectly){
    Buf buf = {'I', 0x02, 0xE8, 0x03};
    int num = unpackInt(buf);

    EXPECT_EQ(num, 1000);
}

//double tests
TEST(PowerLedTest, packDoubleWorksCorrectly){
    double num = 10.99;
    Buf buf = packDouble(num,5);

    EXPECT_EQ(buf[0],'D');
    EXPECT_EQ(buf[1],5);
    EXPECT_EQ(buf[2],'1');
    EXPECT_EQ(buf[3],'0');
    EXPECT_EQ(buf[4], '.');
    EXPECT_EQ(buf[5],'9');
    EXPECT_EQ(buf[6],'9');
}

TEST(PowerLedTest, negativeDoubleDoesNotWork){
    double num = -1.0;
    Buf buf = packDouble(num, sizeof(num));
    double num2 = unpackDouble(buf);

    EXPECT_TRUE(num2 < 0.000000000001); //num should be equivalent or close to zero as default
}

TEST(PowerLedTest, largeDoubleTest){
    double num = DBL_MAX;
    Buf buf = packDouble(num, sizeof(DBL_MAX));
    double num2 = unpackDouble(buf);

    EXPECT_TRUE(num2-num < 0.000000000001);
}

TEST(PowerLedTest, zeroDoubleTest){
    Buf buf = packDouble(0, 1);
    double num = unpackDouble(buf);

    EXPECT_TRUE(num < 0.000000000001);
}

TEST(PowerLedTest, unpackDoubleWorksCorrectly){
    Buf buf = {'D',0x05,0x31,0x30,0x2e,0x39,0x39};
    double num = unpackDouble(buf);

    EXPECT_TRUE(num-10.99<0.00001);
}

//integer array tests
TEST(PowerLedTests, intArrayWorks){
    int arr[3] = {1, 1000, 10};
    Buf buf = packIntArray(arr, 3);
    int unpacked[3];
    unpackIntArray(buf, unpacked);

    EXPECT_EQ(unpacked[0], 1);
    EXPECT_EQ(unpacked[1], 1000);
    EXPECT_EQ(unpacked[2], 10);
}

TEST(PowerLedTests, largeIntegerArray1){
    int arr[255];
    arr[254] = 100;
    Buf buf = packIntArray(arr, 255);
    int unpacked[255];
    unpackIntArray(buf, unpacked);

    EXPECT_EQ(unpacked[254], 100);
}

TEST(PowerLedTests, largeIntegerArray2){
    int arr[256];
    arr[256] = 100;
    Buf buf = packIntArray(arr, 256);
    int unpacked[256];
    unpackIntArray(buf, unpacked);

    EXPECT_NE(unpacked[256], arr[256]); //there is a small chance this could fail.
    EXPECT_EQ(buf[0], 'A');
    EXPECT_EQ(buf[1], '\0'); //default char value happens because 256 cannot be converted to a 1 byte unsigned integer
}

//double array tests
TEST(PowerLedTests, doubleArrayWorks){
    double arr[3] = {1.4, 98.7, 2.34};
    Buf buf = packDoubleArray(arr, 3, 5);
    double unpacked[3];
    unpackDoubleArray(buf, unpacked);

    EXPECT_TRUE(unpacked[0] - arr[0] < 0.000000000001);
    EXPECT_TRUE(unpacked[1] - arr[1] < 0.000000000001);
    EXPECT_TRUE(unpacked[2] - arr[2] < 0.000000000001);
}

TEST(PowerLedTests, largeDoubleArray1){
    double arr[255];
    arr[254] = 9.99;
    Buf buf = packDoubleArray(arr, 255, 4);
    double unpacked[255];
    unpackDoubleArray(buf, unpacked);

    EXPECT_TRUE(unpacked[254]-9.99 < 0.000000000001);
}

TEST(PowerLedTests, largeDoubleArray2){
    double arr[256];
    arr[256] = 9.99;
    Buf buf = packDoubleArray(arr, 256, 4);
    double unpacked[256];
    unpackDoubleArray(buf, unpacked);

    EXPECT_TRUE(unpacked[256]-arr[256]); //there is a small chance this could fail.
    EXPECT_EQ(buf[0], 'A');
    EXPECT_EQ(buf[1], '\0'); //default char value happens because 256 cannot be converted to a 1 byte unsigned integer
}

//pack(Buf& buf) tests
TEST(PowerLedTest, packSetsPowerCorrectly)
{
    PowerLed led{2.5, 1};
    Buf buf = pack(led);

    EXPECT_EQ(buf[11], 1);  // power on
}

TEST(PowerLedTest, packSetsVoltageCorrectly)
{
    PowerLed led{2.5, 1};
    Buf buf = pack(led);

    EXPECT_EQ(buf[14], '2'); //2
    EXPECT_EQ(buf[15],'.');
    EXPECT_EQ(buf[16],'5'); //.5
    EXPECT_EQ(buf[17],'0');//.00
}

//unpack(Buf &buf ) test
TEST(PowerLedTest, unpackWorksCorrectly){
    PowerLed led{2.5, 1};
    Buf buf = pack(led);
    PowerLed led1 = unpack(buf);

    EXPECT_EQ(led1.power,1);
    EXPECT_TRUE(led1.voltage-2.5<0.00001);
}