/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include <PID_v1.h>

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, REVERSE);

void setup()
{
  //initialize the variables we're linked to
  pinMode(A0, INPUT);
  digitalWrite(A0, HIGH);
  
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);

  pinMode(A2, INPUT);
  digitalWrite(A2, HIGH);
  
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);

  
  Input = analogRead(A0);
  Setpoint = analogRead(A2);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  Serial.begin(57600);
}

void loop()
{
  Setpoint = analogRead(A2);  
  Input = analogRead(A0);
  myPID.Compute();
  analogWrite(3,Output);

  Serial.print(Setpoint);
  Serial.print("\t");
  Serial.print(Input);
  Serial.print("\t");
  Serial.println(Output);
}


