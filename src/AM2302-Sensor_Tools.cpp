#include "AM2302-Sensor_Tools.h"

bool AM2302_Tools::validateChecksum(uint8_t hum_h, uint8_t hum_l, uint8_t temp_h, uint8_t temp_l, uint8_t cs) {
   // check checksum
   bool checksum_ok = (cs == ( (hum_h + hum_l + temp_h + temp_l) & 0xFF) );
   return checksum_ok;
}

float AM2302_Tools::decode_humidity_data(uint8_t hum_high, uint8_t hum_low) {
   return ( (static_cast<uint16_t>(hum_high) << 8) | hum_low) / 10.0F;
}

float AM2302_Tools::decode_temperature_data(uint8_t temp_high, uint8_t temp_low) {
   uint16_t temp_data_raw = (static_cast<uint16_t>(temp_high) << 8) | temp_low;

   uint16_t temp_mag = static_cast<uint16_t>(temp_data_raw & 0x7FFF);
   bool temp_sign = (temp_data_raw & 0x8000);
   
   float temp = temp_mag / 10.0F;

   if (temp_sign) {
      temp = -temp;
   }

   return temp;
}

