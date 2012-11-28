#include "TimerOne.h"
 
// The system timer interval in microseconds
long TimerOneMicros = 500;

// For keeping track of when to update the output
long outputUpdateTimer = 0;
long outputUpdateIntervalMicros = 5000;

// For keeping track of when to blink the LED
long LEDBlinkTimer = 0;
long LEDBlinkIntervalMicros = 500000;

// For keeping track of when to print
long PrintTimer = 0;
long PrintIntervalMicros = 250000;

// For keeping track of when to calculate the Pulses Per Second
long pulsesPerSecondTimer = 0;
long pulsesPerSecondIntervalMicros = 100000;

// For keeping track of when to calculate the average Pulses Per Second
long pulsesPerSecondAvgTimer = 0;
long pulsesPerSecondAvgIntervalMicros = 20000;

// Should we print data to the computer
// Note, sometimes printing data can slow down our
// system too much to make it seem "real-time", so
// this flag can help us to quickly switch from not
// printing to printing without changing any code.
boolean debugPrint = true;

// For calculating averages of temperature readings
float numberOfSampleInAverage = 5.0f;
float weightOfOldAverage = 0.0f;
float weightOfNewSample = 0.0f;
float newTemperatureValue = 0.0f;

// For storing Averages of input and output temperatures
volatile long pulses = 0;
volatile float pulsesPerSecond = 0.0f;
volatile float pulsesPerSecondAverage = 0.0f;

float targetPulsesPerSecond = 1000.0f;

// Details about the pin that drives heating element
int motorTransistorGatePin = 3;
int motorOutputLevel = 0;
float motorOutputLevelFloat = 0.0f;

// Our LED pin
int ledPin = 13;




void setup() 
{
  
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
  // Configure PWM with 976.5625Hz on pins 3 and 11
  TCCR2B = TCCR2B & 0b11111000 | 0x03;
  
  // Configure the N-FET gate pin as an output, connected to GND at first
  // to switch off the transistor.  
  pinMode (motorTransistorGatePin, OUTPUT);
  digitalWrite(motorTransistorGatePin, LOW);
  
  // Pre-calculate the weights for averaging - saves us time later in the main loop.
  weightOfOldAverage = (numberOfSampleInAverage - 1.0f) / numberOfSampleInAverage; 
  weightOfNewSample = 1.0f / numberOfSampleInAverage;

  // Initialize ledPin as an output.
  // Pin 13 has an LED connected on most Arduino boards
  pinMode(ledPin, OUTPUT);    
  
  // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.initialize(TimerOneMicros); 
  
  // attach the service routine here that is called every "TimerOneMicros" microseconds.
  Timer1.attachInterrupt( blinky ); 
  
  // attach a hardware interrupt to call "count()" on the rising edge
  // of a signal on Pin D2 (Interrupt 0)
  attachInterrupt(0, count, RISING);

  Serial.begin(57600);
}
 

void count() {
  pulses += 1;
}

void loop()
{
  // Main code loop
  // Put your regular (non-ISR) logic here
  // Use updates from the Timer1 ISR to trigger actions at certain intervals.


  // Actions to update the heating element output PWM signal
  if (outputUpdateTimer >= outputUpdateIntervalMicros) {
    outputUpdateTimer = 0;
    if (pulsesPerSecondAverage < targetPulsesPerSecond) {
      if (motorOutputLevelFloat < 255.0f) {
        motorOutputLevelFloat += 1.0f;
      }
    }
    if (pulsesPerSecondAverage > targetPulsesPerSecond) {
      if (motorOutputLevelFloat > 1.0f) {
        motorOutputLevelFloat -= 1.0f; 
      } else {
        motorOutputLevelFloat = 0.0f;
      }
    }
  }

  analogWrite(motorTransistorGatePin, (int)motorOutputLevelFloat);


  // Actions to sense the Pulses Per Second
  if (pulsesPerSecondTimer >= pulsesPerSecondIntervalMicros) {
    pulsesPerSecondTimer = 0;   
    pulsesPerSecond = (((float)pulses) / ((float)pulsesPerSecondIntervalMicros)) * (1000000.0f /* us per second */);
    pulses = 0;
  }

  // Actions to calculate averages
  if (pulsesPerSecondAvgTimer >= pulsesPerSecondAvgIntervalMicros) {
    pulsesPerSecondAvgTimer = 0;
    pulsesPerSecondAverage = (weightOfOldAverage * pulsesPerSecondAverage) + (weightOfNewSample * pulsesPerSecond);
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
    if (debugPrint) Serial.print("PPS.trgt: ");
    if (debugPrint) Serial.print((int)targetPulsesPerSecond);
    if (debugPrint) Serial.print("\tPPS.actual: ");
    if (debugPrint) Serial.print((int)pulsesPerSecond);
    if (debugPrint) Serial.print("\tPPS.Avg: ");
    if (debugPrint) Serial.print((int)pulsesPerSecondAverage);
    if (debugPrint) Serial.print("\tOutput PWM: ");
    if (debugPrint) Serial.print((int)motorOutputLevelFloat);
    if (debugPrint) Serial.println(); 
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
  pulsesPerSecondAvgTimer += TimerOneMicros;
  LEDBlinkTimer = LEDBlinkTimer + TimerOneMicros;
  PrintTimer += TimerOneMicros;
  pulsesPerSecondTimer += TimerOneMicros;
}
