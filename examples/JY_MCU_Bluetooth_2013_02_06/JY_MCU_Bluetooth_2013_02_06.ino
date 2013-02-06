// -------- Begin don't edit section ----------------
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// -------- End don't edit section ------------------

// Reports data out as numbers with ";" between.
// Looks for strings including only numbers separated by ;
// e.g. 123;456;789   (With new line / Carriage Return at the end)

// "incomingValue" is where your incoming numbers from the other
// side will be stored.  Initialize this with as many zeros as the
// number of data you expect to receive. "{0, 0, 0}" means we expect
// 3 numbers arriving to us.
float incomingValue[] = {0, 0, 0};

// Necessary variables for dealing with serial input.
// -------- Begin don't edit section ----------------
const int incomingBufferLength = 60;
char serialInputBuffer[incomingBufferLength+1];
int numberOfIncomingValues = sizeof(incomingValue)/sizeof(float);
int serialIndex = 0;
int incomingByte;
// -------- End don't edit section ------------------

// Keep track of timing so we don't flood the other side with data.
unsigned long millisOfMyLastDataReport;
unsigned long millisBetweenMyDataReports = 10;

int myAnalogSensorPin = A0;
int myAnalogSensorValue;

int ledToRepresentOtherSidePin = 11;
int ledToRepresentOtherSideValue = 0;


void setup() {
  pinMode(myAnalogSensorPin, INPUT_PULLUP);
  Serial.begin(57600);
  Serial.println("Ready.");
}


void loop() {
  watchForIncomingSerial();
  readSensors();
  driveLED();

  if (millis() - millisOfMyLastDataReport > millisBetweenMyDataReports) {
    millisOfMyLastDataReport = millis();
    reportMyData();
  }  
}

// _DO_ CHANGE "readSensors" to sense things locally.
void readSensors() {
  myAnalogSensorValue = analogRead(myAnalogSensorPin);
}

// _DO_ CHANGE "driveLED" to take actions based on the data from the other side.
void driveLED() {
  analogWrite(ledToRepresentOtherSidePin, (int)incomingValue[0]);
}

// _DO_ CHANGE "reportMyData" to print your events/data with ";" in between.
// You need to print ONLY NUMBERS.  The data CANNOT be text.
void reportMyData() {
  Serial.print(myAnalogSensorValue);
  Serial.print(";");
  Serial.print(myAnalogSensorValue/2);
  Serial.print(";");
  Serial.println(myAnalogSensorValue/4);
  Serial.flush();
}



// DON'T CHANGE "assignValues" --- all changes should be made in the top 
// variable declarations.

void assignValues (char *buf) {
  char *p = buf;
  char *str;
  int index = 0;
  float tempIncoming[numberOfIncomingValues];
  while ((str = strtok_r(p, ";", &p)) != NULL) {
    if ((str[0] >= '0') && (str[0] <= '9')) {
      tempIncoming[index] = atof (str);
      index++;
    }
  }
  if (index == numberOfIncomingValues) {
    for (int i=0; i<numberOfIncomingValues; i++) {
      incomingValue[i] = tempIncoming[i];
    }
  }
}



// DON'T CHANGE "watchForIncomingSerial" --- all changes should be made in the top 
// variable declarations.

void watchForIncomingSerial(){
  if (Serial.available() ) {
    incomingByte = Serial.read();
    if ((incomingByte == 10) || (incomingByte == 13)) {
      if ((serialIndex > 0) && (serialIndex < incomingBufferLength)){
        serialInputBuffer[serialIndex] = '\0';
//        Serial.println(serialInputBuffer);
        assignValues(serialInputBuffer);
//        for (int i=0; i<numberOfIncomingValues; i++) {
//          Serial.println(incomingValue[i]);
//        }
        strcpy(serialInputBuffer, "");
        serialIndex = 0;
        Serial.flush();
      } else {
        strcpy(serialInputBuffer, "");
        serialIndex = 0;
        Serial.flush();
      }
    } 
    else {
      if (serialIndex < incomingBufferLength) {
        serialInputBuffer[serialIndex] = incomingByte;
        serialIndex++;
      } else {
        strcpy(serialInputBuffer, "");
        serialIndex = 0;
        Serial.flush();
      }
    }
  }
}

