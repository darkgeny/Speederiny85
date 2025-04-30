/***************************************************
    Author: Giacomo Catanzariti <darkgeny@gmail.com>
    https://github.com/darkgeny/Speederiny85
**************************************************/

#include "TakeValuesSerial.h"

#define RXD1 5
#define TXD1 17
HardwareSerial tiny85_A(1);


#define RXD2 19
#define TXD2 18
HardwareSerial tiny85_B(2);

TakeValuesSerial optic_A( &tiny85_A );
TakeValuesSerial optic_B( &tiny85_B );

char row[] = "                                                 ";
int sA=0,sB=0;
unsigned long pA=0,pB=0;
HardwareSerial *hs1, *hs2;

void setup(){
  hs1 = &tiny85_A;
  hs2 = &tiny85_B;

  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  delay(2000);
  Serial.println("Serial started!");

  optic_A.tvserial = hs1;
  optic_A.tvserial->begin(38400, SERIAL_8N1, RXD1, TXD1);
 
  optic_B.tvserial = hs2;
  optic_B.tvserial->begin(38400, SERIAL_8N1, RXD2, TXD2);
 
  Serial.println("\n\n\n\n\n\n\nI have set interval -for take speed- every 1750 milliseconds.\n(otherwise, default value is 500).");
  delay(2000);
}
String recstring="";
int change_speed_so = 0;
void loop(){
  while( Serial.available() > 0 ){
    char c = Serial.read();
    if( c == '\r' ) continue;
    if( c == '\n' )
      if( recstring.length() ){
        change_speed_so = recstring.toInt();
        recstring = "";
        continue;
      }
    recstring += c;
  }
  if( change_speed_so ){
    optic_A.set_remote_speed_interval( change_speed_so );
    optic_B.set_remote_speed_interval( change_speed_so );
    change_speed_so = 0;
    optic_A.sync();
    optic_B.sync();
  }

  /*while( Serial.available() > 0 )
    tiny85_A.write( Serial.read() );
  while( tiny85_A.available() > 0 )
    Serial.write( tiny85_A.read() );*/

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
