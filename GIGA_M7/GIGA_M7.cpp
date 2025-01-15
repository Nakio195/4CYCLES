// Do not remove the include below

#define _BLE_TRACE_

#include "GIGA_M7.h"
#include "Utilities/src/Controller.h"
#include "Utilities/src/Phaserunner.h"

void setup()
{
  Serial.begin(230400);
  Serial1.begin(115200);
  Serial2.begin(230400);
  Serial3.begin(230400);
  Serial4.begin(115200);

  while (!Serial);
  pinMode(86, OUTPUT);
  digitalWrite(86, LOW);
}

void loop() {

}
