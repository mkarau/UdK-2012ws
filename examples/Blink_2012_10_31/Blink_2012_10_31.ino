/*
  Modified 2012-10-31 by Matt Karau
  
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

const int numberOfLEDs = 3;
int ledPin[numberOfLEDs];
int blinkMillis = 500;
long now = 0;
long lastBlinkMillis = 0;
boolean ledOn = false;


void setup() {
  ledPin[0] = 13;
  ledPin[1] = 12;
  ledPin[2] = 11;
  // initialize the digital pin as an output.
  // Pin 12 has an LED connected on most Arduino boards:
  for (int i=0; i<numberOfLEDs; i++) {
    pinMode(ledPin[i], OUTPUT);     
  }
}

void loop() {
  now = millis();
  
  ///  Do other things!
  
  
  if ((now - lastBlinkMillis) >= blinkMillis) {
    lastBlinkMillis = now;
    ledOn = !ledOn;
    for (int i=0; i<numberOfLEDs; i++) {
      if (ledOn) {
        digitalWrite(ledPin[i], HIGH);   // set the LED on
      } else {
        digitalWrite(ledPin[i], LOW);   // set the LED on
      }
    }
      
  }
/*  
  for (int i=0; i<numberOfLEDs; i++) {
    digitalWrite(ledPin[i], HIGH);   // set the LED on
  } 
  delay(blinkMillis);              // wait for a second
  for (int i=0; i<numberOfLEDs; i++) {
    digitalWrite(ledPin[i], LOW);    // set the LED off
  }
  delay(blinkMillis);              // wait for a second  }
*/
}
