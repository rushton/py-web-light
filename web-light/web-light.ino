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
    byte data[10];
    struct
    {
      unsigned int address;
      unsigned int red;
      unsigned int green;
      unsigned int blue;
	  unsigned int duration;
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
  for (int i =0; i < 10; i++) {
    byte val = Serial.read();
    m.data[i] = val;
  }
  Serial.println("ACK");
}


void transition(int r, int g, int b, int duration) {
    float rtick = float(r - rgbLed.redValue) / float(duration);
    float gtick = float(g - rgbLed.greenValue) / float(duration);
    float btick =  float(b - rgbLed.blueValue) / float(duration);
    int sr = rgbLed.redValue;
    int sg = rgbLed.greenValue;
    int sb = rgbLed.blueValue;

	if (duration == 0) {
    	rgbLed.writeRGB(r,g,b);
	} else {
		for (int i = 1; i <= duration; i++) {

			rgbLed.writeRGB(
				int(sr + rtick * i),
				int(sg + gtick * i),
				int(sb + btick * i)
			);
			delay(1);
		}
	}
}

void loop() {
  // check for incoming serial data:
  if (Serial.available() > 9) {
    readMessage();
    transition(m.red, m.green, m.blue, m.duration);
  }
}
