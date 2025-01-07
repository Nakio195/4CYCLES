/*
 * ModbusDriver.cpp
 *
 *  Created on: 2 janv. 2025
 *      Author: To
 */

#include "ModbusDriver.h"


RS485Class customRS485(Serial1, UART1_TX_PIN, UART1_RE, UART1_DE);


ModbusDriver::ModbusDriver()
{
	// Define a specific HardwareSerial as interface
	customRS485.begin(115200, SERIAL_8N1);
	// Start Modbus Client with given interface
	ModbusRTUClient.begin(customRS485, 115200);
	ModbusRTUClient.setTimeout(200);

	//TODO Retrieve from FLASH or EEPROM values
	successRequest = 0;
	failedRequest = 0;
	requestPanicCounter = 0;
	answerPanicCounter = 0;
	successiveFailure = 0;
}


/*
 * This function handle the hadware and process the request and retrieve the answer from the modbus slaves
 */


void ModbusDriver::process()
{
	if(RequestFIFO.size() == 0)
	{
		return;
	}

	while(RequestFIFO.size() > 0)
	{
		ModbusPacket* packet = RequestFIFO.front();
		RequestFIFO.pop_front();

		// Getting packet informations
		uint16_t startAddress = packet->registers[0].address;
		uint16_t blockSize = packet->registers.size();
		bool direction = packet->direction;
		uint8_t slaveID = packet->slave;

		/*
		 * Writing multiple registers ans handling error information
		 */
		if(direction == ModbusPacket::Write)
		{
			packet->success = true;

			// Begin transmission for first register
			blockSize = calculateBlockSize(packet->registers, 0, startAddress);
			packet->success &= ModbusRTUClient.beginTransmission(slaveID, HOLDING_REGISTERS, startAddress, blockSize);

			for(uint8_t i = 0; i < packet->registers.size(); i++)
			{
				uint16_t previousAddress = i == 0 ? startAddress : packet->registers[i-1].address;
				// Next address is not contiguous
				// Ending transmission, checking for error and starting new transimission
				if(abs(packet->registers[i].address - previousAddress) > 1)
				{
					packet->success &= ModbusRTUClient.endTransmission();

					if(!packet->success)
					{
						failedRequest++;
						successiveFailure++;
						//TODO Warn about a failed transmission
						Serial.println(ModbusRTUClient.lastError());
					}

					else
					{
						successRequest++;
						successiveFailure = 0;
					}

					blockSize = calculateBlockSize(packet->registers, i, packet->registers[i].address);
					packet->success &= ModbusRTUClient.beginTransmission(slaveID, HOLDING_REGISTERS, packet->registers[i].address, blockSize);

				}

				// Writing words to bus
				packet->success &= ModbusRTUClient.write(packet->registers[i].value);


			}

			packet->success &= ModbusRTUClient.endTransmission();

			if(!packet->success)
			{
				failedRequest++;
				successiveFailure++;
				//TODO Warn about a failed transmission
				Serial.println(ModbusRTUClient.lastError());
			}

			else
			{
				successRequest++;
				successiveFailure = 0;
			}
		}

		if(direction == ModbusPacket::Read)
		{
			packet->success = true;

			if(blockSize == 1)
			{
				uint16_t read = ModbusRTUClient.holdingRegisterRead(slaveID, startAddress);
				if(read == -1)
				{
					failedRequest++;
					successiveFailure++;
					packet->success = false;
					Serial.println(ModbusRTUClient.lastError());
				}
				else
					packet->registers[0].value = read;
			}

			else
			{
				ModbusRTUClient.requestFrom(slaveID, HOLDING_REGISTERS, startAddress, blockSize);

				if(ModbusRTUClient.available() == blockSize)
				{
					for(uint8_t i = 0; i < blockSize; i++)
					{
						uint16_t read = ModbusRTUClient.read();
						if(read == -1)
						{
							failedRequest++;
							successiveFailure++;
							packet->success = false;
							Serial.println(ModbusRTUClient.lastError());
							break;
						}
						else
							packet->registers[i].value = read;
					}

					successRequest++;
					successiveFailure = 0;
				}

				else
				{
					packet->success = false;
					failedRequest++;
					successiveFailure++;
					Serial.println(ModbusRTUClient.lastError());

					//TODO Warn about invalid read
				}
			}

		}

		/*
		 * Transferring packet to Answer FIFO with results
		 */
		if(AnswerFIFO.size() >= PANIC_FIFO_SIZE)
		{
			answerPanicCounter++;
			//TODO Use logger to warn user about a Panic situation
		}

		AnswerFIFO.push_back(packet);
	}
}

bool ModbusDriver::request(ModbusPacket *packet)
{
	if(RequestFIFO.size() >= PANIC_FIFO_SIZE)
	{
		requestPanicCounter++;
		//TODO Use logger to warn user about a Panic situation
		return false;
	}

	if(packet->registers.size() > 0)
	{
		RequestFIFO.push_back(packet);
	}

	else
	{
		requestPanicCounter++;
		//TODO Use logger to warn about a malformed packet
		return false;
	}

	return true;

}

ModbusPacket* ModbusDriver::getAnswer(uint8_t slaveID)
{
	if(AnswerFIFO.size() == 0)
		return nullptr;

	if(AnswerFIFO.front()->slave == slaveID)
	{
		ModbusPacket* packet = AnswerFIFO.front();
		AnswerFIFO.pop_front();
		return packet;
	}

	else
		return nullptr;
}

uint8_t ModbusDriver::available(uint8_t slaveID)
{
	uint8_t count = 0;

	if(AnswerFIFO.size() == 0)
		return count;

	for(uint8_t i = 0; AnswerFIFO.size(); i++)
	{
		if(AnswerFIFO[i]->slave == slaveID)
			count++;
	}

	return count;
}

uint16_t ModbusDriver::calculateBlockSize(const std::vector<Register>& registers, uint8_t startIndex, uint16_t startAddress)
{
    uint16_t size = 0;

    for (uint8_t i = startIndex; i < registers.size(); i++)
    {
        if (abs(registers[i].address - startAddress) > 1)
        {
            break;
        }
        size++;
        startAddress = registers[i].address;
    }
    return size;
}
