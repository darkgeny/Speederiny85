/**********************************************
    https://github.com/darkgeny/Speederiny85
    Author: Giacomo Catanzariti darkgeny@gmail.com
    Name: TakeValuesSerial
    Purpose: take values from "OpticSensor_send_speed_serial" that control the rotation speed and steps of the path
    of optoelectric sensor and enable client-server simple serial communication
    Operational commands:
    1) #s = will receive actual speed from sensor
    2) #S<num> = set interval resolution for taking sensor speed on sensor
    3) #p = will receive actual steps gathered from sensor
    4) #P = reset to ZERO gathered steps on sensor
    Note:
    values received are formatted so:

    #s<speed>#

    and 

    #p<steps>#
*********************************************/

#include "TakeValuesSerial.h"
//
TakeValuesSerial::TakeValuesSerial(){};
TakeValuesSerial::TakeValuesSerial(uint8_t mc, SoftwareSerial *ser){//max chars
  valuetmp="";
  value="";
  maxchars = mc;
  cnt_char = 0;
  speed = 0;
  steps = 0;
  taked_speed = false;
  taked_steps = false;
  commands[0] = 's';
  commands[1] = 'p';
  tvserial = ser;
  tvserial->begin(38400);
}
void TakeValuesSerial::take(){
  char c = '\0';
  //polling from serial
  while (tvserial->available()) { 
    if( cnt_char > maxchars ) //else overflow
      reset_vars();
    c = tvserial->read();
    if( c == '\r' ) continue; //drop this special char
    if( c != '\n' ){ //adding c to value
      valuetmp += c;
      cnt_char++;
      continue;
    }else{
      if( valuetmp[0] == '#' && valuetmp[valuetmp.length()-1] == '#') //result is ready?
        if( valuetmp.length() > 3 ) //value into?
          if( strchr(commands, valuetmp[1]) != NULL ) //have command char?
            if( isNumeric( valuetmp ) ){ //contain ready formatted number?
              switch( valuetmp[1] ){
                case 's': //save last speed
                    speed = valuetmp.substring( 2, valuetmp.length()-1 ).toInt();
                    taked_speed = true;
                    serial_flow_started = true;
                    break;
                case 'p': //save last steps
                    steps = valuetmp.substring( 2, valuetmp.length()-1 ).toInt();
                    taked_steps = true;
                    break;
                default: break;
              }
            }
      reset_vars();
    }
  }
}
void TakeValuesSerial::reset_vars(){
  valuetmp="";
  cnt_char = 0;
  set_remote_speed_interval();
}
bool TakeValuesSerial::isNumeric(String str){
  uint8_t cnt=0;
  for(byte i=2;i<str.length()-1;i++)
    if(isDigit(str.charAt(i))) cnt++;
  if( cnt == ( str.length() -3 ) )
    return true;
  return false;
}
int TakeValuesSerial::get_speed(){
  return speed;
}
unsigned long TakeValuesSerial::get_steps(){
  return steps;
}
bool TakeValuesSerial::have_taked_speed(){
  bool last_status = taked_speed;
  taked_speed = false;
  return last_status;
}
bool TakeValuesSerial::have_taked_steps(){
  bool last_status = taked_steps;
  taked_steps = false;
  return last_status;
}
void TakeValuesSerial::set_local_speed_interval(unsigned long si){
  remote_speed_interval = si;
}
/*Sometimes Attiny activates the serial slowly,
so if you want to set the interval in the setup,
better be sure that the serial has started.
This function is called ALWAYS, but it sends
the command only if the serial number is started
and the value is greater than 9
*/
void TakeValuesSerial::set_remote_speed_interval(){
  if( !serial_flow_started || remote_speed_interval < 10 )
    return;
  char rsint[20];
  sprintf(rsint,"#S%d\n", (int)remote_speed_interval );
  tvserial->write( rsint );
  remote_speed_interval = 0; //one time setted and not over
}
void TakeValuesSerial::update(){
  if( !tvserial->isListening() )
    tvserial->listen();
    tvserial->write( "#s\n" );
    delay(2);
    tvserial->write( "#p\n" );
    delay(2);
  take();
}