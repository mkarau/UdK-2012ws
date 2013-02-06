/*
  Example showing how to keep track of time using millis() and micros()
  without needing to write extra code to account for roll-over of the counters.
  
  
  2012-12-12
  Matt Karau
*/

unsigned long nowMicros;

unsigned long LEDBlinkIntervalMillis = 1000;
unsigned long lastLEDBlinkMillis=0;

unsigned long LEDBlinkIntervalMicros = 5000000;  // 5,000,000 us = 10 s
unsigned long lastLEDBlinkMicros=0;

unsigned long printIntervalMicros = 10000000;   // 10,000,000 us = 10 s
unsigned long lastPrintMicros=0;

int redLEDPin = 12;
int greenLEDPin = 13;
 
 
void setup() {
  lastPrintMicros = (unsigned long)(-1 * printIntervalMicros);
  lastLEDBlinkMicros = (unsigned long)(-1 * LEDBlinkIntervalMicros);
  lastLEDBlinkMillis = (unsigned long)(-1 * LEDBlinkIntervalMillis);
  
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  Serial.begin(57600);
}
 
 
void loop() {
  
  if ((unsigned long)(millis() - lastLEDBlinkMillis) >= LEDBlinkIntervalMillis) {  // Check if it's time to blink.
    digitalWrite(redLEDPin, !digitalRead(redLEDPin));  // Toggle the LED.
    lastLEDBlinkMillis = millis();  // Set the time that we should next blink.
  } 
    
  /*
    By Casting the result of the subraction into "unsigned long" and keeping
    track of time in relation to the target future timer value, we avoid the
    roll-over problem of millis() @ 4294967295 ms = 49.7 days.
  
    For example:
  
    1.  Initial State:
      LEDBlinkIntervalMillis = 1000;
      current lastLEDBlinkMillis = 4,294,966,095
      current millis() = 4,294,967,094  (max = 4,294,967,295)
      millis() - lastLEDBlinkMillis = 999
   
    2.  One millisecond passes, and now millis() is greater than lastLEDBlinkMillis by 1000...
      current lastLEDBlinkMillis = 4,294,966,095
      current millis() = 4,294,967,095
      millis() - lastLEDBlinkMillis = 1000
  
    The condition "if ((unsigned long)(millis() - lastLEDBlinkMillis) >= LEDBlinkIntervalMillis)"
    evaluates true.  We blink the LED and add 1000 to lastLEDBlinkMillis, becomes 4,294,966,095.
  
    3.  One second passes, and now millis() would be 4,294,968,095 but has instead 
    rolled-over the max 4,294,967,295 by 800 (4,294,968,095 - 4,294,967,295).
      current lastLEDBlinkMillis = 4,294,967,095
      current millis() = 800
      millis() - lastLEDBlinkMillis = -4,294,965,295

    We need to now carefully walk through how the condition
    "if ((unsigned long)(millis() - lastLEDBlinkMillis) >= LEDBlinkIntervalMillis)"
    is evaluated.
      if ((unsigned long)(800 - 4,294,966,095) >= 1000)
      if ((unsigned long)(-4,294,965,295) >= 1000)
    
    A number stored as an "unsigned long" cannot be negative, and that is where the magic
    happens here.  When we force -4,294,965,295 to be a positive number by casting (forcably converting)
    the result to be "(unsigned long)", the value wraps around backwards and becomes 1000.
    
    e.g. The moments before and after roll-over.
          (unsigned long)(millis() - lastLEDBlinkMillis)
          (unsigned long)(4,294,967,294 - 4,294,967,095) = (unsigned long)(199) = 199
          (unsigned long)(4,294,967,295 - 4,294,967,095) = (unsigned long)(200) = 200
          --- ROLL-OVER ---   
          [4,294,967,295 + 1 = 4,294,967,296 ===> 0]
          --- ROLL-OVER ---
          (unsigned long)(0 - 4,294,967,095) = (unsigned long)(-4,294,967,095) = 201
          (unsigned long)(1 - 4,294,967,095) = (unsigned long)(-4,294,967,094) = 202
          ...
          ...
          ...
          (unsigned long)(798 - 4,294,967,095) = (unsigned long)(-4,294,966,297) = 990
          (unsigned long)(799 - 4,294,967,095) = (unsigned long)(-4,294,966,296) = 1000
          (unsigned long)(800 - 4,294,967,095) = (unsigned long)(-4,294,966,295) = 1001
    
    When we keep track of time in this manner, we inherently take care of the "roll-over problem".  Using
    millis(), the maximum time between events (LEDBlinkIntervalMicros) is limited to around 49.7 days.



    Side Note about negative numbers cast into unsigned long:
          (unsigned long)(4,294,967,297) = 1
          (unsigned long)(4,294,967,296) = 0
          (unsigned long)(4,294,967,295) = 4,294,967,295
          (unsigned long)(4,294,967,294) = 4,294,967,294
          ...
          ...
          (unsigned long)(1) = 1
          (unsigned long)(0) = 0
          (unsigned long)(-1) = 4,294,967,295
          (unsigned long)(-2) = 4,294,967,294
          (unsigned long)(-3) = 4,294,967,293
   
    The number system wraps around in both directions!

    Inspiration for this tutorial came from:
    http://www.cmiyc.com/blog/2012/07/16/arduino-how-do-you-reset-millis/
*/

/*
  Don't Believe about the casting?  Try it yourself.
  Set believe=false; to enable  the printing below.
*/  

  boolean believe = true;
  
  if (!believe) {
    if ((unsigned long)(micros() - lastPrintMicros) >= printIntervalMicros) { 
      lastPrintMicros = micros();
      Serial.println("Roll-over from counting UP.");
      Serial.println();
    
      Serial.print("4,294,967,294 = ");
      Serial.println((unsigned long)(4294967294));
    
      Serial.print("4,294,967,295 = ");
      Serial.println((unsigned long)(4294967295));

      Serial.print("4,294,967,296 = ");
      Serial.println((unsigned long)(4294967295+1));

      Serial.print("4,294,967,297 = ");
      Serial.println((unsigned long)(4294967295+2));


      Serial.println("Roll-over from counting DOWN.");
      Serial.println();
    
      Serial.print("1 = ");
      Serial.println((unsigned long)(1));
    
      Serial.print("0 = ");
      Serial.println((unsigned long)(0));

      Serial.print("-1 = ");
      Serial.println((unsigned long)(0-1));

      Serial.print("-2 = ");
      Serial.println((unsigned long)(0-2));
      
      Serial.print("Our unbelievable interval = ");
      Serial.println((unsigned long)(800 - 4294967095));
    }
  }
  
  
/*
    The same works for micros().  Using micros(), the maximum time between events (LEDBlinkIntervalMicros)
    is limited to 4294967295 us = 71.6 minutes.
*/

  nowMicros = micros();
  if ((unsigned long)(nowMicros - lastLEDBlinkMicros) >= LEDBlinkIntervalMicros) { 
    digitalWrite(greenLEDPin, !digitalRead(greenLEDPin));
    lastLEDBlinkMicros = nowMicros;
    Serial.print(((float)nowMicros/(float)60000000));
    Serial.print("\t");
    Serial.println (nowMicros - lastLEDBlinkMicros);
  } 

}
