#include "TimerOne.h"
 
// The system timer interval in microseconds
long TimerOneMicros = 500;

// For keeping track of when to update the output
long outputUpdateTimer = 0;
long outputUpdateIntervalMicros = 4000;

// For keeping track of when to blink the LED
long LEDBlinkTimer = 0;
long LEDBlinkIntervalMicros = 500000;

// For keeping track of when to print
long PrintTimer = 0;
long PrintIntervalMicros = 250000;

// For keeping track of when to sense the temperature
long tempSensingTimer = 0;
long tempSensingIntervalMicros = 10000;

// For keeping track of when to calculate the average
long tempSensingAvgTimer = 0;
long TempSensingAvgIntervalMicros = 10000;


// Should we print data to the computer
// Note, sometimes printing data can slow down our
// system too much to make it seem "real-time", so
// this flag can help us to quickly switch from not
// printing to printing without changing any code.
boolean debugPrint = true;

// For calculating averages of temperature readings
float numberOfSampleInTemeratureAverage = 50.0f;
float weightOfOldAverage = 0.0f;
float weightOfNewSample = 0.0f;
float newTemperatureValue = 0.0f;

// For storing Averages of input and output temperatures
float temperatureAverage = 0.0f;
float outputTemperatureAverage = 0.0f;

// Details about the temperature sensor connected to the heating element
int outputTempSensePin = A3;
int outputVirtualGroundPin = A2;
int outputTemperature = 0;

// Details about the temperature sensor that senses the environment
int tempSensePin = A1;
int virtualGroundPin = A0;
int temperature = 0;

// A boolean that acts as a safety mechanism to detect if a sensor
// in the feedback loop is disconnected.  When one is disconnected,
// the output is disabled.
boolean sensorConnected = true;

// Details about the pin that drives heating element
int heaterTransistorGatePin = 3;
int heaterOutputLevel = 0;
float heaterOutputLevelFloat = 0.0f;

// Our LED pin
int ledPin = 13;




void setup() 
{
  /* Pins 5 and 6: controlled by Timer 0

  Setting 	Divisor 	Frequency
  0x01 	 	1 	 	62500
  0x02  	8 	 	7812.5
  0x03  	64 	 	976.5625
  0x04 	 	256 	 	244.140625
  0x05 	 	1024 	 	61.03515625

  TCCR0B = TCCR0B & 0b11111000 | <setting>;
  */

  /*
  Pins 9 and 10: controlled by timer 1

  Setting 	Divisor 	Frequency
  0x01 	 	1 	 	31250
  0x02 	 	8 	 	3906.25
  0x03 		64 	 	488.28125
  0x04 		256 	 	122.0703125
  0x05 	 	1024 	 	30.517578125

TCCR1B = TCCR1B & 0b11111000 | <setting>;
  */
  /*
  Pins 11 and 3: controlled by timer 2

  Setting 	Divisor 	Frequency
  0x01 	 	1  		31250
  0x02 	 	8 	 	3906.25
  0x03  	32  		976.5625
  0x04 	 	64 	 	488.28125
  0x05 	 	128  		244.140625
  0x06  	256  		122.0703125
  0x07 	 	1024  		30.517578125
  */
  // Configure PWM with 31.250kHz on pins 3 and 11
  TCCR2B = TCCR2B & 0b11111000 | 0x02;
  
  // Configure the N-FET gate pin as an output, connected to GND at first
  // to switch off the transistor.  
  pinMode (heaterTransistorGatePin, OUTPUT);
  digitalWrite(heaterTransistorGatePin, LOW);
  
  // Pre-calculate the weights for averaging - saves us time later in the main loop.
  weightOfOldAverage = (numberOfSampleInTemeratureAverage - 1.0f) / numberOfSampleInTemeratureAverage; 
  weightOfNewSample = 1.0f / numberOfSampleInTemeratureAverage;

  // Use outputVirtualGroundPin as virtual GND for the output thermistor
  pinMode(outputVirtualGroundPin, OUTPUT);
  digitalWrite(outputVirtualGroundPin, LOW);
  
  // outputTempSensePin is our sensing pin.
  pinMode(outputTempSensePin, INPUT);
  // We need to enable the pull-up to create voltage-divider.
  digitalWrite(outputTempSensePin, HIGH);
  
  // Use virtualGroundPin as virtual GND for the environment-sensing thermistor
  pinMode(virtualGroundPin, OUTPUT);
  digitalWrite(virtualGroundPin, LOW);
  
  // tempSensePin is our sensing pin.
  pinMode(tempSensePin, INPUT);
  // We need to enable the pull-up to create voltage-divider.  
  digitalWrite(tempSensePin, HIGH);
  
  // Initialize ledPin as an output.
  // Pin 13 has an LED connected on most Arduino boards
  pinMode(ledPin, OUTPUT);    
  
  // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.initialize(TimerOneMicros); 
  // attach the service routine here that is called every "TimerOneMicros" microseconds.
  Timer1.attachInterrupt( blinky ); 
  
  Serial.begin(57600);
}
 
void loop()
{
  // Main code loop
  // Put your regular (non-ISR) logic here
  // Use updates from the Timer1 ISR to trigger actions at certain intervals.


  // Actions to update the heating element output PWM signal
  if (outputUpdateTimer >= outputUpdateIntervalMicros) {
    outputUpdateTimer = 0;
    if (temperatureAverage < outputTemperatureAverage) {
      if (heaterOutputLevelFloat < 255.0f) {
        heaterOutputLevelFloat = 89.0f + 1000.0f * (((float)abs(temperatureAverage-outputTemperatureAverage))/(float)temperatureAverage); 
//        heaterOutputLevelFloat = 1023.0f * (((float)abs(temperatureAverage-outputTemperatureAverage))/(float)temperatureAverage);
        heaterOutputLevelFloat = constrain(heaterOutputLevelFloat, 0, 255);
      }
    }
    if (temperatureAverage > outputTemperatureAverage) {
      if (heaterOutputLevelFloat > 3.0f) {
        heaterOutputLevelFloat = heaterOutputLevelFloat - 3.0f; 
      } else {
        heaterOutputLevelFloat = 0.0f;
      }
    }
  }

  // Only drive the output if the sensors are connected
  if (sensorConnected) {
    analogWrite(heaterTransistorGatePin, (int)heaterOutputLevelFloat);
  } else {
    analogWrite(heaterTransistorGatePin, 0);
  }

  // Actions to sense the temperatures
  if (tempSensingTimer >= tempSensingIntervalMicros) {
    tempSensingTimer = 0;
    temperature = analogRead(tempSensePin);
    outputTemperature = analogRead(outputTempSensePin);
    // Check to see if we have problems with the sensor connections
    if ((temperature < 15) || (temperature > 1022) || (outputTemperature < 15) || (outputTemperature > 1022)) {
      sensorConnected = false;
    } else {
      sensorConnected = true;
    }
  }

  // Actions to calculate averages
  if (tempSensingAvgTimer >= TempSensingAvgIntervalMicros) {
    tempSensingAvgTimer = 0;
    if (sensorConnected) {
      temperatureAverage = (weightOfOldAverage * temperatureAverage) + (weightOfNewSample * temperature);
      outputTemperatureAverage = (weightOfOldAverage * outputTemperatureAverage) + (weightOfNewSample * outputTemperature);
    }
    
  }

  // Actions to blink the LED
  if (LEDBlinkTimer >= LEDBlinkIntervalMicros) {
    LEDBlinkTimer = 0;
    int state = digitalRead(ledPin); 
    digitalWrite( ledPin, !state );
  }

  // Actions to print debug info to the serial port
  if (PrintTimer >= PrintIntervalMicros) {
    PrintTimer = 0;
    if (debugPrint) Serial.print("Alive.\t");
    if (debugPrint) Serial.print("Tmp.trgt: ");
    if (sensorConnected) {
      if (debugPrint) Serial.print(temperature);
      if (debugPrint) Serial.print("\tTmp.trgt.Avg: ");
      if (debugPrint) Serial.print((int)temperatureAverage);
      if (debugPrint) Serial.print("\tTmp.Peltier: ");
      if (debugPrint) Serial.print(outputTemperature);
      if (debugPrint) Serial.print("\tTmp.Peltier.Avg: ");
      if (debugPrint) Serial.print(outputTemperatureAverage);
      if (debugPrint) Serial.print("\tOutput PWM: ");
      if (debugPrint) Serial.print(heaterOutputLevelFloat);
      if (debugPrint) Serial.println(); 
    } else {
      if (debugPrint) Serial.println("Check Sensor Connection");
    }    
  }
}
 
/// ----------------------------------------------------
/// Custom Interrupt Service Routine (ISR) Timer Routine
/// ----------------------------------------------------
void blinky()
{
  // update all the sub-timers.  Each time blinky() gets
  // called, "TimerOneMicros" have elapsed.  Update all timers
  // accordingly.
  outputUpdateTimer += TimerOneMicros;
  tempSensingAvgTimer += TimerOneMicros;
  LEDBlinkTimer = LEDBlinkTimer + TimerOneMicros;
  PrintTimer += TimerOneMicros;
  tempSensingTimer += TimerOneMicros;
}
