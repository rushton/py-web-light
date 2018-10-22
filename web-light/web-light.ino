#include <RGBLED.h>

/*
  Keyboard test

  For the Arduino Leonardo, Micro or Due

  Reads a byte from the serial port, sends a keystroke back.
  The sent keystroke is one higher than what's received, e.g. if you send a,
  you get b, send A you get B, and so forth.

  The circuit:
  - none

  created 21 Oct 2011
  modified 27 Mar 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/KeyboardSerial
*/

#include "Keyboard.h"
union Message
{
    byte data[8];
    struct
    {
      unsigned int address;
      unsigned int red;
      unsigned int green;
      unsigned int blue;
    };
}__attribute__((packed));

union Msg {
  byte data[2];
  int red;
};


Message m;

RGBLED rgbLed(11,9,6,COMMON_ANODE);
void setup() {
  // open the serial port:
  Serial.begin(9600);

}

void readMessage() {
  for (int i =0; i < 8; i++) {
    byte val = Serial.read();
    m.data[i] = val;
  }
  Serial.println("ACK");
}

void loop() {
  // check for incoming serial data:
  if (Serial.available() > 7) {
    readMessage();
    rgbLed.writeRGB(m.red, m.green, m.blue);
  }
}
