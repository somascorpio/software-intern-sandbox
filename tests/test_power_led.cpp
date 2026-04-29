#include <gtest/gtest.h>
#include "power_led.hpp"

TEST(PowerLedTest, packIntWorksCorrectly){
    int num = 1000;
    Buf buf = packInt(num);
    EXPECT_EQ(buf[0], 'I');
    EXPECT_EQ(buf[1], 0x02);
    EXPECT_EQ(buf[2], 0xE8);
    EXPECT_EQ(buf[3], 0x03);
}

TEST(PowerLedTest, unpackIntWorksCorrectly){
    Buf buf = {'I', 0x02, 0xE8, 0x03};
    int num = unpackInt(buf);
    EXPECT_EQ(num, 1000);
}

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

TEST(PowerLedTest, unpackDoubleWorksCorrectly){
    Buf buf = {'D',0x05,0x31,0x30,0x2e,0x39,0x39};
    double num = unpackDouble(buf);

    EXPECT_TRUE(num-10.99<0.00001);
}

TEST(PowerLedTest, PackSetsPowerCorrectly)
{
    PowerLed led{2.5, 1};
    Buf buf = pack(led);

    EXPECT_EQ(buf[11], 1);  // power on
}

TEST(PowerLedTest, PackSetsVoltageCorrectly)
{
    PowerLed led{2.5, 1};
    Buf buf = pack(led);

    EXPECT_EQ(buf[14], '2'); //2
    EXPECT_EQ(buf[15],'.');
    EXPECT_EQ(buf[16],'5'); //.5
    EXPECT_EQ(buf[17],'0');//.00
}

TEST(PowerLedTest, UnpackWorksCorrectly){
    PowerLed led{2.5, 1};
    Buf buf = pack(led);
    PowerLed led1 = unpack(buf);

    EXPECT_EQ(led1.power,1);
    EXPECT_TRUE(led1.voltage-2.5<0.00001);
}