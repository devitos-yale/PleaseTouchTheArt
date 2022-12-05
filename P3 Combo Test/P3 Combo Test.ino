
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

//define the pins you want to connect to the sensors and buzzers
//if you want to use more then two sensors, define them the same way as sensor 1 and 2 where defined

//pins, that are connected to "trigger" of ultrasonic sensors
#define TRIGGER1 6
#define TRIGGER2 3

//pins, that are connected to "echo" of ultrasonic sensors
#define ECHO1 5
#define ECHO2 2



//define your global variables


//define one long for every sensor you want to use and the distance it measures
long distance1=0; 
long distance2=0;

//define one long for every sensor you want to use. Using these make it possible to run the buzzers simultaneously.
long t1=-10000;
long t2=-10000;

//declare the funtions, we'll be using

//checkDistance will check if the given distance is lower then 200 cm. If it is lower then 200 cm, it plays a tone on the buzzer that is connected with the given tone object.
//The length of the tone depends on how close the object is to the sensor.
/*
  long distance - distance you want to check
  Tone toneobj - tone object you want to play the tone at, if the distance is between 200 and 0 cm
  int freqeuency - the frequency the played tone should have (we suggest to use different frequencies for each buzzer, so you can tell apart, wich sensor got triggered)
  long *timer - one of our initialised timers (t1,...,tn), that keeps control, that the sound is played as long as it should be
 */
void checkDistance(long distance, long *timer);

//measure will measure and return the distance between the sensor and an object in the way of the sensor in cm. If theres no object within two meter, it will return 0.
/*
  int trigger - trigger pin connected to the sensor, you want to check
  int echo - echo pin connected to the sensor, you want to check
 */
long measure(int trigger, int echo);

//setup the pins and connect the tone objects to the buzzers
void setup(){
  
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(11);

  pinMode(TRIGGER1, OUTPUT); 
  pinMode(ECHO1, INPUT); 
  pinMode(TRIGGER2, OUTPUT); 
  pinMode(ECHO2, INPUT); 
  Serial.begin(9600);

  
}

//constantly measure the distances and checks, if it is necessary to play a tone or not
void loop()
{
  
  distance1 = measure(TRIGGER1, ECHO1);
  //Serial.println(distance1);
  if (distance1 > 75) {
    myservo1.write(5);
    delay(150);
  }
  else if (distance1 > 70){
    myservo1.write(22);
    delay(150);
  }
  else if (distance1 > 60){
    myservo1.write(45);
    delay(150);
  }
  else if (distance1 > 50){
    myservo1.write(67);
    delay(150);
  }
  else if (distance1 > 40){
    myservo1.write(90);
    delay(150);
  }
  else if (distance1 > 30){
    myservo1.write(112);
    delay(150);
  }
  else if (distance1 > 20){
    myservo1.write(135);
    delay(150);
  }
  else if (distance1 > 10){
    myservo1.write(157);
    delay(150);
  }
  else{
    myservo1.write(175);
    delay(150);
  }
  distance2 = measure(TRIGGER2, ECHO2);
  Serial.println(distance2);
  if (distance2 > 75) {
    myservo2.write(5);
    delay(150);
  }
  else if (distance2 > 70){
    myservo2.write(22);
    delay(150);
  }
  else if (distance2 > 60){
    myservo2.write(45);
    delay(150);
  }
  else if (distance2 > 50){
    myservo2.write(67);
    delay(150);
  }
  else if (distance2 > 40){
    myservo2.write(90);
    delay(150);
  }
  else if (distance2 > 30){
    myservo2.write(112);
    delay(150);
  }
  else if (distance2 > 20){
    myservo2.write(135);
    delay(150);
  }
  else if (distance2 > 10){
    myservo2.write(157);
    delay(150);
  }
  else{
    myservo2.write(175);
    delay(150);
  }
  checkDistance(distance1, &t1);
  checkDistance(distance2, &t2);
  
}


long measure(int trigger, int echo){
  long duration = 0;
  digitalWrite(trigger, LOW);  
  delay(5); 
  digitalWrite(trigger, HIGH); 
  delay(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH,11662); 
  return (duration/2) * 0.03432; 
}

void checkDistance(long distance, long *timer){
  if(distance<200&&distance>0){
    if(millis()-*timer > long(5.4*distance-81)||distance<15){
      *timer=millis();
    }
    
  }
}