// Do not remove the include below

#define _BLE_TRACE_

#include "GIGA_M7.h"
#include "Utilities/src/Controller.h"
#include "Utilities/src/Phaserunner.h"

void setup()
{
  Serial.begin(230400);
  Serial2.begin(230400);
  Serial3.begin(230400);
  while (!Serial);
  pinMode(86, OUTPUT);
  digitalWrite(86, LOW);

  volatile Phaserunner L10;

}

void loop() {
	  if(Serial2.available())
	  {
		  char c = Serial2.read();
		  Serial.write(c);
		  Serial3.write(c);
	  }
}
