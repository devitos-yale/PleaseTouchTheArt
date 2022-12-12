#include "SimpleMessageSystem.h" 

// variables will change:
int buttonState1 = 0;  // variable for reading the pushbutton status
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;

unsigned char val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  pinMode(11, OUTPUT);
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

  messageSendInt(!buttonState1); // add values to message sent via SMS (simple message system)
  messageSendInt(!buttonState2);
  messageSendInt(!buttonState3);
  messageSendInt(!buttonState4);
  messageSendInt(p1);
  messageSendInt(p2);
  messageSendInt(p3);
  messageEnd();
  
  //delay(val);                  // stop the program for some time
  delay(50);  

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
