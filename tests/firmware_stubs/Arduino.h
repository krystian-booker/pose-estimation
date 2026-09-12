#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

// Model the pinned STM32duino USB API, including its surprising 10 ms
// boolean conversion. Only the firmware main-loop test uses this header.
inline uint64_t fake_time_us = 0;
struct FakeUsbSerial {
    bool connected = true;
    std::vector<std::vector<uint8_t>> frames;
    void begin(uint32_t) {}
    bool dtr() { return connected; }
    operator bool() { fake_time_us += 10'000; return connected; }
    int available() { return 0; }
    int read() { return -1; }
    int availableForWrite() { return 1024; }
    size_t write(const uint8_t* p, size_t n) {
        frames.emplace_back(p, p + n);
        return n;
    }
};
inline FakeUsbSerial Serial;
struct FakeRcc { uint32_t CSR = 0; };
inline FakeRcc fake_rcc;
inline FakeRcc* RCC = &fake_rcc;
inline uint32_t HAL_GetUIDw0() { return 1; }
inline uint32_t HAL_GetUIDw1() { return 2; }
inline uint32_t HAL_GetUIDw2() { return 3; }
