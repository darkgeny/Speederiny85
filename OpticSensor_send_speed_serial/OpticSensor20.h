/***************************************************
    Author: Giacomo Catanzariti
    https://github.com/darkgeny/Speederiny85
/**************************************************/

#ifndef OPTICSENSOR20_H
#define OPTICSENSOR20_H
#include "Arduino.h"
class OpticSensor20 {
  public:
    OpticSensor20();
    OpticSensor20(uint8_t, unsigned long);
    void check_sensor();
    void set_interval_millis(unsigned long);
    int get_speed();
    void reset_steps();
    unsigned long get_steps();
  private:
    void set_pin(uint8_t);
    int speed, tmpspeed;
    unsigned long steps, tmpsteps;
    bool taked_speed;
    bool taked_steps;
    bool status;
    uint8_t pin;
    unsigned long millisec;
    unsigned long interval_millis;
    void update(bool);
    void take();
};
#endif
