// Do not remove the include below
#include "GIGA_M7_STM32.h"
#include "ArduinoJson.h"

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	while(!Serial);

	Serial.begin(115200);
	Serial1.begin(115200);
	Serial2.begin(115200);
	Serial3.begin(115200);
	Serial4.begin(115200);
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	Serial.println("Coucou !");
	delay(1000);
}
