/***************************************************
    Author: Giacomo Catanzariti <darkgeny@gmail.com>
    https://github.com/darkgeny/Speederiny85
**************************************************/

#ifndef TAKEVALUESSERIAL_H
#define TAKEVALUESSERIAL_H
#include "Arduino.h"
#include "SoftwareSerial.h"

struct JOYMOVE_DATA{
  int cX,cY; //centered: C,c
  int mX,mY; //minimum: N,n
  int MX,MY; //maximum: M,m
  int dirX,dirY; //directions: D,d
  int dirXY;//other directions: R
  int centeredX, centeredY;//if centered anyone the it's true: X,x
  int CX,CY; //calibrated: A,B
};
typedef struct JOYMOVE_DATA JOYMOVE;

struct JOYMOVE_DATA_TAKED{
  bool cX,cY; //centered: C,c
  bool mX,mY; //minimum: N,n
  bool MX,MY; //maximum: M,m
  bool dirX,dirY; //directions: D,d
  bool dirXY;//other directions: R
  bool centeredX, centeredY;//if centered anyone the it's true: X,x
  bool CX,CY; //calibrated: A,B
};
typedef struct JOYMOVE_DATA_TAKED JOYMOVE_TAKED;

class TakeValuesSerial {
  public:
    TakeValuesSerial();
    TakeValuesSerial( SoftwareSerial * );
    void take();
    int get_sensor_speed();
    unsigned long get_sensor_steps();
    bool have_taked_sensor_speed(); //callme and I change status
    bool have_taked_sensor_steps(); //callme and I change status

    int get_joy(int);
    bool have_taked_joy(int);

    void set_local_speed_interval(unsigned long);
    SoftwareSerial *tvserial;
    void update();
    enum JOY_STRUCT_MEMBER { cX,cY,mX,mY,MX,MY,dirX,dirY,dirXY,centeredX, centeredY,CX,CY };
  private:
    JOYMOVE joymove;
    JOYMOVE_TAKED joymove_taked;
    void set_remote_speed_interval();
    bool serial_flow_started;
    unsigned long remote_speed_interval;

    bool isNumeric(String str);
    void reset_vars();
    int sensor_speed;
    unsigned long sensor_steps;
    bool taked_sensor_speed;
    bool taked_sensor_steps;
    char commands_sensor[3]; //s=speed, p=path
    char commands_joystick[15]; //w=word; chars 4->10 = JOYMOVE_DATA struct
    String valuetmp;
    String value;
    int maxchars;
    int cnt_char;
    int cmd_words_num   = 4;
    const char *cmd_words[4] = {  "READY",  //connected, start receiving commands
                                  "CALM",   //calibrate motor
                                  "STOP",   //speed motor zero
                                  "HEART" };
    int get_cmd_word(char *);
    enum CMD_WORD { READY, CALM, STOP, HEART, unknown=-1 };
    enum JOY_DIRECTION { BACKWARD, FORWARD };
  };
#endif