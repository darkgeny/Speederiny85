/**************************************************************************
    Author: Giacomo Catanzariti darkgeny@gmail.com
    Name: OpticSensor
    Purpose: control the rotation speed and steps of the path
    of optoelectric sensor and enable client-server simple serial communication
    Operational commands:
    1) #s = Send via serial: the number of occurrences coming from the optical
    sensor during an interval of milliseconds
    2) #S = Receives via serial: any number and consequently sets the interval
    in milliseconds in the range 100-10000
    3) #p = Send via serial: the number of steps taken after the reset
    4) #P = Receives via serial: the step count reset command
    Note: Set the sensor pin and serial speed
    Please note: for attiny 85 set serial rate to 38400 baud
**************************************************************************/

int maxchars=7;

String read_string; //temp buffer
int cnt_char=0;
bool string_cmd_started=false;
char commands[]="SsPp"; //S=set interval, s=get speed, P=reset path, p=get path
bool got_cmd=false; //command taken
char cmd='\0'; //char command

void wait_request_from_serial(){
  char c = '\0';
  //polling from serial
  while (THESERIAL.available()) { 
    if( cnt_char > maxchars )
      reset_vars();
    c = THESERIAL.read();
    if( c == '\r' ) continue; //drop this special char
    if( c == '#' ){ //ready to read value of command, if exists
      string_cmd_started = true;
      continue;
    }
    else if( !string_cmd_started )
            continue; //drop any other char, got only command char
    if( !got_cmd ){ //having the command now?
      if( strchr(commands, c) == NULL ){
        reset_vars();
        continue; //no...
      }else{
        got_cmd = true; //yes!
        cmd=c; //save command
        continue;
      }
    }
    if( c != '\n' ){ //adding c to value
      read_string += c;
      cnt_char++;
      continue;
    }
    //then line is ready to go
    if( c == '\n' ) //having command and value, then go ahead
      exec_command(cmd, read_string );
  }
}
void reset_vars(){ //reset all
      read_string="";
      cnt_char = 0;
      string_cmd_started = false;
      got_cmd=false;
      cmd='\0';
}
void exec_command(char c, String value){
  //the client will use: Serial.readBytesUntil(), then set timeout during reading
  switch( c ){
    case 'S': if( value.length() > 0 )
                optic.set_interval_millis( value.toInt() );
              break;
    case 's': THESERIAL.print("#s");
              THESERIAL.print( optic.get_speed() );
              THESERIAL.println( "#" );
              break;
    case 'P': optic.reset_steps();
              break;
    case 'p': THESERIAL.print("#p");
              THESERIAL.print( optic.get_steps() );
              THESERIAL.println( "#" );
              break;
    default: break;
  }
  reset_vars();
}
