------------
Speederiny85
------------
Manage Optic Sensor via Serial delegating Attiny-85

I need to delegate an Attiny 85 to the reading of a classic infrared optical sensor (type 20 windows)<br>
to calculate the speed and steps taken during an interval of time.

This way, in my main program in the main controller I can read the values
without going out of time, basically in a kind of multitasking.

In this is the loop example show a deferred reading from 2 serial ports *simultaneously* (sequentially).

As you can see, the speed value and steps value are changed locally only if there has been an actual change
in the remote attiny. Otherwise, the speed and steps value remains the last value detected.

There is no delay, except for 2 milliseconds for each speed-update and 2 milliseconds for each steps-update.

Therefore -see update() function- in this specific case, there is a total of 8 milliseconds of delay for each loop() cycle.

<code>
#include "TakeValuesSerial.h"
<br>
SoftwareSerial tiny85_A(2, 3); // RX, TX
SoftwareSerial tiny85_B(4, 5); // RX, TX
<br>
TakeValuesSerial optic_A(7, &tiny85_A);
TakeValuesSerial optic_B(7, &tiny85_B);
<br>
char row[] = "                                                 ";
int sA=0,sB=0;
unsigned long pA=0,pB=0;
<br>
void loop()
{
  optic_A.update();
  optic_B.update();
<br>
  if( optic_A.have_taked_speed() )
    sA = optic_A.get_speed();
  if( optic_B.have_taked_speed() )
    sB = optic_B.get_speed();
<br>
  if( optic_A.have_taked_steps() )
    pA = optic_A.get_steps();
  if( optic_B.have_taked_steps() )
    pB = optic_B.get_steps();
<br>
  // |   speed A |   speed B |   steps A |   steps B |
  sprintf(row, "| %7d | %7d | %7lu | %7lu |", sA, sB, pA, pB );
  Serial.println( row );
}
</code>
<br>
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
<h3> On OpticSensor_send_speed_serial_simulation.ino<br>
You must set:
<code>
	TakeValuesSerial TVSerial(_MAX_LENGTH_OF_VALUES_, &attiny85);
</code>
And:
<code>
	SoftwareSerial mySerial(_RX_PIN_, _TX_PIN_);
</code>
</h3>
		
TakeValuesSerial.cpp<br>
TakeValuesSerial.h<br>

----------------
<b>Run on Attiny 85</b>
----------------
<h3> On OpticSensor_send_speed_serial.ino<br>
<br>
You must set:
<code>
	OpticSensor20 optic(_SENSOR_PIN_, _INTERVAL_MILLISECOND_TAKING_VALUES_);
</code>
And:
<code>
	SoftwareSerial mySerial(_RX_PIN_, _TX_PIN_);
</code>
And comment or uncomment :
<code>
	//#define LOCALSERIAL
</code>
<br>
On OpticSensor_wait_request_from_serial.ino<br>
You must set:

<code>
int maxchars=_MAX_LENGTH_OF_VALUES_;
</code>
</h3>

OpticSensor20.cpp<br>
OpticSensor20.h<br>

-----------
Common code
-----------
SoftwareSerial.cpp
SoftwareSerial.h

![Speederiny85](https://github.com/user-attachments/assets/433564c1-a1d9-42a3-a2e4-7479f0819c1b)

Note well from the scheme, with a double serial: to have a multiserial operation, it is not possible simultaneously.<br>
So for those projects type, which require simltaneity of multiserial I/O, this code is not working for.<br>
<br>
<b>But</b> this code is useful if you want to query a series of serial ports in sequence, without any delay or lag.<br>
So you can read and write sequentially on ALL SERIAL PORTS *without having to open and close them*,<br>
just listen to read and write on any port, at any time, but not at the same time, just in sequence.
<br>

![Speederiny85_dual_serial](https://github.com/user-attachments/assets/c526c86d-7368-4cb2-8f7a-ec7d0d046a9d)

