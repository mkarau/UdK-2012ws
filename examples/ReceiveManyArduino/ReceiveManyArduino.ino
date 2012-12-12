/*
  Example of how to receive many values into Arduino
 
 Expects data arriving over serial port like:
 1234;1456;1445\r\n
 
 2012-12-12
 Matt Karau
 
 */
char serialInString[100];
int incomingInts[4] = {
  0, 0, 0, 0};
int serialIndex = 0;
int incomingByte;


void assignValues (char *buf) {
  char *p = buf;
  char *str;
  int index = 0;
  while ((str = strtok_r(p, ";", &p)) != NULL) {
    char *h;
    incomingInts[index] = atoi (str);
    index++;
    if (index > (sizeof(incomingInts)/sizeof(int))) break;
  }

}


void setup() {
  Serial.begin(57600);
}


void loop() {

  if (Serial.available() ) {
    incomingByte = Serial.read();

    if ((incomingByte == 10) || (incomingByte == 13)) {
      if (serialIndex > 0) {
        serialInString[serialIndex] = '\0';
        Serial.println(serialInString);
        assignValues(serialInString);
        for (int i=0; i<(sizeof(incomingInts)/sizeof(int)); i++) {
          Serial.println(incomingInts[i]);
        }
        strcpy(serialInString, "");
        serialIndex = 0;
      }
    } 
    else {
      serialInString[serialIndex] = incomingByte;
      serialIndex++;
    }

  }
}

