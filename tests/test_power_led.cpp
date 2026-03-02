#include <gtest/gtest.h>
#include "power_led.hpp"

TEST(PowerLedTest, PackSetsPowerCorrectly)
{
    PowerLed led{2.5, 1};
    Buf buf = pack(led);

    EXPECT_EQ(buf[0], 1);  // power on
}