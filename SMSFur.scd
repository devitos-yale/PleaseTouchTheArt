// 1. Create Control Buses: three buses to which to write your incoming sensor data
(
~b1Bus = Bus.control(s,1);
~b2Bus = Bus.control(s,1);
~b3Bus = Bus.control(s,1);
~b4Bus = Bus.control(s,1);
~p1Bus = Bus.control(s,1);
~p2Bus = Bus.control(s,1);
~p3Bus = Bus.control(s,1);
)

// 2. Write a GrainBuf SynthDef

(// Use ~busN.kr in any parameters of GrainBuf below

SynthDef( \sin,    { | amp, freq, trig = 1 |
    var  sig;
	sig = SinOsc.ar(freq) * amp;
    Out.ar( [ 0 ], sig * 0.6 );
}).add;
)

Quarks.update("arduino");
Quarks.install("arduino");
Quarks.gui;


// 3. Connect to your Arduino

SerialPort.devices; // evaluate to find your device path
~arduino = ArduinoSMS("/dev/tty.usbserial-210", 9600);
// replace string with your path from line above

// 4. Calibration

(
~eLo = 1000;
~eHi = 0;
~fLo = 1000;
~fHi = 0;
~gLo = 1000;
~gHi = 0;

~arduino.action = {|... msg|

msg.postln;

	if (msg[4] < ~eLo) { ~eLo = msg[4]; ("~eLo = " + ~eLo).postln };
	if (msg[4] > ~eHi) { ~eHi = msg[4]; ("~eHi = " + ~eHi).postln };

	if (msg[5] < ~fLo) { ~fLo = msg[5]; ("~fLo = " + ~fLo).postln };
	if (msg[5] > ~fHi) { ~fHi = msg[5]; ("~fHi = " + ~fHi).postln };

	if (msg[6] < ~gLo) { ~gLo = msg[6]; ("~gLo = " + ~gLo).postln };
	if (msg[6] > ~gHi) { ~gHi = msg[6]; ("~gHi = " + ~gHi).postln };

};
)

// stop the above:
~arduino.action = nil;

// check the values, these are the low and high for sensor at Arduino analog input 0.

~dLo.postln; ~dHi.postln;


//5. Create the action that will assign values from the Arduino to the buses
(
~arduino.action = {|... msg|
	var one, two, three, four, five, six, seven;
	// Post the incoming raw messages
	("    INPUT ONE = "+ msg[0]).postln; // button 1
	("    INPUT TWO = " + msg[1]).postln; // button 2
    ("    INPUT THREE = " + msg[2]).postln; // button 3
	("    INPUT FOUR = "+ msg[3]).postln; // button 4
	("    INPUT FIVE = " + msg[4]).postln; // p1
    ("    INPUT SIX = " + msg[5]).postln; // p2
	("    INPUT SEVEN = "+ msg[6]).postln; // p3

	// write the outputs to the control buses, linearly mapping the actual low and high to the range that we want in our Synth.

	~b1Bus.set(msg[0]);
	~b2Bus.set(msg[1]);
	~b3Bus.set(msg[2]);
	~b4Bus.set(msg[3]);
	~p1Bus.set(msg[4].linlin(~eLo, ~eHi, 400, 1600));
	~p2Bus.set(msg[5].linlin(~fLo, ~fHi, 400, 1600));
	~p3Bus.set(msg[6].linlin(~gLo, ~gHi, 400, 1600));

};
)

//6. Instantiate your synth.  It will pull values from the buses you define in the SynthDef above that will change dynamically as a result of the ~arduino.action function above.
//x = Synth(\bufGrain, [\sndbuf, b, \envbuf, -1]);

(
w = Synth(\sin, [\amp, ~b1Bus.kr(), \freq, ~p1Bus.kr()]);
x = Synth(\sin, [\amp, ~b2Bus.kr(), \freq, ~p2Bus.kr()]);
y = Synth(\sin, [\amp, ~b3Bus.kr(), \freq, ~p3Bus.kr()]);
z = Synth(\sin, [\amp, ~b4Bus.kr(), \freq, ~p1Bus.kr()]);
)


// WHEN DONE
(
w.free;
x.free;
y.free;
z.free;
//~arduino.action = nil; // or
~arduino.close;
)
