#include <gtest/gtest.h>

#include "firmware/src/bmi088_registers.hpp"

using namespace gw_fw::bmi088_registers;

TEST(Bmi088RegistersTest, AcceptsFirstBoardBandwidthReadback) {
    EXPECT_TRUE(gyro_bandwidth_matches(0x83));
    EXPECT_TRUE(gyro_bandwidth_matches(0x03));
    EXPECT_FALSE(gyro_bandwidth_matches(0x80));  // reset: 2000 Hz
    EXPECT_FALSE(gyro_bandwidth_matches(0x82));  // 1000 Hz
    EXPECT_FALSE(gyro_bandwidth_matches(0x84));  // 200 Hz
    EXPECT_FALSE(gyro_bandwidth_matches(0xFF));  // disconnected bus
}

TEST(Bmi088RegistersTest, PreservesUpperBitsWhenSelectingBandwidth) {
    EXPECT_EQ(with_gyro_bandwidth(0x80), 0x83);
    EXPECT_EQ(with_gyro_bandwidth(0x00), 0x03);
    EXPECT_EQ(with_gyro_bandwidth(0x84), 0x83);
}
