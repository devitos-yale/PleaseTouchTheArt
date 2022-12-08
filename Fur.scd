// evaluate to find the name of your arduino
SerialPort.listDevices;
// see post window

// make sure serial monitor is not open in arduino IDE
// then open port
(
~port = SerialPort(
	"/dev/cu.usbmodem141101", // your arduino's name
	baudrate: 9600, // must match arduino rate
	crtscts: true
);
)

// a loop for reading from arduino
(
~routine = Routine {
	var byte, str, val;
	inf.do { |i|
		if(~port.read == Char.nl.asInteger, {
			str = "";
			while(
				{ byte = ~port.read; byte != Char.ret.asInteger },
				{ str = str ++ byte.asAscii }
			);
			val = str.split(Char.comma).asInteger;

			// our sound
			// triggered and controlled form arduino

			// val[0] = push button = on/off sound
			// val[1] = pot0 = freq
			// val[2] = pot1 = amp

			// val[0] = movement1
			// val[1] = p1
			// val[2] = movement2
			// val[3] = p2
			// val[4] = movement3
			// val[5] = p3
			// val[6] = b1
			// val[7] = b2
			// val[8] = b3
			// val[9] = b4

			if(val[0] == 1, {
			{ SinOsc.ar(
				val[1].linexp(0, 1023, 400, 1600), 0,
				val[3].linlin(0, 1023, -30, -16).dbamp
			) * Env.perc(0.01, 0.1).kr(2) }.play;
			});

			if(val[2] == 1, {
			{ SinOsc.ar(
				val[3].linexp(0, 1023, 400, 1600), 0,
				val[5].linlin(0, 1023, -30, -16).dbamp
			) * Env.perc(0.01, 0.1).kr(2) }.play;
			});

			if(val[4] == 1, {
			{ SinOsc.ar(
				val[5].linexp(0, 1023, 400, 1600), 0,
				val[1].linlin(0, 1023, -30, -16).dbamp
			) * Env.perc(0.01, 0.1).kr(2) }.play;
			});
		});
	};
}.play;
)

// this will kick off reading (handshaking with arduino)
~port.put(0);

// stop
~routine.stop; ~port.close;