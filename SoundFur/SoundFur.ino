// variables will change:
int buttonState1 = 0;  // variable for reading the pushbutton status
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int movement1 = 0;
int movement2 = 0;
int movement3 = 0;
int p1a = 0;
int p1b = 0;
int p1c = 0;
int p2a = 0;
int p2b = 0;
int p2c = 0;
int p3a = 0;
int p3b = 0;
int p3c = 0;

unsigned char val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  pinMode(11, OUTPUT);
  waitForSuperCollider();
}

void loop() {
  // read the state of the pushbutton value:

  buttonState1 = digitalRead(2);
  buttonState2 = digitalRead(4);
  buttonState3 = digitalRead(5);
  buttonState4 = digitalRead(16);

  int p1 = analogRead(33); //P1 sensor
  delay(10); // delay in between analog reads for stability
  int p2 = analogRead(32); //P2 sensor
  delay(10);
  int p3 = analogRead(35); //P3 sensor

  //if one of the photoresistors has been triggered, movement is true
  if (p1 != p1c)
    movement1 = 1;
  else 
    movement1 = 0;
  if (p2 != p2c)
    movement2 = 1;
  else 
    movement2 = 0;
  if (p3 != p3c)
    movement3 = 1;
  else 
    movement3 = 0;

  //store last 3 photoresistor numbers
  p1c = p1b;
  p1b = p1a;
  p1a = p1;
  p2c = p2b;
  p2b = p2a;
  p2a = p2;
  p3c = p3b;
  p3b = p3a;
  p3a = p3;

  if (Serial.available() > 0) {
    Serial.read();
    Serial.print(movement1);
    Serial.print(",");
    Serial.print(p1);
    Serial.print(",");
    Serial.print(movement2);
    Serial.print(",");
    Serial.print(p2);
    Serial.print(",");
    Serial.print(movement3);
    Serial.print(",");
    Serial.print(p3);
    Serial.print(",");
    Serial.print(buttonState1);
    Serial.print(",");
    Serial.print(buttonState2);
    Serial.print(",");
    Serial.print(buttonState3);
    Serial.print(",");
    Serial.println(buttonState4); //end of data line
  }

/*
  Serial.println(p1);
  Serial.println(p2);
  Serial.println(p3);

  if (buttonState1 == LOW) {
    // turn LED on:
    Serial.println("ONE");
  } 
  if (buttonState2 == LOW) {
    // turn LED on:
    Serial.println("TWO");
  } 
  if (buttonState3 == LOW) {
    // turn LED on:
    Serial.println("THREE");
  } 
  if (buttonState4 == LOW) {
    // turn LED on:
    Serial.println("FOUR");
  } 
*/
}

void waitForSuperCollider() {
  while (Serial.available() <= 0) {
    // send an initial string. must match the number of values you want to send
    Serial.println("0,0,0,0,0,0,0,0,0,0");
    delay(300);
  }
}






/*
buttons constantly sending zero (amplitude) until they are pressed, then send 1? for amp
just an action, not a routine
use busses, lag them to smooth light code
delay(50) in arduino code

sclang and add file as argument, have it in rc local
SC code gets run through a one liner added to bash rc
wrap SC doc in a wait for boot statemnet (s.waitforboot at top)
inside ASC doc, have vars and synthdef and s.sink statement, then have any code that instantiates synths
initializes seven busses to 0 (so values can be pulled at control wait)
synth.new ... under that is arduino.action which takes message input
everytime you are getting a message you are writing to the bus
reading from the busses in the synthdef
can also lag amp/buttons

FMgen quark from scott's github, auto generates random fm quarks, get rid of plonk envelope, in.linen

using SC on pi (terminal?)
nicer sound
*/

