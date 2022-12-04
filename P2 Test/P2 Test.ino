#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN    2
#define LED_COUNT 90
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Pin for the LED
int LEDPin = 13;
// Pin to connect to your drawing
int capSensePin = 13;
int capSensePin2 = 12;
int capSensePin3 = 11;
int capSensePin4 = 10;
int capSensePin5 = 9;
int capSensePin6 = 8;
int capSensePin7 = 7;
int capSensePin8 = 6;
int capSensePin9 = 5;
int capSensePin10 = 4;
// This is how high the sensor needs to read in order
//  to trigger a touch.  You'll find this number
//  by trial and error, or you could take readings at 
//  the start of the program to dynamically calculate this.
int touchedCutoff = 75;

void setup(){
  Serial.begin(9600);
  // Set up the LED
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop(){
  // If the capacitive sensor reads above a certain threshold,
  //  turn on the LED
  if ((readCapacitivePin(capSensePin) < touchedCutoff) && (readCapacitivePin(capSensePin2) < touchedCutoff) 
  && (readCapacitivePin(capSensePin3) < touchedCutoff) && (readCapacitivePin(capSensePin4) < touchedCutoff) 
  && (readCapacitivePin(capSensePin5) < touchedCutoff) && (readCapacitivePin(capSensePin6) < touchedCutoff) 
  && (readCapacitivePin(capSensePin7) < touchedCutoff) && (readCapacitivePin(capSensePin8) < touchedCutoff) 
  && (readCapacitivePin(capSensePin9) < touchedCutoff) && (readCapacitivePin(capSensePin10) < touchedCutoff)) {
    strip.clear();
    strip.show();
  }
  
  if (readCapacitivePin(capSensePin) > touchedCutoff) {
    Serial.println("ONE");
    strip.setPixelColor(41, strip.Color(  0, 255,   0));         //  Set pixel's color (in RAM)
    strip.show();
  }

  if (readCapacitivePin(capSensePin2) > touchedCutoff) {
    Serial.println("TWO");
    strip.setPixelColor(53, strip.Color(  255, 0,   0));         //  Set pixel's color (in RAM)
    strip.show();
  }
  if (readCapacitivePin(capSensePin3) > touchedCutoff) {
    Serial.println("THREE");
    strip.setPixelColor(32, strip.Color(  0, 0,   255));         //  Set pixel's color (in RAM)
    strip.show();
  }
  if (readCapacitivePin(capSensePin4) > touchedCutoff) {
    Serial.println("FOUR");
    strip.setPixelColor(19, strip.Color(  0, 255,   0));         //  Set pixel's color (in RAM)
    strip.show();
  }
  if (readCapacitivePin(capSensePin5) > touchedCutoff) {
    Serial.println("FIVE");
    strip.setPixelColor(57, strip.Color(  0, 0,   255));         //  Set pixel's color (in RAM)
    strip.show();
  }
  if (readCapacitivePin(capSensePin6) > touchedCutoff) {
    Serial.println("SIX");
    strip.setPixelColor(13, strip.Color(  255, 0,   0));         //  Set pixel's color (in RAM)
    strip.show();
  }
  if (readCapacitivePin(capSensePin7) > touchedCutoff) {
    Serial.println("SEVEN");
    strip.setPixelColor(65, strip.Color(  0, 255,   0));         //  Set pixel's color (in RAM)
    strip.show();
  }
  if (readCapacitivePin(capSensePin8) > touchedCutoff) {
    Serial.println("EIGHT");
    strip.setPixelColor(5, strip.Color(  0, 0,   255));         //  Set pixel's color (in RAM)
    strip.show();
  }
  if (readCapacitivePin(capSensePin9) > touchedCutoff) {
    Serial.println("NINE");
    strip.setPixelColor(76, strip.Color(  255, 0,   0));         //  Set pixel's color (in RAM)
    strip.show();
  }
  if (readCapacitivePin(capSensePin10) > touchedCutoff) {
    Serial.println("TEN");
    strip.setPixelColor(85, strip.Color(  0, 255,   0));         //  Set pixel's color (in RAM)
    strip.show();
  }

  // Every 500 ms, print the value of the capacitive sensor
  //if ( (millis() % 500) == 0){
    //Serial.print("Capacitive Sensor on Pin 13 reads: ");
    //Serial.println(readCapacitivePin(capSensePin));
  //}


}

uint8_t readCapacitivePin(int pinToMeasure){
  // This is how you declare a variable which
  //  will hold the PORT, PIN, and DDR registers
  //  on an AVR
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  if ((pinToMeasure >= 0) && (pinToMeasure <= 7)){
    port = &PORTD;
    ddr = &DDRD;
    bitmask = 1 << pinToMeasure;
    pin = &PIND;
  }
  if ((pinToMeasure > 7) && (pinToMeasure <= 13)){
    port = &PORTB;
    ddr = &DDRB;
    bitmask = 1 << (pinToMeasure - 8);
    pin = &PINB;
  }
  if ((pinToMeasure > 13) && (pinToMeasure <= 19)){
    port = &PORTC;
    ddr = &DDRC;
    bitmask = 1 << (pinToMeasure - 13);
    pin = &PINC;
  }
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input WITHOUT the internal pull-up on
  *ddr &= ~(bitmask);
  // Now see how long the pin to get pulled up
  int cycles = 16000;
  for(int i = 0; i < cycles; i++){
    if (*pin & bitmask){
      cycles = i;
      break;
    }
  }
  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  
  return cycles;
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

