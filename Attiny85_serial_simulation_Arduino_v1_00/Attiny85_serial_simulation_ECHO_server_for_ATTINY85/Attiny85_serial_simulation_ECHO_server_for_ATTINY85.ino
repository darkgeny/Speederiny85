/*************************************
This code is useful for simulate serial communication.
This code is simply a serial echo server sender (to client SoftwareSerial).
This code is valid for both Attiny and Arduino and for ESP32.
*************************************/
#include "SoftwareSerial.h"

SoftwareSerial serial(2, 3);

void setup(){
  serial.begin(38400);
}
void loop(){
  while(serial.available()){
    serial.write(serial.read());
  }
}
/****************************************
This is client code:

#include "SoftwareSerial.h"

SoftwareSerial server(2, 3);  // RX, TX
void setup(){
  server.begin(38400);
}
void loop() {
  while (Serial.available()){
    server.write(Serial.read());
    take();
  }
}
void take(){
  //polling from serial
  while (server->available()){
    ...do something...
  }
}
*****************************************/