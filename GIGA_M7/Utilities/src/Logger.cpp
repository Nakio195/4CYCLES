/*
 * Logger.cpp
 *
 *  Created on: 12 nov. 2024
 *      Author: TooT
 */

#include "Logger.h"

Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}


void Logger::log(LogLevel level, uint32_t uid, arduino::String payload)
{
	Serial.print(level); Serial.print(" : "); Serial.print(uid); Serial.print(" : "); Serial.println(payload);
}
