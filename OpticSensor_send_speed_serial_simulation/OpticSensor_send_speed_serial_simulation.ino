/***************************************************
    Author: Giacomo Catanzariti <darkgeny@gmail.com>
    https://github.com/darkgeny/Speederiny85
**************************************************/

#include "TakeValuesSerial.h"

SoftwareSerial optic_A_attiny85(2, 3); // RX, TX
SoftwareSerial optic_B_attiny85(4, 5); // RX, TX

TakeValuesSerial take_value_optic_A(7, &optic_A_attiny85);
TakeValuesSerial take_value_optic_B(7, &optic_B_attiny85);

unsigned long last_time=0,passed_time=0;

void setup(){
  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Serial started!");

  optic_A_attiny85.begin(38400);
  //delay(1000);
  //optic_B_attiny85.begin(38400);
  //delay(1000);
  take_value_optic_A.set_local_speed_interval(1000);
  //take_value_optic_B.set_local_speed_interval(1000);
  Serial.println("Set interval for take speed every 1000 milliseconds.\n(otherwise, default value is 500).");
}
void loop(){
  char c = '\0';
  if (Serial.available())
    c = Serial.read();
  if( c )
    take_value_optic_A.tvserial->write( c );
    optic_A_attiny85.write( c );

  take_value_optic_A.take();

  if( take_value_optic_A.have_taked_speed() ){
    Serial.print("speed A taken:");
    Serial.println( take_value_optic_A.get_speed() );
  }
  if( take_value_optic_A.have_taked_steps() ){
    Serial.print("steps A taken:");
    Serial.println( take_value_optic_A.get_steps() );
  }
/*
  if( c )
    optic_B_attiny85.write( c );

  take_value_optic_B.take();

  if( take_value_optic_B.have_taked_speed() ){
    Serial.print("speed B taken:");
    Serial.println( take_value_optic_B.get_speed() );
  }
  if( take_value_optic_B.have_taked_steps() ){
    Serial.print("steps B taken:");
    Serial.println( take_value_optic_B.get_steps() );
  }
*/
}
