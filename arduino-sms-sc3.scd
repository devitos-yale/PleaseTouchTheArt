/*
 _______  ______    ______   __   __  ___   __    _  _______
|   _   ||    _ |  |      | |  | |  ||   | |  |  | ||       |
|  |_|  ||   | ||  |  _    ||  | |  ||   | |   |_| ||   _   |
|       ||   |_||_ | | |   ||  |_|  ||   | |       ||  | |  |
|       ||    __  || |_|   ||       ||   | |  _    ||  |_|  |
|   _   ||   |  | ||       ||       ||   | | | |   ||       |
|__| |__||___|  |_||______| |_______||___| |_|  |__||_______|
 _______  _______  __    _  _______  _______  ______
|       ||       ||  |  | ||       ||       ||    _ |
|  _____||    ___||   |_| ||  _____||   _   ||   | ||
| |_____ |   |___ |       || |_____ |  | |  ||   |_||_
|_____  ||    ___||  _    ||_____  ||  |_|  ||    __  |
 _____| ||   |___ | | |   | _____| ||       ||   |  | |
|_______||_______||_|  |__||_______||_______||___|  |_|
 _______  ______    _______  __    _  __   __  ___      _______  _______  _______  ______
|       ||    _ |  |   _   ||  |  | ||  | |  ||   |    |   _   ||       ||       ||    _ |
|    ___||   | ||  |  |_|  ||   |_| ||  | |  ||   |    |  |_|  ||_     _||   _   ||   | ||
|   | __ |   |_||_ |       ||       ||  |_|  ||   |    |       |  |   |  |  | |  ||   |_||_
|   ||  ||    __  ||       ||  _    ||       ||   |___ |       |  |   |  |  |_|  ||    __  |
|   |_| ||   |  | ||   _   || | |   ||       ||       ||   _   |  |   |  |       ||   |  | |
|_______||___|  |_||__| |__||_|  |__||_______||_______||__| |__|  |___|  |_______||___|  |_|


Arduino Sensor Granulator Synth (Granular Synthesis)

v042822

________________________________________________________________

To work with an Arduino you must do *at least* the following:

1) have Arduino IDE installed
  a) have required libraries installed (SimpleMessageSystem)
  b) have the correct board, processor, and port selected
  c) create and upload valid code to the board
2) (Assumes the above) have a working circuit*
3) have SC3 installed and working
  a) have the arduino Quark installed
  b) have valid SC3 code to connect to the Arduino
________________________________________________________________

*Circuit Hookup Guide:

Please reference the visual parts list in your Miuzei kits to locate the following parts.


1) Potentiometer (B10k): Left, Middle and Right pins are named looking at the pot from the front

  Left Pin (Row 5, Column J) --> Jumper --> GND (- ground)

  Middle Pin (Row 3, Column J) --> Jumper --> A0 (arduino analog pin 0)

  Right Pin (Row 1, Column J ) --> Jumper --> +Vcc

2) Photoresistor:

  One leg --> +Vcc

  One leg  (Row 20, Column A)

  Jumper (Row 20, Column E) --> A2 ( arduino analog pin 2)

  1k Ohm Resistor (Row 20, Column C) --> GND (- ground)

3) Rangefinder (Ultrasonic)

  Gnd (Row 30, Column J) --> Jumper --> GND

  Echo (Row 31, Column J) --> Jumper --> ~3 (digital pin ~3)

  Trig (Row 32, Column J) --> Jumper --> 2 (digital pin 2)

  Vcc (Row 33, Column J) --> Jumper --> +Vcc

________________________________________________________________

*/

/* ________________________________________________________________

IMPORTANT: The audio file used below (if not voicetest.wav) *must* be mono and, to get expected results, must have the same sample rate at which SC3 is running on your system. */

b = Buffer.read(s, "partita.wav".resolveRelative);

b.play;

/*

GrainBuf is a buffer granulating Ugen -- it divides up the audio in the buffer into tiny chunks called grains.  The length of the chunk or grain is determined by 'dur' below.  You can then change the speed at which the buffer is read, changing the pitch as well.  You can play with pan and the trigger at which grains are generated as well.  The arguments are here:

GrainBuf.ar(numChannels: 1, trigger: 0, dur: 1, sndbuf, rate: 1, pos: 0, interp: 2, pan: 0, envbufnum: -1, maxGrains: 512, mul: 1, add: 0)

You will need to decide what parameters to assign your sensors to in the SynthDef.  The p.action code below will need to be filled out to assign the incoming values from the sensors to buses.  You can then use those buses in your SynthDef.

________________________________________________________________
*/


// ________________________________________________________________

// 1. Create Control Buses: three buses to which to write your incoming sensor data
(
~durBus = Bus.control(s,1);
~rateBus = Bus.control(s,1);
~posBus = Bus.control(s,1);
)

// ________________________________________________________________

// 2. Write a GrainBuf SynthDef

(// Use ~busN.kr in any parameters of GrainBuf below

SynthDef(\bufGrain, { |out=0, gate = 1, amp = 1, sndbuf, envbuf|
    var pan, trigger, dur, rate, pos, grain;

	pan = 0;  // range is -1 to 1
	trigger = Impulse.kr(15) * (~durBus.kr() + 0.66); // anything, but over 50 gets crazed
	dur = ~durBus.kr(); // anything 0.001 to 0.25 should work
	rate = ~rateBus.kr(); // MouseY.kr(0.01,4); // < 0 = sound played backwards.
	pos = ~posBus.kr(); // MouseX.kr(0, 1); // try swapping out with pos = ~bus1 and make the linlin range output 0-1.
	grain = GrainBuf.ar(2, trigger, dur, sndbuf, rate, pos, 2, pan, envbuf);

	Out.ar(out, FreeVerb.ar(grain, 0.4, 0.8));

}).add;
)

/* ________________________________________________________________

3. Connect to your Arduino

Once you have your circuit working and your code uploaded and running
on the Arduino, use the following code to connect to your Arduino. Be
sure to substitute your path "/dev..." with the appropriate one
for your system.

________________________________________________________________
*/


SerialPort.devices; // evaluate to find your device path
~arduino = ArduinoSMS("/dev/tty.usbmodem144201", 115200); // replace string with your path from line above

// Scott's Linux Box Serial Path:
~arduino = ArduinoSMS("/dev/ttyUSB0", 115200);


/* ________________________________________________________________

4. CALIBRATION:

Run the below and make sure you get minimum and maximum values for each sensor. Will be used for linear scaling below to make sure you get the values you need for your Synth.

________________________________________________________________
*/

(
~dLo = 1000;
~dHi = 0;
~rLo = 1000;
~rHi = 0;
~pLo = 1000;
~pHi = 0;

~arduino.action = {|... msg|

msg.postln;

	if (msg[0] < ~dLo) { ~dLo = msg[0]; ("~dLo = " + ~dLo).postln };
	if (msg[0] > ~dHi) { ~dHi = msg[0]; ("~dHi = " + ~dHi).postln };

	if (msg[1] < ~rLo) { ~rLo = msg[1]; ("~rLo = " + ~dLo).postln };
	if (msg[1] > ~rHi) { ~rHi = msg[1]; ("~rHi = " + ~dHi).postln };

	if (msg[2] < ~pLo) { ~pLo = msg[2]; ("~pLo = " + ~dLo).postln };
	if (msg[2] > ~pHi) { ~pHi = msg[2]; ("~pHi = " + ~dHi).postln };

};
)

// stop the above:
~arduino.action = nil;

// check the values, these are the low and high for sensor at Arduino analog input 0.

~dLo.postln; ~dHi.postln;

/* ________________________________________________________________

5. Create the action that will assign values from the Arduino to the buses

________________________________________________________________ */

(
~arduino.action = {|... msg|
	var one, two, three;
	// Post the incoming raw messages
	("    INPUT ONE = "+ msg[0]).postln; // distance sensor
	("    INPUT TWO = " + msg[1]).postln; // photoresistor
    ("    INPUT THREE = " + msg[2]).postln; // potentiometer

	// write the outputs to the control buses, linearly mapping the actual low and high to the range that we want in our Synth.

	~durBus.set(msg[0].linlin(~dLo, ~dHi, 0.001, 0.3)); // Length of the grain
	~rateBus.set(msg[1].linlin(~rLo, ~rHi, 0.01, 3.0)); // rate at which to produce grains
	~posBus.set(msg[2].linlin(~pLo, ~pHi, 0.0, 1.0)); // position in the sound file to pull grains from

};
)

/*________________________________________________________________

6. Instantiate your synth.  It will pull values from the buses you define in the SynthDef above that will change dynamically as a result of the ~arduino.action function above.

________________________________________________________________
*/

x = Synth(\bufGrain, [\sndbuf, b, \envbuf, -1]);

// WHEN DONE
x.free;
~arduino.action = nil; // or
~arduino.close;
