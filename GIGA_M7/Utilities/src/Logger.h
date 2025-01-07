/*
 * Logger.h
 *
 *  Created on: 12 nov. 2024
 *      Author: TooT
 */

#ifndef UTILITIES_SRC_LOGGER_H_
#define UTILITIES_SRC_LOGGER_H_

#include <Arduino.h>

class Logger
{
	public:
		enum LogLevel{Critical, Error, Warning, Info};

	public:
		Logger();
		virtual ~Logger();

		static void log(LogLevel level, uint32_t uid, arduino::String payload);
};


/* UID Dictionnary
 *
 * Control Dictionnary
 * 100 --> 199
 *
 * ControlAction
 * 		100 : Deserialization global error
 * 		101 : Invalid action Type
 * 		102 : Invalid timestamp
 *
 *
 *
 *
 */
#endif /* UTILITIES_SRC_LOGGER_H_ */
