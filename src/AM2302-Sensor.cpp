/*
*	AM2302-Sensor.cpp
*
*	Author: Frank Häfele
*	Date:	21.11.2023
*
*	Object: Measure Sensor Data of AM2302-Sensor
*
*/
#include <AM2302-Sensor.h>

/**
 * @brief Construct a new am2302::am2302 sensor::am2302 sensor object
 * 
 * @param pin Pin for AM2302 sensor
 */
AM2302::AM2302_Sensor::AM2302_Sensor(uint8_t pin) : _pin{pin}
{}

/**
 * @brief begin function set Pin to INPUT_PULLUP with PullUp
 * 
 */
void AM2302::AM2302_Sensor::begin() {
    pinMode(_pin, INPUT_PULLUP);
}


int8_t AM2302::AM2302_Sensor::read() {
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
    await_state(0);
    // wait for HIGH (80 µs)
    await_state(1);

    // *****************************
    //  ==== Read Sensor Data ====
    // *****************************
    // ==> START of time critical code <==
    int8_t data_buffer[40U];
    // read 40 bits from sensor:
    // ==> HIGH state is 70 µs
    // ==> LOW state is 28 µs
    //for (uint8_t i = 0; i < 40U; ++i) {
    if (read_sensor_data(data_buffer, 40U) == AM2302_ERROR_TIMEOUT) {
        return AM2302_ERROR_TIMEOUT;
    }
    // ==> END of time critical code <==
    uint8_t count{0};
    int8_t val;
    for (uint8_t i = 0; i < 5U; ++i) {
        for (uint8_t j = 0; j < 8U; ++j) {
            _data[i] <<= 1;
            val = data_buffer[count++];
            _data[i] |= val;
        }
    }
    // check checksum
    _checksum_ok = (_data[4] == ( (_data[0] + _data[1] + _data[2] + _data[3]) & 0xFF) );

    /*
    // Code part to check the checksum
    // Due to older sensors have an bug an deliver wrong data
    auto d4 = _data[4];
    auto cs = ( (_data[0] + _data[1] + _data[2] + _data[3]) & 0xFF) ;
    Serial.print("delivered Checksum:  ");
    AM2302_Tools::print_byte_as_bit(d4);
    Serial.print("calculated Checksum: ");
    AM2302_Tools::print_byte_as_bit(cs);
    */

    if (_checksum_ok) {
        _hum  = (_data[0] << 8) | _data[1];
        _temp = (_data[2] << 8) | _data[3];
        return AM2302_READ_OK;	
    }
    else {
        return AM2302_ERROR_CHECKSUM;
    }
}

/**
 * @brief wait for a specific pin state
 * 
 * @param state state to wait for
 * @return int8_t status
 */
int8_t AM2302::AM2302_Sensor::await_state(uint8_t state) {
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

/**
 * @brief read sensor data
 * 
 * @param buffer data buffer of 40 bit
 * @param size of buffer => 40
 * @return int8_t 
 */
int8_t AM2302::AM2302_Sensor::read_sensor_data(int8_t *buffer, uint8_t size) {
    for (uint8_t i = 0; i < size; ++i) {
        uint8_t wait_counter{0}, state_counter{0};
        // count wait for state time
        while ( (!digitalRead(_pin)) ) {
            ++wait_counter;
            delayMicroseconds(1.0);
            if (wait_counter >= READ_TIMEOUT) {
                return AM2302_ERROR_TIMEOUT;
            }
        }
        // count state time
        while ( (digitalRead(_pin)) ) {
            ++state_counter;
            delayMicroseconds(1U);
            if (state_counter >= READ_TIMEOUT) {
                return AM2302_ERROR_TIMEOUT;
            }
        }
        buffer[i] = (state_counter > wait_counter);
    }
    return AM2302_READ_OK;
}

/**
 * @brief helper function to print byte as bit
 * 
 * @param value byte with 8 bits
 */
void AM2302_Tools::print_byte_as_bit(char value) {
    for (int i = 7; i >= 0; --i) {
        char c = (value & (1 << i)) ? '1' : '0';
        Serial.print(c);
    }
    Serial.println();
}