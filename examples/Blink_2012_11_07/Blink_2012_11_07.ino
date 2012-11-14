/*
  Modified 2012-10-31 by Matt Karau
  
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

const int numberOfLEDs = 3;
int ledPin[numberOfLEDs];
int blinkMillis = 1000;
long now = 0;
long lastBlinkMillis = 0;
boolean ledOn = false;
long lastPrintMillis = 0;
long SerialMessageIntervalMillis = 100;
int laserPin = 5;
int laserLevel = 0;
int laserLevelIncrement = 1;
int laserLevelMax = 150;
int laserLevelMin =0;
long lastLaserUpdateMillis = 0;
long LaserUpdateMillis = 3;
int temperature = 0;
int temperaturePin = A2;
long temperatureIntervalMillis = 200;
long lastTemperatureReadingMillis = 0;


void setup() {
  ledPin[0] = 13;
  ledPin[1] = 12;
  ledPin[2] = 11;
  // initialize the digital pin as an output.
  // Pin 12 has an LED connected on most Arduino boards:
  for (int i=0; i<numberOfLEDs; i++) {
    pinMode(ledPin[i], OUTPUT);     
  }
  
  pinMode(temperaturePin, INPUT);
  digitalWrite(temperaturePin, HIGH);
  
  Serial.begin(57600);
}



void loop() {
  now = millis();
  
  ///  Do other things!
  
  // Print some debugging Info
  if ((now - lastPrintMillis) > SerialMessageIntervalMillis) {
    lastPrintMillis = now;
    Serial.print("Alive: ");
    Serial.println(temperature);
  }
  
//  delayMicroseconds();
  
  // Read Analog Port
  if ((now - lastTemperatureReadingMillis) > temperatureIntervalMillis) {
    lastTemperatureReadingMillis = now;
    temperature = analogRead(temperaturePin);
  }  
  
  // Lock up the system for at least 255 * 15ms
/*
  for (int i=0; i<255; i++) {
    analogWrite(laserPin, i);
    delay(15);
  }
*/
  
  // Modify Laser PWM 
  if ((now - lastLaserUpdateMillis) > LaserUpdateMillis) {
    lastLaserUpdateMillis = now;
    laserLevel += laserLevelIncrement;
    if ((laserLevel >= laserLevelMax) || (laserLevel <= laserLevelMin)) {
      laserLevelIncrement *= -1;
    }
    analogWrite(laserPin, laserLevel);
//  delay (10);
  }
  
  // Blink LED
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
// Don't Code with delays, since they block the loop from looping.
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
