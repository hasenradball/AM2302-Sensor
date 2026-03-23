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
   bool validateChecksum(const uint8_t hum_h, const uint8_t hum_l,
                         const uint8_t temp_h, const uint8_t temp_l,
                         const uint8_t cs);

   /**
    * @brief decode data for humidity value
    * 
    * @param hum_high hugh byte of humidity
    * @param hum_low low byte of humidity
    * @return float humidity value
    */
   float decode_humidity_data(const uint8_t hum_high, const uint8_t hum_low);

   /**
    * @brief decode data for temperature value
    * 
    * @param temp_high high byte of temperature
    * @param temp_low low byte of temperature
    * @return float temperature value
    */
   float decode_temperature_data(const uint8_t temp_high, const uint8_t temp_low);
}

#endif