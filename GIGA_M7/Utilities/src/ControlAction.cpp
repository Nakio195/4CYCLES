/*
 * Action.cpp
 *
 *  Created on: 3 nov. 2024
 *      Author: TooT
 */

#include "ControlAction.h"
#include "Logger.h"

ControlAction::ControlAction()
{

	mTimestamp = millis();
	mType= Invalid;

}

ControlAction::~ControlAction()
{
}


ControlAction* ControlAction::from(HardwareSerial &Serial)
{
	ControlAction *Action = new ControlAction();

	DeserializationError err = deserializeJson(Action->mAction, Serial);

	if(err == DeserializationError::Ok)
	{
		if(Action->mAction.containsKey("type"))
		{
			switch(Action->mAction.as<ActionType>())
			{
				case ActionType::Invalid:
					Action->mType = Invalid;
					break;

				case ActionType::Link:
					Action->mType = Link;
					break;

				case ActionType::Throttle:
					Action->mType = Throttle;
					break;

				case ActionType::Brake:
					Action->mType = Brake;
					break;

				case ActionType::Direction:
					Action->mType = Direction;
					break;

				case ActionType::HeadLight:
					Action->mType = HeadLight;
					break;

				case ActionType::Turn:
					Action->mType = Turn;
					break;

				case ActionType::Horn:
					Action->mType = Horn;
					break;

				default:
					Action->mType = Invalid;
					Logger::log(Logger::Error, 101, String(int(Action->mAction.as<int>())));
			}
		}

		if(Action->mAction.containsKey("timestamp"))
		{
			if(Action->mAction["timestamp"].as<uint32_t>() > 0)
			{
				Action->mTimestamp = Action->mAction["timestamp"].as<uint32_t>();
			}

			else
				Logger::log(Logger::Error, 102, String(Action->mAction["timestamp"].as<uint32_t>()));
		}
	}
}


