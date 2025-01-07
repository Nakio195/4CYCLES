/*
 * Phaserunner.cpp
 *
 *  Created on: 10 déc. 2024
 *      Author: To
 */

#include "Phaserunner.h"

Phaserunner::Phaserunner(uint8_t slaveID)
{
	mConnection.slaveID = slaveID;
	mRegisters = new Registers;

	for(const auto& r : mRegisters->map)
	{
		Serial.print('@'); Serial.print(r.address);
		Serial.print(", scale "); Serial.print(r.scale);
		Serial.print(", value "); Serial.print(r.value);
		Serial.print(", pending "); Serial.println(r.pendingWrite);
	}

	instantRequest(509, 0);
	setControlSource(0);
	setCurrentsLimits(100.0, 100.0);
	setSpeedRegulatorMode(2);
	setTorqueCommand(50.0);
}

void Phaserunner::startMotor()
{
	setSpeedCommand(0);
	setRemoteState(2);
}

void Phaserunner::stopMotor()
{
	setSpeedCommand(0);
	setRemoteState(1);
}


void Phaserunner::process()
{
	// Send all non critical pending write request

	std::vector<Register> pendingRegisters;

	bool needTransmit = false;
	for(auto& r : mRegisters->map)
	{
		if(r.pendingWrite)
		{
			pendingRegisters.push_back(r);
			r.pendingWrite = false;
			needTransmit = true;
		}
	}

	if(needTransmit)
	{
		ModbusPacket* request = new ModbusPacket(mConnection.slaveID, ModbusPacket::Write);
		request->registers = pendingRegisters;
		ModbusHandler->request(request);
	}

	// Read one received Answer
	//delete ModbusHandler->getAnswer(mConnection.slaveID);
	ModbusPacket* answer = ModbusHandler->getAnswer(mConnection.slaveID);

	if(answer == nullptr)
		return;

	if(!answer->success)
	{
		//TODO Warn a about a invalid answer
		Serial.print("\tError ");
		answer->direction == ModbusPacket::Read ? Serial.println("reading :") : Serial.println("writing :");
		for(const auto& r : answer->registers)
		{
			Serial.print("\t\t@");
			Serial.println(r.address);
		}
		delete answer;
	}

	else
	{
//		Serial.print("\tSuccess ");
//		answer->direction == ModbusPacket::Read ? Serial.println("reading :") : Serial.println("writing :");
//		for(const auto& r : answer->registers)
//		{
//			Serial.print("\t@");
//			Serial.println(r.address);
//		}
		delete answer;
	}

}


bool Phaserunner::instantRequest(uint8_t add, uint16_t val)
{
	ModbusPacket* packet = new ModbusPacket(mConnection.slaveID, ModbusPacket::Write);
	packet->push(Register(add, 0, val));
	ModbusHandler->request(packet);
}

bool Phaserunner::readAllParameters()
{
	return true;
}

bool Phaserunner::setControlSource(uint8_t source)
{
	/*
	 *  @208
	 *  Specify command source
	 * 		0 is serial, ..., 5
	 */
	if(source < 0 || source > 5)
		return false;

	mRegisters->set(208, source);

	return true;
}

bool Phaserunner::setSpeedRegulatorMode(uint8_t mode)
{
	/*
	 *  @11
	 *  Specify regulator mode
	 *  	0 - Speed, 1 - Torque, 2 - Speed Limit + Torque
	 */
	if(mode < 0 || mode > 2)
		return false;

	mRegisters->set(11, mode);
	return true;

}
bool Phaserunner::setSpeedCommand(float speed)
{
	/* CRITICAL
	 *  @490
	 *  Value is % of Rated RPM (kV*Bat Voltage)
	 * 		100% is 4096
	 */

	if(speed > 100.0)
		return false;

	mRegisters->set(490, 4095*(speed/100.0));
	return true;
}
bool Phaserunner::setCurrentsLimits(float motor, float brake)
{
	/*
	 *  @491 - Motor current
	 *  @492 - Brake current
	 *  Value are % of Nominal currents
	 *  	100% is 4096
	 */

	if(motor > 100.0 || brake > 100.0)
		return false;

	mRegisters->set(491, 4096*(motor/100.0));
	mRegisters->set(492, 4096*(brake/100.0));

	return true;
}
bool Phaserunner::setRemoteState(uint8_t state)
{
	/*	CRITICAL
	 *  @493
	 *  Specify motor state
	 *  	0 - OFF, 1 - IDLE, 2 - RUNNING
	 */

	if(state < 0 || state > 2)
		return false;

	mRegisters->set(493, state);
	return true;

}

bool Phaserunner::setTorqueCommand(float torque)
{
	/* CRITICAL
	 *  @494
	 *  Value is % of Rated torque
	 * 		100% is 4096
	 */

	if(torque > 100.0)
		return false;

	mRegisters->set(494, 4096*(torque/100.0));
	return true;

}
