/*__AM2302_SENSOR_H__
*	AM2302-Sensor.h
*
*	Author: Frank Häfele
*	Date:	21.11.2023
*
*	Object: Measure Sensor Data of AM2302-Sensor
*
*/

#ifndef __AM2302_SENSOR_H__
#define __AM2302_SENSOR_H__

#include <Arduino.h>

namespace AM2302 {

    constexpr int8_t READ_OK 		{0};
    constexpr int8_t ERROR_CHECKSUM	{-1};
    constexpr int8_t ERROR_TIMEOUT	{-2};

    // define timeout in 100 µs
    constexpr uint8_t READ_TIMEOUT	{100U};

    class AM2302_Sensor {
        
        public:
            void setup();
            int8_t read();
            float get_Temperature() {return _temp * 0.1F;}
            float get_Hunidity() {return _hum * 0.1F;}

        
        private:
            uint8_t _data[5];
            uint16_t _hum {0};
            int16_t _temp {0};
            uint8_t _pin;
            bool _checksum_ok {false};

            int8_t read_sensor_data(int8_t *buffer, uint8_t size);
    };
}
#endif