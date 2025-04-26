/***************************************************
    Author: Giacomo Catanzariti <darkgeny@gmail.com>
    https://github.com/darkgeny/Speederiny85
/**************************************************/

#include "SoftwareSerial.h"
#include "OpticSensor20.h"
//
OpticSensor20::OpticSensor20(){}
OpticSensor20::OpticSensor20(uint8_t p, unsigned long im){
  set_pin(p);
  speed=tmpspeed=0;
  status=false;
  millisec=0;
  interval_millis=im;
  steps=tmpsteps=0;
}
void OpticSensor20::set_pin(uint8_t p){
  pin = p;
  pinMode(pin, INPUT);
}
void OpticSensor20::update(bool s){
  if ( s != status ){
    status = ! status;
    ++tmpspeed;
    ++tmpsteps;
    if(millisec == 0)
      millisec=millis();
  }
}
void OpticSensor20::check_sensor(){
  if (digitalRead(pin) == HIGH) {
    update(false);
  } else {
    update(true);
  }
  if( (millis()-millisec) > interval_millis){
    take();
    tmpspeed=0;
    millisec=millis();
  }
}
void OpticSensor20::set_interval_millis(unsigned long m){
  if(m<100)
    m=100;
  if(m>10000)
    m=10000;
  interval_millis=m;
}
void OpticSensor20::reset_steps(){
  steps = tmpsteps = 0;
  taked_steps = true;
}
unsigned long OpticSensor20::get_steps(){
  return steps;
}
int OpticSensor20::get_speed(){
  return speed;
}
void OpticSensor20::take(){
  speed = tmpspeed;
  taked_speed = true;
  if( tmpsteps > steps ){
    steps = tmpsteps;
    taked_steps = true;
  }
}
