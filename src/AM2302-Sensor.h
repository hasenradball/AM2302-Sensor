/**
 * @file    AM2302-Sensor.h
 * @author  Frank Häfele
 * @date    23.03.2026
 * @version 1.5.0
 * @brief   Measure Temperature and Humidity of AM2302-Sensor
 */


#ifndef __AM2302_SENSOR_H__
#define __AM2302_SENSOR_H__

#include <Arduino.h>
#include "AM2302-Sensor_Tools.h"

namespace AM2302 {

  constexpr const char * AM2302_STATE_OK{"OK"};
  constexpr const char * AM2302_STATE_ERR_CKSUM{"Error: Checksum"};
  constexpr const char * AM2302_STATE_ERR_TIMEOUT{"Error: Timeout"};
  constexpr const char * AM2302_STATE_ERR_READ_FREQ{"Error: Read Frequency"};

  constexpr int8_t AM2302_READ_OK          {0};
  constexpr int8_t AM2302_ERROR_CHECKSUM   {-1};
  constexpr int8_t AM2302_ERROR_TIMEOUT    {-2};
  constexpr int8_t AM2302_ERROR_READ_FREQ  {-3};

  // define timeout in 100 µs
  constexpr uint8_t READ_TIMEOUT         {100U};
  
  // define maximum sensor read frequency in milliseconds (2 s)
  constexpr uint16_t READ_FREQUENCY      {2000U};

  class AM2302_Sensor {

    public:
      /**
       * @brief Construct a new am2302::am2302 sensor::am2302 sensor object
       * 
       * @param pin Pin for AM2302 sensor
       */
      explicit AM2302_Sensor(uint8_t pin);

      /**
       * @brief begin function, setup pin and run sensor check.
       * 
       * @return true if sensor check is successful.
       * @return false if sensor check failed.
       */
      bool begin();

      /**
       * @brief read function, call of read_sensor()
       * 
       * @return sensor status
      */
      int8_t read();


      inline float get_Temperature() const {return _temp;}
      inline float get_Humidity() const {return _hum;}

      /**
       * @brief get Sensor State in human readable manner
       * 
       * @return sensor state : OK, Checksum Error or Timeout Error
      */
      static const char * get_sensorState(int8_t state);

    private:
      // memory millis between last read
      unsigned long _millis_last_read;
      // memory for sensor data
      uint8_t _data[5] = {0};
      // holds humidity
      float _hum {0.0f};
      // holds temperature
      float _temp {0.0f};
      // sensor pin
      uint8_t _pin;
      // holds checksum state
      bool _checksum_ok {false};

    /**
     * @brief wait for a specific pin state
     * 
     * @param state state to wait for
     * @return int8_t status
     */
    int8_t await_pin_state(uint8_t state);

    /**
     * @brief initiate start sequence, setup pins and call of read_ sensor_data()
     * 
     * @return sensor status
    */
    int8_t read_sensor();

    /**
     * @brief read 40 bit of sensor data
     * 
     * @param buffer data buffer for 40 bit
     * @param size of buffer => 5 Byte
     * @return int8_t 
     */
    int8_t read_sensor_data(uint8_t *buffer, uint8_t const size);

    /**
     * @brief reset temperature and humidity data
     * 
     */
    void resetData();
  };
}

namespace AM2302_Tools {
   /**
   * @brief helper function to print byte as bits
   * 
   * @param value byte with 8 bits
   */
   void print_byte_as_bit(const uint8_t value);
}

#endif
