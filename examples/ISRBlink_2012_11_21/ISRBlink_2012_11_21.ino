#include "TimerOne.h"
 
long LEDBlinkTimer = 0;
//long LEDBlinkIntervalMillis = 500;
long LEDBlinkIntervalMicros = 500000;
//long TimerOneMillis = 1;
long TimerOneMicros = 1200;
long PrintTimer = 0;
//long PrintIntervalMillis = 100;
long PrintIntervalMicros = 100000;

boolean debugPrint = true;
boolean sensorConnected = true;
int ledPin = 13;

int tempSensePin = A1;
int virtualGroundPin = A0;

int temperature = 0;
long tempSensingTimer = 0;
long tempSensingIntervalMicros = 50000;

float numberOfSampleInTemeratureAverage = 200.0f;
float temperatureAverage = 0.0f;
float weightOfOldAverage = 0.0f;
float weightOfNewSample = 0.0f;
float newTemperatureValue = 0.0f;

long tempSensingAvgTimer = 0;
long TempSensingAvgIntervalMicros = 10000;

void setup() 
{
  weightOfOldAverage = (numberOfSampleInTemeratureAverage - 1.0f) / numberOfSampleInTemeratureAverage; 
  weightOfNewSample = 1.0f / numberOfSampleInTemeratureAverage;
  
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
    if ((temperature < 15) || (temperature > 1022)) {
      sensorConnected = false;
    } else {
      sensorConnected = true;
    }
  }
  
  if (tempSensingAvgTimer >= TempSensingAvgIntervalMicros) {
    tempSensingAvgTimer = 0;
    if (sensorConnected) {
      temperatureAverage = (weightOfOldAverage * temperatureAverage) + (weightOfNewSample * temperature);
    }
    
  }
  
  if (LEDBlinkTimer >= LEDBlinkIntervalMicros) {
//    if debugPrint Serial.println(LEDBlinkTimer);
    LEDBlinkTimer = 0;
    int state = digitalRead(ledPin); 
//    delay(300);
    digitalWrite( ledPin, !state );
  }
  
  if (PrintTimer >= PrintIntervalMicros) {
    PrintTimer = 0;
    if (debugPrint) Serial.print("Alive.\t");
    if (debugPrint) Serial.print("Temp: ");
    if (sensorConnected) {
      if (debugPrint) Serial.print(temperature);
      if (debugPrint) Serial.print("\t");
      if (debugPrint) Serial.println((int)temperatureAverage);
 
    } else {
      if (debugPrint) Serial.println("Check Sensor Connection");
    }    
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
  tempSensingAvgTimer += TimerOneMicros;
  LEDBlinkTimer = LEDBlinkTimer + TimerOneMicros;
  PrintTimer += TimerOneMicros;
  tempSensingTimer += TimerOneMicros;
  // Toggle LED
}
