/**
 * watchForJump
 * 2012-11-02
 * Code: Matt Karau (mkarau@gmail.com)
 * UdK Digital Class, Short-term project: Freeze
 * 
 * Read data from the serial port (Arduino) and watch for a specific command.
 * When the "Jump" signal is received, we touch a text file that is watched by
 * an external shell script.  That shell script triggers gphoto2 to take a photo.
 
 Note: In order to use gphoto2 on a Mac, you need to follow a quite lengthy
 process of installing Xcode, macports, and then gphoto2.  Google for it.
 
 *
 */

import processing.serial.*;
PrintWriter output;
PImage bg;
String imageFilename = "../../jumpPhotos/wallpaper.jpg";
String imageTriggerFilename = "../takePhoto.txt";
String jumpSignalText = "Jump";
boolean newMessage = false;

int lf = 10;    // Linefeed in ASCII
String myString = null;
Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port

void setup() 
{
//  size(864, 576);  
  size(432, 288);  
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 57600);
  myPort.clear();
  // Throw out the first reading, in case we started reading 
  // in the middle of a string from the sender.
  
  // Buffer incoming text until we see a Line Feed.
  myPort.bufferUntil(lf);
  myString = null;

  // Load the last image taken.
  bg = loadImage(imageFilename);
}

void draw()
{
  // LAZY! But we load the image new each time around.
  bg = loadImage(imageFilename);

  // Display the image
  image(bg, 0, 0, width, height);  
}

void serialEvent(Serial p) {
  // When a serial message has been buffered, we read it.
  myString = (myPort.readString());
  // Trim any leading or trailing whitespace.
  trim(myString);
  // Trim any <CR> or <LF> characters
  myString = myString.replace("\n", "").replace("\r", "");
    // Check we get the message desired
    if (myString.equals(jumpSignalText)) {
      // Print a message to the terminal
      println(myString+" "+myString.length());
      // and print a message to the trigger file
      output = createWriter(imageTriggerFilename); 
      output.println(myString+" "+myString.length());
      output.flush(); // Writes the remaining data to the file
      output.close(); // Finishes the file 
    } 
}

