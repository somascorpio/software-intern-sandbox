#include <gtest/gtest.h>
#include "power_led.hpp"

TEST(PowerLedTest, PackSetsPowerCorrectly)
{
    PowerLed led{2.5, 1};
    Buf buf = pack(led);

    EXPECT_EQ(buf[0], 1);  // power on
}

TEST(PowerLedTest, PackSetsVoltageCorrectly)
{
    PowerLed led{2.5, 1};
    Buf buf = pack(led);

    EXPECT_EQ(buf[1], '2'); //2
    EXPECT_EQ(buf[2],'5'); //.5
    EXPECT_EQ(buf[3],'0');//.00
}

TEST(PowerLedTest, UnpackWorksCorrectly){
    PowerLed led{2.5, 1};
    Buf buf = pack(led);
    PowerLed led1 = unpack(buf);

    EXPECT_EQ(led1.power,1);
    EXPECT_EQ(led1.voltage,2.5);
}