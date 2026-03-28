#include <gtest/gtest.h>
#include "AM2302-Sensor_Tools.h"

// =====================================================
//  validateChecksum
// =====================================================

TEST(ValidateChecksumTest, ValidChecksum) {
  // hum = 0x02D4 (724 = 72.4%), temp = 0x00ED (237 = 23.7°C)
  // checksum = (0x02 + 0xD4 + 0x00 + 0xED) & 0xFF = 0xC3
  EXPECT_TRUE(AM2302_Tools::validateChecksum(0x02, 0xD4, 0x00, 0xED, 0xC3));
}

TEST(ValidateChecksumTest, InvalidChecksum) {
  EXPECT_FALSE(AM2302_Tools::validateChecksum(0x02, 0xD4, 0x00, 0xED, 0x00));
}

TEST(ValidateChecksumTest, AllZeroValid) {
  // all null: checksum must be 0
  EXPECT_TRUE(AM2302_Tools::validateChecksum(0x00, 0x00, 0x00, 0x00, 0x00));
}

// =====================================================
//  decode humidity data
// =====================================================

TEST(DecodeHumidityTest, NormalHumidity) {
  // 0x02D4 = 724 → 72.4%
  float hum = AM2302_Tools::decode_humidity_data(0x02, 0xD4);
  EXPECT_EQ(hum, 72.4f);
}

TEST(DecodeHumidityTest, LowHumidity) {
  // 0x0190 = 400 → 40.0%
  float hum = AM2302_Tools::decode_humidity_data(0x01, 0x90);
  EXPECT_EQ(hum, 40.0f);
}

TEST(DecodeHumidityTest, MaxHumidity) {
  // 0x03E8 = 1000 → 100.0%
  float hum = AM2302_Tools::decode_humidity_data(0x03, 0xE8);
  EXPECT_EQ(hum, 100.0f);
}

TEST(DecodeHumidityTest, ZeroHumidity) {
  EXPECT_EQ(AM2302_Tools::decode_humidity_data(0x00, 0x00), 0.0f);
}

// =====================================================
//  decode temperature data for original AM2302 sensors
// =====================================================

TEST(DecodeTemperatureTest, PositiveTemperature) {
  // 0x00ED = 237 → 23.7°C
  float temp = AM2302_Tools::decode_temperature_data(0x00, 0xED);
  EXPECT_EQ(temp, 23.7f);
}

TEST(DecodeTemperatureTest, ZeroDegrees) {
  // 0x0000 = 0 → 0.0°C
  EXPECT_EQ(AM2302_Tools::decode_temperature_data(0x00, 0x00), 0.0f);
}

TEST(DecodeTemperatureTest, NegativeTemperature) {
  // Bit 15 set = negative: 0x8020 → -(0x0020) = -32 → -3.2°C
  float temp = AM2302_Tools::decode_temperature_data(0x80, 0x20);
  EXPECT_EQ(temp, -3.2f);
}

TEST(DecodeTemperatureTest, NegativeTemperatureLarge) {
  // 0x8190 = -(0x0190) = -400 → -40.0°C
  float temp = AM2302_Tools::decode_temperature_data(0x81, 0x90);
  EXPECT_EQ(temp, -40.0f);
}

// Regressions-Test: High-Bit-Shift-Bug
TEST(DecodeTemperatureTest, HighBitShiftRegression) {
  // temp_high = 0x81 hat Bit 7 gesetzt → früher Overflow auf AVR
  // has to be implemented as negative temperature, no UB!
  float temp = AM2302_Tools::decode_temperature_data(0x81, 0x00);
  EXPECT_EQ(temp, -25.6f);
}

// =====================================================
//  decode_temperature_data for cloned AM2302 sensors
// =====================================================

// Test with decode negative temperatures as two complement => should fail
TEST(DecodeTemperatureTest, minus40_degrees_with_TC) {
  // 0xFE70 = -400 → -40.0°C
  float temp = AM2302_Tools::decode_temperature_data(0xFE, 0x70);
  EXPECT_EQ(temp, -40.0f);
}

TEST(DecodeTemperatureTest, minus18_degrees_with_TC) {
  // 0xFF4C = -180 → -18.0°C
  float temp = AM2302_Tools::decode_temperature_data(0xFF, 0x4C);
  EXPECT_EQ(temp, -18.0f);
}
