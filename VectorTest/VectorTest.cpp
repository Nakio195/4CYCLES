// Do not remove the include below
#include "VectorTest.h"
#include <CircularBuffer.hpp>
#include <Array.h>

typedef struct Register
{
	uint16_t address;
	uint16_t value;
};

typedef struct ModbusPacket
{
	enum {Read = 0, Write = 1};
	uint8_t sender; // ID of the sender
	uint8_t slave; // ID of the recipient
	bool direction; // Read = false, Write = true

	Array<Register, 50> registers = {};

	bool success = 0;

	ModbusPacket()
	{
		sender = 0;
		direction = Read;
		slave = 0;
		registers = Array<Register, 50>();
	}
};

CircularBuffer<Register*, 4*4> b;

bool request(Register *packet)
{
	if(b.isFull())
	{
		//TODO Use logger to warn user about a Panic situation
		return false;
	}

	if(packet->address > 0)
	{
		if(!b.unshift(packet))
		{
			//TODO Use logger to warn about a Panic situation
			return false;
		}

		Serial.println("Adding a request to the FIFO");
	}

	else
	{
		//TODO Use logger to warn about a malformed packet
		return false;
	}

	return true;

}

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	Serial.begin(115200);
	Serial.println("");
}

// The loop function is called in an endless loop
void loop()
{
	Register* battery = new Register{265, 48};

	Serial.print("Is Empty ? "); Serial.println(b.isEmpty());
	request(battery);
	Serial.print("Is Empty ? "); Serial.println(b.isEmpty());

	Register* b2;
	while(!b.isEmpty())
	{
		b2= b.pop();
	}

	Serial.print("Is Empty ? "); Serial.println(b.isEmpty());
	Serial.print("\tAddress : "); Serial.println(b2->address);
	Serial.print("\tValue : "); Serial.println(b2->value);

	while(1);

//Add your repeated code here
}
