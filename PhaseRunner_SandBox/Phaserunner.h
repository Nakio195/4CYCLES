/*
 * Phaserunner.h
 *
 *  Created on: 10 déc. 2024
 *      Author: To
 */

#ifndef UTILITIES_SRC_PHASERUNNER_H_
#define UTILITIES_SRC_PHASERUNNER_H_

#include <Arduino.h>
//#include "Timer.h"
#include "ModbusDriver.h"
#include "PhaserunnerRegisterMap.h"

extern ModbusDriver* ModbusHandler;

class Phaserunner
{
	public:
		Phaserunner(uint8_t slaveID);

		struct ConnectionParameters
		{
			bool isConnected;
			uint8_t slaveID;
			//Timer hearthBeat;
		};

		void process();
		void startMotor();
		void stopMotor();
		bool setSpeedCommand(float speed);

	private:

		bool readAllParameters();

		bool setControlSource(uint8_t source);
		bool setSpeedRegulatorMode(uint8_t mode);
		bool setCurrentsLimits(float motor, float brake);
		bool setRemoteState(uint8_t state);
		bool setTorqueCommand(float torque);

		bool instantRequest(uint8_t add, uint16_t val);

		void readMotorFaults();
		void readControllerFaults();

		void heartbeat();

	private:
		ConnectionParameters mConnection;
		Registers *mRegisters;
};

#endif /* UTILITIES_SRC_PHASERUNNER_H_ */
