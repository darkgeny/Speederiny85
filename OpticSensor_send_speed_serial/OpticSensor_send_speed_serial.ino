/***************************************************
    Author: Giacomo Catanzariti <darkgeny@gmail.com>
    https://github.com/darkgeny/Speederiny85
/**************************************************/

#include "SoftwareSerial.h"
#include "OpticSensor20.h"

#ifdef LOCALSERIAL
  #define THESERIAL Serial
#else
  #define THESERIAL mySerial
#endif

SoftwareSerial mySerial(2, 3); // RX, TX

//uncomment this is you want test via USB
//comment this if you update code on attiny 85
//#define LOCALSERIAL

#ifdef LOCALSERIAL
OpticSensor20 optic(13, 500);//sensor pin
#else
OpticSensor20 optic(0, 500);//sensor pin
#endif

void setup() {
  THESERIAL.begin(38400);
  delay(1000);
  THESERIAL.println("started!");
}
void loop() {
  optic.check_sensor();
  wait_request_from_serial();
}
