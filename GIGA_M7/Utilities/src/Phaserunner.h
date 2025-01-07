/*
 * Phaserunner.h
 *
 *  Created on: 10 déc. 2024
 *      Author: To
 */

#ifndef UTILITIES_SRC_PHASERUNNER_H_
#define UTILITIES_SRC_PHASERUNNER_H_

#include <Arduino.h>
#include "Timer.h"
#include "PhaserunnerRegisterMap.h"

class Phaserunner
{
	public:
		Phaserunner();
		virtual ~Phaserunner();

		struct ConnectionParameters
		{
			bool isConnected;
			uint8_t slaveId;
			uint32_t baudRate;
			uint16_t hearthBeatRate;
			Timer hearthBeat;
		};

	private:
		// Write a single register and return a
		uint8_t writeRegister(Register& reg);
		uint16_t readRegister(Register& reg);

		uint8_t writeMultipleRegisters(Register* reg[]);
		uint8_t readMultipleRegisters(Register* reg[]);

	private:

		ConnectionParameters mConnection;
		Registers *mRegisters;
};

#endif /* UTILITIES_SRC_PHASERUNNER_H_ */
