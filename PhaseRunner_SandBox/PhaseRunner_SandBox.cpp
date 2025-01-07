// Do not remove the include below
#include "PhaseRunner_SandBox.h"
#include "ModbusDriver.h"
#include "Phaserunner.h"

ModbusDriver *ModbusHandler;

Phaserunner* Motor;

long heartbeat = 0;
long Report = 0;
bool motorRunning = false;
bool config = false;
float speed = 0;
bool dir = false;
uint8_t tick = 0;

int freeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	Serial.begin(115200);
	while (!Serial);

	pinMode(13, OUTPUT);
	ModbusHandler = new ModbusDriver();
	Motor = new Phaserunner(1);

	Motor->startMotor();
}

// The loop function is called in an endless loop
void loop()
{

	if(millis() - heartbeat > 100)
	{
		heartbeat = millis();

		if(speed >= 100)
			dir = true;
		if(speed <= 0)
			dir = false;

		dir ? speed-- : speed++;

		Motor->setSpeedCommand(speed);
	}

	if(millis() - Report > 2000)
	{
		Report = millis();

		tick++;

		if(tick == 20)
			Motor->stopMotor();

		Serial.println("----");
		Serial.print("Failed : "); Serial.println(ModbusHandler->failedRequest);
		Serial.print("Success : "); Serial.println(ModbusHandler->successRequest);
		Serial.print("Panic Request : "); Serial.println(ModbusHandler->requestPanicCounter);
		Serial.print("Panic Answer : "); Serial.println(ModbusHandler->answerPanicCounter);
		Serial.print("Free Memory : "); Serial.println(freeMemory());

	}

	Motor->process();
	ModbusHandler->process();

}
