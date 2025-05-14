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
TakeValuesSerial::TakeValuesSerial( SoftwareSerial *ser ){
  JOYMOVE_TAKED *j = &joymove_taked;
  j->cX = j->cY = 
  j->mX = j->mY = 
  j->MX = j->MY = 
  j->dirX = j->dirY = 
  j->dirXY = 
  j->centeredX = j->centeredY = 
  j->CX = j->CY = 
  taked_sensor_speed = taked_sensor_steps = false;

  valuetmp="";
  value="";
  maxchars = 8; //maximum number of char readed from remote serial attiny
  cnt_char = 0;
  sensor_speed = 0;
  sensor_steps = 0;
  //sensor 1 and sensor 2
  commands_sensor[0]  = 's'; //speed
  commands_sensor[1]  = 'p'; //steps
  commands_sensor[2]  = '\0'; //steps
  //joystick
  commands_joystick[0]  = 'w'; //single word commands
  commands_joystick[1]  = 'C'; //cX
  commands_joystick[2]  = 'c'; //cY
  commands_joystick[3]  = 'N'; //mX
  commands_joystick[4]  = 'n'; //mY
  commands_joystick[5]  = 'M'; //MX
  commands_joystick[6]  = 'm'; //MY
  commands_joystick[7]  = 'D'; //dirX
  commands_joystick[8] = 'd'; //dirY
  commands_joystick[9] = 'R'; //dirXY
  commands_joystick[10] = 'X'; //centeredX
  commands_joystick[11] = 'x'; //centeredY
  commands_joystick[12] = 'A'; //CX
  commands_joystick[13] = 'B'; //CY
  commands_joystick[14] = '\0'; //CY

  tvserial = ser;
  tvserial->begin(38400);
}
void TakeValuesSerial::take(){
  JOYMOVE *j = &joymove;
  JOYMOVE_TAKED *jt = &joymove_taked;
  char c = '\0',*word;
  int intvalue;
  word = valuetmp.c_str()+2;
  //polling from serial
  while (tvserial->available()) { 
    if( cnt_char > maxchars ) //else overflow
      reset_vars();
    c = tvserial->read();
    if( c == '\r' ) continue; //drop this special char
    if( c != '\n' && c != ';' ){ //adding c to value
      valuetmp += c;
      cnt_char++;
      continue;
    }else{
      if( valuetmp[0] == '#' && valuetmp[valuetmp.length()-1] == '#'){ //result is ready?
        if( valuetmp.length() > 3 ) //value into?
          if( strchr(commands_sensor, valuetmp[1]) != NULL ){ //have command_sensor char?
            if( isNumeric( valuetmp ) ){ //contain ready formatted number?
              intvalue = valuetmp.substring( 2, valuetmp.length()-1 ).toInt();
              switch( valuetmp[1] ){
                case 's': //save last speed
                    sensor_speed = intvalue;
                    taked_sensor_speed = true;
                    serial_flow_started = true;
                    break;
                case 'p': //save last steps
                    sensor_steps = intvalue;
                    taked_sensor_steps = true;
                    break;
                default: break;
              }
            }
          } else {
              if( strchr(commands_joystick, valuetmp[1]) != NULL ){ //have command_joystick char?
                    if( isNumeric( valuetmp ) ){ //contain ready formatted number?
                      intvalue = valuetmp.substring( 2, valuetmp.length()-1 ).toInt();
                      switch( valuetmp[1] ){
                        case 'C': j->cX        = intvalue; jt->cX        = true; break;
                        case 'c': j->cY        = intvalue; jt->cY        = true; break;
                        case 'N': j->mX        = intvalue; jt->mX        = true; break;
                        case 'n': j->mY        = intvalue; jt->mY        = true; break;
                        case 'M': j->MX        = intvalue; jt->MX        = true; break;
                        case 'm': j->MY        = intvalue; jt->MY        = true; break;
                        case 'D': j->dirX      = intvalue; jt->dirX      = true; break;
                        case 'd': j->dirY      = intvalue; jt->dirY      = true; break;
                        case 'R': j->dirXY     = intvalue; jt->dirXY     = true; break;
                        case 'X': j->centeredX = intvalue; jt->centeredX = true; break;
                        case 'x': j->centeredY = intvalue; jt->centeredY = true; break;
                        case 'A': j->CX        = intvalue; jt->CX        = true; break;
                        case 'B': j->CY        = intvalue; jt->CY        = true; break;
                        default: break;
                      }
                    }else{
                      valuetmp[cnt_char-1] = '\0';
                      switch( (CMD_WORD)get_cmd_word( word ) ){
                        case READY  : Serial.println("ready!");break;
                        case CALM   : Serial.println("calibrate motors!");break;
                        case STOP   : Serial.println("stop motors!");break;
                        case HEART  : Serial.println("heartbeat!");break;
                        default: break;
                      }
                    }
                  }
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
int TakeValuesSerial::get_sensor_speed(){
  return sensor_speed;
}
unsigned long TakeValuesSerial::get_sensor_steps(){
  return sensor_steps;
}
bool TakeValuesSerial::have_taked_sensor_speed(){
  bool last_status = taked_sensor_speed;
  taked_sensor_speed = false;
  return last_status;
}
bool TakeValuesSerial::have_taked_sensor_steps(){
  bool last_status = taked_sensor_steps;
  taked_sensor_steps = false;
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
int TakeValuesSerial::get_cmd_word(char *w){
  for(int i=0;i<cmd_words_num;i++)
  if( strcmp(w, cmd_words[i]) == 0 ) //same?
    return i;
  return -1;
}
/***************************************************/
/*joystick*/
/***************************************************/
int TakeValuesSerial::get_joy(int c){
  JOYMOVE *j = &joymove;
  switch( (JOY_STRUCT_MEMBER) c ){
    case cX:        return j->cX;
    case cY:        return j->cY;
    case mX:        return j->mX;
    case mY:        return j->mY;
    case MX:        return j->MX;
    case MY:        return j->MY;
    case dirX:      return j->dirX;
    case dirY:      return j->dirY;
    case dirXY:     return j->dirXY;
    case centeredX: return j->centeredX;
    case centeredY: return j->centeredY;
    case CX:        return j->CX;
    case CY:        return j->CY;
  }
}
bool TakeValuesSerial::have_taked_joy(int c){
  bool last_status;
  JOYMOVE_TAKED *jt = &joymove_taked;
  switch( (JOY_STRUCT_MEMBER) c ){
    case cX: last_status        = jt->cX; jt->cX                = false; return last_status;
    case cY: last_status        = jt->cY; jt->cY                = false; return last_status;
    case mX: last_status        = jt->mX; jt->mX                = false; return last_status;
    case mY: last_status        = jt->mY; jt->mY                = false; return last_status;
    case MX: last_status        = jt->MX; jt->MX                = false; return last_status;
    case MY: last_status        = jt->MY; jt->MY                = false; return last_status;
    case dirX: last_status      = jt->dirX; jt->dirX            = false; return last_status;
    case dirY: last_status      = jt->dirY; jt->dirY            = false; return last_status;
    case dirXY: last_status     = jt->dirXY; jt->dirXY          = false; return last_status;
    case centeredX: last_status = jt->centeredX; jt->centeredX  = false; return last_status;
    case centeredY: last_status = jt->centeredY; jt->centeredY  = false; return last_status;
    case CX: last_status        = jt->CX; jt->CX                = false; return last_status;
    case CY: last_status        = jt->CY; jt->CY                = false; return last_status;
    default: break;
  }
  taked_sensor_steps = false;
  return last_status;
}
/**************************************************/
