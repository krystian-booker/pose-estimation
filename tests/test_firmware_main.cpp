#include <gtest/gtest.h>

// Compile the real firmware loop against a host USB/timer model. The sensor
// and output implementations below avoid any physical I/O.
#include "firmware/src/main.cpp"

namespace {
uint64_t next_sample_us = 0;
unsigned stop_calls = 0;
}

namespace gw_fw {
bool Bmi088::begin() { ok_ = true; next_sample_us = 2500; return true; }
bool Bmi088::poll(gw_sync::ImuRecord& sample) {
    if (fake_time_us < next_sample_us) return false;
    sample = {};
    sample.t_us = fake_time_us;
    next_sample_us += 2500;
    ++sample_count_;
    return true;
}
void trigger_begin() {}
void trigger_stop() { ++stop_calls; }
bool trigger_pop(TriggerQueueEvent&) { return false; }
bool trigger_is_armed() { return false; }
uint32_t trigger_drop_count() { return 0; }
gw_sync::AckStatus trigger_set_config(const gw_sync::GroupConfig*, uint8_t) {
    return gw_sync::AckStatus::Ok;
}
gw_sync::AckStatus trigger_arm() { return gw_sync::AckStatus::Ok; }
gw_sync::AckStatus trigger_test_output(uint8_t) { return gw_sync::AckStatus::Ok; }
namespace timebase {
uint64_t now_us() { return fake_time_us; }
uint32_t now_us32() { return static_cast<uint32_t>(fake_time_us); }
}
}  // namespace gw_fw

class FirmwareMainTest : public ::testing::Test {
    void SetUp() override {
        fake_time_us = 0;
        Serial = FakeUsbSerial{};
        stop_calls = 0;
        g_imu = gw_fw::Bmi088{};
        g_was_usb_connected = false;
        g_last_heartbeat_us = 0;
        g_usb_errors = 0;
        g_rx_size = 0;
        setup();
    }
};

TEST_F(FirmwareMainTest, UsbChecksDoNotDelay400HzSampling) {
    for (unsigned i = 0; i < 400; ++i) {
        fake_time_us += 2500;
        loop();
    }
    EXPECT_EQ(fake_time_us, 1'000'000u);
    unsigned imu_frames = 0;
    for (const auto& frame : Serial.frames) {
        if (frame[3] == static_cast<uint8_t>(gw_sync::MessageType::ImuBatch)) {
            ++imu_frames;
        }
    }
    EXPECT_EQ(imu_frames, 400u);
    EXPECT_EQ(g_usb_errors, 0u);
}

TEST_F(FirmwareMainTest, DroppingDtrStopsOutputsWithoutWaiting) {
    loop();
    Serial.connected = false;
    fake_time_us = 2500;
    loop();
    EXPECT_EQ(stop_calls, 1u);
    EXPECT_EQ(fake_time_us, 2500u);
    EXPECT_TRUE(Serial.frames.empty());
}
