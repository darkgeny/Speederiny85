/***************************************************
    Author: Giacomo Catanzariti <darkgeny@gmail.com>
    https://github.com/darkgeny/Speederiny85
**************************************************/

#ifndef TAKEVALUESSERIAL_H
#define TAKEVALUESSERIAL_H
#include "Arduino.h"
#include "SoftwareSerial.h"

class TakeValuesSerial {
  public:
    TakeValuesSerial();
    TakeValuesSerial(uint8_t, SoftwareSerial *);
    void take();
    int get_speed();
    unsigned long get_steps();
    bool have_taked_speed(); //callme and I change status
    bool have_taked_steps(); //callme and I change status
    void set_local_speed_interval(unsigned long);
    SoftwareSerial *tvserial;
  private:
    void set_remote_speed_interval();
    bool serial_flow_started;
    unsigned long remote_speed_interval;

    bool isNumeric(String str);
    void reset_vars();
    int speed;
    unsigned long steps;
    bool taked_speed;
    bool taked_steps;
    char commands[2]; //s=speed, p=path
    String valuetmp;
    String value;
    int maxchars;
    int cnt_char;
};
#endif