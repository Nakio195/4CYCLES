/*
 * Action.h
 *
 *  Created on: 3 nov. 2024
 *      Author: TooT
 */

#ifndef CONTROLACTION_H_
#define CONTROLACTION_H_

#include <Arduino.h>
#include <ArduinoJson.h>

class ControlAction
{
	enum ActionType{Invalid, Link, Throttle, Direction, Brake, Turn, HeadLight, Horn};

	public:
		ControlAction();
		virtual ~ControlAction();

		static ControlAction* from(HardwareSerial &Serial);

	private:
		ActionType mType;
		uint32_t mTimestamp;
		JsonDocument mAction;

};

#endif /* CONTROLACTION_H_ */
