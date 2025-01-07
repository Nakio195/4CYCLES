/*
 * ModbusDriver.h
 *
 *  Created on: 2 janv. 2025
 *      Author: To
 */

#ifndef MODBUSDRIVER_H_
#define MODBUSDRIVER_H_


#include <Arduino.h>
#include "PhaserunnerRegisterMap.h"

#include <ArxContainer.h>

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

#define UART1_TX_PIN 18       // TX
#define UART1_RX_PIN 19      // RX
#define UART1_RE 22     // RE
#define UART1_DE 24	// DE

#define PANIC_FIFO_SIZE 12

class ModbusPacket
{
	public:
		explicit ModbusPacket(uint16_t slaveID, bool dir = Read) : slave(slaveID), direction(dir)
		{
		}

		void push(const Register& r)
		{
			registers.push_back(r);
		}

	public:
		enum {Read = 0, Write = 1};
		uint8_t slave; // ID of the recipient
		bool direction; // Read = false, Write = true

		std::vector<Register> registers = {};

		bool success = 0;
};

class ModbusDriver
{
	public:
		ModbusDriver();

		void process();

		bool request(ModbusPacket *paquet);
		ModbusPacket* getAnswer(uint8_t slaveID);
		uint8_t available(uint8_t slaveID);

		uint32_t successRequest;
		uint32_t failedRequest;

		uint16_t requestPanicCounter;
		uint16_t answerPanicCounter;

		uint8_t successiveFailure;

		std::deque<ModbusPacket*> RequestFIFO;
		std::deque<ModbusPacket*> AnswerFIFO;

	private:
		uint16_t calculateBlockSize(const std::vector<Register>& registers, uint8_t startIndex, uint16_t startAddress);
};


#endif /* MODBUSDRIVER_H_ */
