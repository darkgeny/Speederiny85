#include "TakeValuesSerial.h"
#include "SoftwareSerial.h"

SoftwareSerial attiny85(2, 3); // RX, TX
TakeValuesSerial TVSerial(7, &attiny85);

unsigned long last_time=0,passed_time=0;

void setup(){
  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  attiny85.begin(38400);
  Serial.println("Serial started!");
}
void loop(){
  if (Serial.available())
    attiny85.write(Serial.read());

  TVSerial.take();

  if( TVSerial.have_taked_speed() ){
    Serial.print("TVSerial.speed:");
    Serial.println( TVSerial.get_speed() );
  }
  if( TVSerial.have_taked_steps() ){
    Serial.print(" TVSerial.steps:");
    Serial.println( TVSerial.get_steps() );
  }
}
