/* 2012-11-02
  Matt Karau (mkarau@gmail.com)
  Light_Sensor_E18_D80NK.ino
  
  Uses an IR light beam sensor to detect the moment
  someone enters [or exits] the beam.  When that event
  occurs, it sends a serial message "Jump"

*/


int lightBeamSensorPin = A5;    // Pin we use for the sensor
boolean lightBeamSensorActivated = false;    // Whether someone is in the beam
boolean lastTimeInTheBeam = false;    // Whether someone WAS in the beam
boolean pullupEnabledOnLightBeamSensorPin = true;    // If we need to use a pull-up resistor
boolean debugPrint = true;    // Should we print our self-help messages
long holdOffMillis = 500;    // millis to wait between jumps
long lastJumpMillis = 0;    // mills that the last jump happened


void setup() {

  // Configure the pin used for the light sensor as INPUT 
  pinMode(lightBeamSensorPin, INPUT);
  // If we need to use the internal pull-up resistor...
  if (pullupEnabledOnLightBeamSensorPin) {
    // ...then we enable the pull-up resistor.
    digitalWrite(lightBeamSensorPin, HIGH);
  }
  
  // Start Serial commuications
  Serial.begin(57600);
}

// getLightBeamValue returns the state of the light beam connected
// to pin.  true=in-beam.  false=out-of-beam.
boolean getLightBeamValue (int pin) {
  int _state = digitalRead(pin);
  if (_state == LOW) {
    return true;
  } else {
    return false;
  }
}

void loop () {
  // Keep track of the last state so we can compare to the new state.
  lastTimeInTheBeam = lightBeamSensorActivated;
  
  // Read the current state of our sensor.
  lightBeamSensorActivated = getLightBeamValue(lightBeamSensorPin);
  
  // Only check for new jumps if holdOffMillis has elapsed since the
  // last jump.
  if ((millis() - lastJumpMillis) > holdOffMillis) {

    // If someone was previously in the beam but is now out of the beam,
    // then we trigger a photo.  Obviously, this could be easily inverted,
    // if you wished for the moment of entering the beam to be the trigger.
    if ((!lastTimeInTheBeam) && (lightBeamSensorActivated)) {

      // mark the time of the jump.
      lastJumpMillis = millis();

      // Print the command that Processing is looking for 
      Serial.print("Jump\r\n");
    }  
  }
}
