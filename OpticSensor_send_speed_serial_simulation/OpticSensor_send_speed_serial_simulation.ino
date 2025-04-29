/***************************************************
    Author: Giacomo Catanzariti <darkgeny@gmail.com>
    https://github.com/darkgeny/Speederiny85
**************************************************/

#include "TakeValuesSerial.h"

SoftwareSerial tiny85_A(2, 3); // RX, TX
SoftwareSerial tiny85_B(4, 5); // RX, TX

TakeValuesSerial optic_A(7, &tiny85_A);
TakeValuesSerial optic_B(7, &tiny85_B);

char row[] = "                                                 ";
int sA=0,sB=0;
unsigned long pA=0,pB=0;

void setup(){
  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Serial started!");

  optic_A.set_local_speed_interval(500);
  optic_B.set_local_speed_interval(500);
  Serial.println("\n\n\n\n\n\n\nI have set interval -for take speed- every 1000 milliseconds.\n(otherwise, default value is 500).");
  Serial.println("Please send command aa to take speed from sensor A and bb to take steps from sensor B.\n");
}
void loop(){
  optic_A.update();
  optic_B.update();

  if( optic_A.have_taked_speed() )
    sA = optic_A.get_speed();
  if( optic_B.have_taked_speed() )
    sB = optic_B.get_speed();
  if( optic_A.have_taked_steps() )
    pA = optic_A.get_steps();
  if( optic_B.have_taked_steps() )
    pB = optic_B.get_steps();

  // |   speed A |   speed B |   steps A |   steps B |
  sprintf(row, "| %7d | %7d | %7lu | %7lu |", sA, sB, pA, pB );
  Serial.println( row );
}
