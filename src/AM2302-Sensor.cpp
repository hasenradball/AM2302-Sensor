/**
 * @file    AM2302-Sensor.cpp
 * @author  Frank Häfele
 * @date    23.03.2026
 * @version 1.5.0
 * @brief   Measure Temperature and Humidity of AM2302-Sensor
 */

#include <AM2302-Sensor.h>
#include <string.h>

AM2302::AM2302_Sensor::AM2302_Sensor(uint8_t pin) : _millis_last_read{0}, _pin{pin}
{}

bool AM2302::AM2302_Sensor::begin() {
   pinMode(_pin, INPUT_PULLUP);
   // required delay() for a secure sensor check,
   // if you reset the mcu very fast one after another
   auto tic{millis()};
   while ( millis() - tic < READ_FREQUENCY ) {
      yield();
   }
   auto status{read()};
   _millis_last_read = millis();
   if (status == AM2302_READ_OK) {
      return true;
   }
   else {
      return false;
   }
}

int8_t AM2302::AM2302_Sensor::read() {
   auto status{read_sensor()};
   
   if (status == AM2302_ERROR_TIMEOUT) {
      resetData();
   }
   return status;
}

int8_t AM2302::AM2302_Sensor::read_sensor() {
   // check read frequency
   if ( millis() - _millis_last_read < READ_FREQUENCY) {
      return AM2302_ERROR_READ_FREQ;
   }
   _millis_last_read = millis();
   // *****************************
   //  === send start sequence ===
   // ****************************
   // start from HIGH ==> switch to LOW for min. of 1 ms
   // Set pin to Output
   pinMode(_pin, OUTPUT);
   // set Pin to LOW 
   digitalWrite(_pin, LOW);
   // wait min. 1,0 ms
   delayMicroseconds(1200U);
   // Set port to HIGH ==> INPUT_PULLUP with PullUp
   digitalWrite(_pin, HIGH);
   pinMode(_pin, INPUT_PULLUP);
   // delay_us(30.0); not needed

   // ******************************
   //  === wait for Acknowledge ===
   // ******************************
   // Acknowledge Sequence 80us LOW 80 us HIGH
   // wait for LOW (80 µs)
   if (await_pin_state(0) == AM2302_ERROR_TIMEOUT) return AM2302_ERROR_TIMEOUT;
   // wait for HIGH (80 µs)
   if (await_pin_state(1) == AM2302_ERROR_TIMEOUT) return AM2302_ERROR_TIMEOUT;

   // *****************************
   //  ==== Read Sensor Data ====
   // *****************************
   // ==> START of time critical code <==
   // read 40 bits from sensor into the buffer:
   // ==> HIGH state is 70 µs
   // ==> LOW state is 28 µs
   memset(_data, 0, sizeof(_data));
   if (read_sensor_data(_data, 5U) == AM2302_ERROR_TIMEOUT) {
      return AM2302_ERROR_TIMEOUT;
   }
   // ==> END of time critical code <==
   
   _checksum_ok = AM2302_Tools::validateChecksum(_data[0], _data[1], _data[2], _data[3], _data[4]);

   if (_checksum_ok) {
      _hum = AM2302_Tools::decode_humidity_data(_data[0], _data[1]);
      _temp = AM2302_Tools::decode_temperature_data( _data[2], _data[3]);
      return AM2302_READ_OK;
   }
   else {
      return AM2302_ERROR_CHECKSUM;
   }
}

int8_t AM2302::AM2302_Sensor::await_pin_state(uint8_t state) {
   uint8_t wait_counter{0}, state_counter{0};
   // count wait for state time
   while ( (digitalRead(_pin) != state) ) {
      ++wait_counter;
      delayMicroseconds(1U);
      if (wait_counter >= READ_TIMEOUT) {
         return AM2302_ERROR_TIMEOUT;
      }
   }
   // count state time
   while ( (digitalRead(_pin) == state) ) {
      ++state_counter;
      delayMicroseconds(1U);
      if (state_counter >= READ_TIMEOUT) {
         return AM2302_ERROR_TIMEOUT;
      }
   }
   return (state_counter > wait_counter);
}

int8_t AM2302::AM2302_Sensor::read_sensor_data(uint8_t *buffer, uint8_t size) {
   for (uint8_t i = 0; i < size; ++i) {
      for (uint8_t bit = 0; bit < 8U; ++bit) {
         int8_t bit_value = await_pin_state(1);
         if (bit_value < 0) {
            // error occured
            return bit_value;
         }
         buffer[i] = (buffer[i] << 1) | static_cast<uint8_t>(bit_value);
      }
   }
   return AM2302_READ_OK;
}

const char * AM2302::AM2302_Sensor::get_sensorState(int8_t state) {
   if(state == AM2302_READ_OK) {
      return AM2302_STATE_OK;
   }
   else if(state == AM2302_ERROR_CHECKSUM) {
      return AM2302_STATE_ERR_CKSUM;
   }
   else if(state == AM2302_ERROR_TIMEOUT) {
      return AM2302_STATE_ERR_TIMEOUT;
   }
   else {
      return AM2302_STATE_ERR_READ_FREQ;
   }
}

void AM2302::AM2302_Sensor::resetData() {
   // reset tem to -255 and hum to 0 as indication
   _temp = -2550;
   _hum = 0;
}

void AM2302_Tools::print_byte_as_bit(uint8_t value) {
   for (int i = 7; i >= 0; --i) {
      char c = (value & (1 << i)) ? '1' : '0';
      Serial.print(c);
   }
   Serial.println();
}