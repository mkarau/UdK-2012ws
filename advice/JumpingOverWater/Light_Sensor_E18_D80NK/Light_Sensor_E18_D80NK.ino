int lightBeamSensorPin = A5;
boolean lightBeamSensorActivated = false;
boolean lastTimeInTheBeam = false;
boolean pullupEnabledOnLightBeamSensorPin = true;
boolean debugPrint = true;
long holdOffMillis = 500;
long lastJumpMillis = 0;

void setup() {
  pinMode(lightBeamSensorPin, INPUT);
  if (pullupEnabledOnLightBeamSensorPin) {
    digitalWrite(lightBeamSensorPin, HIGH);
  }
  Serial.begin(57600);
}

boolean getLightBeamValue (int pin) {
  int _state = digitalRead(pin);
  if (_state == LOW) {
    return true;
  } else {
    return false;
  }
}

void loop () {
  lastTimeInTheBeam = lightBeamSensorActivated;
  lightBeamSensorActivated = getLightBeamValue(lightBeamSensorPin);
  if ((millis() - lastJumpMillis) > holdOffMillis) {
    if ((lastTimeInTheBeam) && (!lightBeamSensorActivated)) {
      lastJumpMillis = millis();
      if (debugPrint) Serial.print("Jump\n");
    }  
  }
}
