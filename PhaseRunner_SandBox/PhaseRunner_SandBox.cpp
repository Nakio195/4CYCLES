// Do not remove the include below
#include "PhaseRunner_SandBox.h"
#include "ModbusDriver.h"
#include "Phaserunner.h"
#include "Timer.h"

ModbusDriver *ModbusHandler;
Phaserunner* Motor;


Timer TimerReport(2000, Timer::Continuous);

bool MotorRunning = false;

unsigned long previousTick = 0;

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
	pinMode(7, INPUT_PULLUP); 	//StartMotor
	pinMode(8, INPUT_PULLUP);	//ClearFaults

	ModbusHandler = new ModbusDriver();
	Motor = new Phaserunner(1);

	TimerReport.startTimer();
}

// The loop function is called in an endless loop
void loop()
{
	unsigned long dt = millis() - previousTick;
	previousTick = dt == 0 ? previousTick : millis();

	Motor->tick(dt);
	TimerReport.tick(dt);

	if(TimerReport.triggered())
	{
		Serial.println("----");
		Serial.print("Failed : "); Serial.println(ModbusHandler->failedRequest);
		Serial.print("Success : "); Serial.println(ModbusHandler->successRequest);
		Serial.print("Panic Request : "); Serial.println(ModbusHandler->requestPanicCounter);
		Serial.print("Motor Faults : "); Serial.println(Motor->getMotorFaults().faults, HEX);
		Serial.print("Motor State : "); Serial.println(MotorRunning, HEX);
		Serial.print("Panic Answer : "); Serial.println(ModbusHandler->answerPanicCounter);
		Serial.print("Free Memory : "); Serial.println(freeMemory());

		if(!digitalRead(7))
		{
			MotorRunning = !MotorRunning;

			if(MotorRunning)
			{
				Motor->startMotor();
				Motor->setSpeed(1);
			}

			else
				Motor->stopMotor();
		}

		if(!digitalRead(8))
			Motor->clearFaults();

	}

	Motor->process();
	ModbusHandler->process();

}
