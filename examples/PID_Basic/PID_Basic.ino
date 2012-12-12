/********************************************************
 * PID Basic Example
 * Reading analog input 0 as the SetPoint
 * Reading analog input 2 as the realWorldTemperature
 * Control analog PWM output 3 to turn on heater
 ********************************************************/

#include <PID_v1.h>

//Define Variables we'll be connecting to
double Setpoint, realWorldTemperature, Output;

//Specify the links and initial tuning parameters
PID myPID(&realWorldTemperature, &Output, &Setpoint, 2, 5, 1, REVERSE);

void setup()
{
  
  pinMode(A0, INPUT);
  digitalWrite(A0, HIGH);
  
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);

  pinMode(A2, INPUT);
  digitalWrite(A2, HIGH);
  
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  
  //initialize the variables we're linked to
  
  Setpoint = analogRead(A0);
  realWorldTemperature = analogRead(A2);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  Serial.begin(57600);
}

void loop()
{
  Setpoint = analogRead(A0);  
  realWorldTemperature = analogRead(A2);
  myPID.Compute();
  analogWrite(3,Output);

  Serial.print(Setpoint);
  Serial.print("\t");
  Serial.print(realWorldTemperature);
  Serial.print("\t");
  Serial.println(Output);
}


