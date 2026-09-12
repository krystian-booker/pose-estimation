#pragma once

#include <stdint.h>

namespace gw_fw::bmi088_registers {

// Bosch BMI08_GYRO_BW_MASK: upper bits are not the bandwidth field.
// The first MicoAir board reads 0x83 after selecting 400 Hz / 47 Hz.
constexpr uint8_t kGyroBandwidthMask = 0x0F;
constexpr uint8_t kGyro400Hz47Hz = 0x03;

constexpr uint8_t with_gyro_bandwidth(uint8_t reg) {
    return (reg & ~kGyroBandwidthMask) | kGyro400Hz47Hz;
}

constexpr bool gyro_bandwidth_matches(uint8_t reg) {
    return (reg & kGyroBandwidthMask) == kGyro400Hz47Hz;
}

}  // namespace gw_fw::bmi088_registers
