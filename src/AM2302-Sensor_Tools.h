#ifndef __AM2302_SENSOR_TOOLS_H__
#define __AM2302_SENSOR_TOOLS_H__

#include <stdint.h>
#include <math.h>

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
    * @brief decode data for temperature value in signed magnitude and two complement
    * 
    * @param temp_high high byte of temperature
    * @param temp_low low byte of temperature
    * @return float temperature value
    */
   inline float decode_temperature_data(uint8_t temp_high, uint8_t temp_low) {
      uint16_t temp_data_raw = (static_cast<uint16_t>(temp_high) << 8) | temp_low;

      // ----------------------------------------------
      // 1.) Temperature value in signed magnitude (sm)
      // ----------------------------------------------
      int16_t temp_value_sm = static_cast<int16_t>(temp_data_raw & 0x7FFF);
      bool temp_sign = (temp_data_raw & 0x8000);

      if (temp_sign) {
         temp_value_sm = -temp_value_sm;
      }

      // ----------------------------------------------
      // 2.) Temperature value in two complement (tc)
      // ----------------------------------------------
      int16_t temp_value_tc = static_cast<int16_t>(temp_data_raw);

      // ----------------------------------------------
      // 3.) Plausibility check
      // ----------------------------------------------

      auto isValid = [] (int16_t temp) {
         return (temp >= -400 && temp <= 800);
      };

      bool isSmValid = isValid(temp_value_sm);
      bool isTcValid = isValid(temp_value_tc);

      // ----------------------------------------------
      // 4.) Desicion
      // ----------------------------------------------
      
      // sm is master due to datasheet description
      if (isSmValid) {
         return temp_value_sm / 10.0f;
      }
      else if (isTcValid) {
         // non standard decoding detected
         return temp_value_tc / 10.0f;
      }
      else {
         // both values are invalid
         return NAN;
      }
   }
}
#endif
