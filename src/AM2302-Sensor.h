/*
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

namespace AM2302 {

	//constexpr int8_t DHT_READ_OK {0};
	//constexpr int8_t DHT_ERROR_CHECKSUM	{-1};
	//constexpr int8_t DHT_ERROR_TIMEOUT	{-2};

	// define timeout in 100 µs
	//constexpr uint8_t DHT_TIMEOUT		{100U};

	class AM2302 {
		
		public:
			void setup();
			read();
			get_Temperature();
			get_Hunidity();
		
		private:
			int8_t _pin;
			uint16_t _hum {0};
			int16_t _temp {0};
			uint8_t _data[5];
	};
}
#endif
