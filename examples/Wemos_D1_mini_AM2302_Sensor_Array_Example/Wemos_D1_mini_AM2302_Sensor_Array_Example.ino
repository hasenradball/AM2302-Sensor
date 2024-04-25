/*
*	Wemos_D1_mini_Sensor_Array_Example.ino
*
*	Author: Frank Häfele
*	Date:	25.04.2024
*
*	Objective:
    Measure sensor data of AM2302-Sensor-Array with Arduino IDE.
    Use 3 sensors and compare them with serial output 
*/

#include <Arduino.h>
#include <AM2302-Sensor.h>
#include <ESP8266WiFi.h>


// Create Sensor Object array with std:array
std::array<AM2302::AM2302_Sensor, 3> sensor_arr{
  AM2302::AM2302_Sensor{4},
  AM2302::AM2302_Sensor{5},
  AM2302::AM2302_Sensor{0}
};


void setup() {
  WiFi.mode(WIFI_OFF);
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial) {
    yield();
  }
  Serial.print(">>> Test Sensor Array of AM2302 (DHT22) Sensors <<<\n\n");
  delay(5000);

  for (size_t i = 0; i < sensor_arr.size(); ++i) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" available - ");
    Serial.println(sensor_arr[i].begin());
  }
  Serial.print("\n");
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print("\tSensor Status : ");
  for (size_t i = 0; i < sensor_arr.size(); ++i) {
    Serial.print(sensor_arr[i].get_sensorState(sensor_arr[i].read()));
    Serial.print("\t");
  }
  Serial.print("\n\tTemperature   : ");
  for (size_t i = 0; i < sensor_arr.size(); ++i) {
    Serial.print(sensor_arr[i].get_Temperature());
    Serial.print("\t");
  }
  Serial.print("\n\tHumidity      : ");
  for (size_t i = 0; i < sensor_arr.size(); ++i) {
    Serial.print(sensor_arr[i].get_Humidity());
    Serial.print("\t");
  }
  Serial.print("\n\n");
  delay(10000);
}
