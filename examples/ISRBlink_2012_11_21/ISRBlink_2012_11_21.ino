#include "TimerOne.h"
 
long LEDBlinkTimer = 0;
//long LEDBlinkIntervalMillis = 500;
long LEDBlinkIntervalMicros = 500000;
//long TimerOneMillis = 1;
long TimerOneMicros = 1200;
long PrintTimer = 0;
//long PrintIntervalMillis = 100;
long PrintIntervalMicros = 100000;

int ledPin = 13;

int tempSensePin = A1;
int virtualGroundPin = A0;

int temperature = 0;
long tempSensingTimer = 0;
long tempSensingIntervalMicros = 500000;

void setup() 
{
  // Use A0 as virtual GND
  pinMode(virtualGroundPin, OUTPUT);
  digitalWrite(virtualGroundPin, LOW);
  
  // A1 is our sensing pin.  We need to have a pull-up to create voltage-divider.
  pinMode(tempSensePin, INPUT);
  digitalWrite(tempSensePin, HIGH);
  
  // Initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards
  pinMode(ledPin, OUTPUT);    
  
  Timer1.initialize(TimerOneMicros); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( blinky ); // attach the service routine here
  
  Serial.begin(57600);
}
 
void loop()
{
/*  if (stateChanged) {
    // blah
    stateChanged = false;
  }
*/  
  if (tempSensingTimer >= tempSensingIntervalMicros) {
    tempSensingTimer = 0;
    temperature = analogRead(tempSensePin);
  }
  
  if (LEDBlinkTimer >= LEDBlinkIntervalMicros) {
//    Serial.println(LEDBlinkTimer);
    LEDBlinkTimer = 0;
    int state = digitalRead(ledPin); 
//    delay(300);
    digitalWrite( ledPin, !state );
  }
  
  if (PrintTimer >= PrintIntervalMicros) {
    PrintTimer = 0;
    Serial.print("Alive.\t");
    Serial.print("Temp: ");
    Serial.println(temperature);
  }
  
  // Main code loop
  // TODO: Put your regular (non-ISR) logic here
}
 
/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void blinky()
{
//  stateChanged = true;
  LEDBlinkTimer = LEDBlinkTimer + TimerOneMicros;
  PrintTimer += TimerOneMicros;
  tempSensingTimer += TimerOneMicros;
  // Toggle LED
}
