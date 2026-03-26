#ifndef __AM2302_SENSOR_TOOLS_H__
#define __AM2302_SENSOR_TOOLS_H__

#include <stdint.h>

namespace AM2302_Tools {
   /**
    * @brief function validating the data against checksum
    * 
    * @param hum_h high byte if humidity
    * @param hum_l low byte of humidity
    * @param temp_h high byte of temperature
    * @param temp_l low byte of temperature 
    * @param cs checksum byte
    * @return true 
    * @return false 
    */
   inline bool validateChecksum(const uint8_t hum_h, const uint8_t hum_l,
                         const uint8_t temp_h, const uint8_t temp_l,
                         const uint8_t cs) {
      return (cs == ( (hum_h + hum_l + temp_h + temp_l) & 0xFF) );
   }

   /**
    * @brief decode data for humidity value
    * 
    * @param hum_high hugh byte of humidity
    * @param hum_low low byte of humidity
    * @return float humidity value
    */
   inline float decode_humidity_data(const uint8_t hum_high, const uint8_t hum_low) {
      return ( (static_cast<uint16_t>(hum_high) << 8) | hum_low) / 10.0F;
   }

   /**
    * @brief decode data for temperature value
    * 
    * @param temp_high high byte of temperature
    * @param temp_low low byte of temperature
    * @return float temperature value
    */
   inline float decode_temperature_data(const uint8_t temp_high, const uint8_t temp_low) {
      uint16_t temp_data_raw = (static_cast<uint16_t>(temp_high) << 8) | temp_low;

      uint16_t temp_mag = static_cast<uint16_t>(temp_data_raw & 0x7FFF);
      bool temp_sign = (temp_data_raw & 0x8000);
      
      float temp = temp_mag / 10.0F;

      if (temp_sign) {
         temp = -temp;
      }

      return temp;
   }
}

#endif