/*
 * Timer.h
 *
 *  Created on: 28 déc. 2022
 *      Author: TooT
 */

#ifndef DRIVERS_TIMER_H_
#define DRIVERS_TIMER_H_

#include "Arduino.h"


class Timer
{
    public:
        enum Mode{OneShot = 0, Continuous};

    public:
        Timer(uint16_t pPeriod = 200, unsigned int pMode = 0);

        bool isRunning() const;

        float getPeriod() const;
        void setPeriod(uint16_t pPeriod);

        float getCounter() const;
        void tick(uint16_t dt);

        bool triggered();
		void setTriggerAction(void (*Press)(Timer *t));

        void startTimer(float pPeriod = 0);  // Start Timer
        void pauseTimer();
        void releaseTimer();
        void stopTimer();                   // Stop

        unsigned int getMode() const;
        void setMode(unsigned int pMode);

        float getProgression() const;

        uint16_t counts();
        void resetCounts();

        bool toggled();

    protected:
        unsigned int mMode;  // Timer mode according to enum TimerMode
        bool mRunning;

        uint16_t mPeriod;       //Time till event trigger in s
        uint16_t mCounter;      //Elapsed time since start in s
        bool mTrigger;       //Event triggered

        uint16_t mCounts;	 //Number of trigger since last reset (see resetCounts())
        bool mToggled;

		void (*onTriggerAction)(Timer *t);
};

#endif /* DRIVERS_TIMER_H_ */
