------------
Speederiny85
------------
Manage Optic Sensor via Serial delegating Attiny-85

I need to delegate an Attiny 85 to the reading of a classic infrared optical sensor (type 20 windows)<br>
to calculate the speed and steps taken during an interval of time.

This way, in my main program in the main controller I can read the values
without going out of time, basically in a kind of multitasking.

Name: OpticSensor20<br>
Purpose: control the rotation speed and steps of the path
of optoelectric sensor and enable client-server simple serial communication<br>

Operational commands:<br>
1) #s = Send via serial: the number of occurrences coming from the optical sensor during an interval of milliseconds<br>
2) #S = Receives via serial: any number and consequently sets the interval in milliseconds in the range 100-10000<br>
3) #p = Send via serial: the number of steps taken after the reset<br>
4) #P = Receives via serial: the step count reset command<br><br>

Note: Set the sensor pin and serial speed<br>
Please note: for attiny 85 set serial rate to 38400 baud<br>
<br>
Name: TakeValuesSerial<br>
Purpose: take values from "OpticSensor_send_speed_serial" that control the rotation speed and steps of the path<br>
of optoelectric sensor and enable client-server simple serial communication<br><br>

Operational commands:<br>
1) #s = will receive actual speed from sensor<br>
2) #S<num> = set interval resolution for taking sensor speed on sensor<br>
3) #p = will receive actual steps gathered from sensor<br>
4) #P = reset to ZERO gathered steps on sensor<br><br>

Note:<br>
values received are formatted so:

#s<speed>#

and 

#p<steps>#

------------------
<b>Run on Arduino/ESP</b>
------------------
OpticSensor_send_speed_serial_simulation.ino
You must set:
	TakeValuesSerial TVSerial(<MAX_LENGTH_OF_VALUES>, &attiny85);
And:
	SoftwareSerial mySerial(<RX_PIN>, <TX_PIN>);

TakeValuesSerial.cpp
TakeValuesSerial.h
------------------

----------------
<b>Run on Attiny 85</b>
----------------
OpticSensor_send_speed_serial.ino
You must set:
	OpticSensor20 optic(<SENSOR_PIN>, <INTERVAL_MILLISECOND_TAKING_VALUES>);
And:
	SoftwareSerial mySerial(<RX_PIN>, <TX_PIN>);
And comment or uncomment :
	//#define LOCALSERIAL

OpticSensor_wait_request_from_serial.ino
You must set:
int maxchars=<MAX_LENGTH_OF_VALUES>;

OpticSensor20.cpp
OpticSensor20.h
----------------

-----------
Common code
-----------
SoftwareSerial.cpp
SoftwareSerial.h
-----------
![Speederiny85](https://github.com/user-attachments/assets/433564c1-a1d9-42a3-a2e4-7479f0819c1b)


