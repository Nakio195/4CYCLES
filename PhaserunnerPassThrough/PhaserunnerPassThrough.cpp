// Do not remove the include below
#include "PhaserunnerPassThrough.h"


//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	Serial.begin(115200);
	Serial1.begin(115200);
}

// The loop function is called in an endless loop
void loop()
{
	if(Serial.available())
		Serial1.write(Serial.read());

	if(Serial1.available())
		Serial.write(Serial1.read());
}
