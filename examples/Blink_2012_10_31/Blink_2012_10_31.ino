/*
  Modified 2012-10-31 by Matt Karau
  
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

int ledPin;



void setup() {    
  ledPin = 13;
  // initialize the digital pin as an output.
  // Pin 12 has an LED connected on most Arduino boards:
  pinMode(ledPin, OUTPUT);     
}

void loop() {
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(1000);              // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(1000);              // wait for a second

}
