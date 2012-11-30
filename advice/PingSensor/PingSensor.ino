/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
long startOfPingMillis = 0;
long endOfPingMillis = 0;
long lengthOfPingMillis = 0;
volatile boolean noResponse = true;
boolean debugPrint = false;
float speedOfSoundMetersPerSecond = 340.29f;
float distance = 0.0f;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);   

  // Pin Digital_2  
  attachInterrupt(1, stopTiming, RISING);
  
  Serial.begin(57600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
}

// the loop routine runs over and over again forever:
void loop() {
  
  if (debugPrint) Serial.println("Sending Ping");
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  startOfPingMillis = millis();
  noResponse = true;
  if (debugPrint) Serial.println("Waiting for Response");
  while (noResponse) {}
  if (debugPrint) Serial.println("Received Response");
  // calculate distance
  lengthOfPingMillis = abs(startOfPingMillis - endOfPingMillis);
  distance = speedOfSoundMetersPerSecond * ( ((float)lengthOfPingMillis) / 1000.0f);
  Serial.print("Distance (m): ");
  Serial.println(distance);
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);               // wait for a second
}


void stopTiming() {
  noResponse = false;
  endOfPingMillis = millis();
}
