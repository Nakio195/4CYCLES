/*
 * AnimatedPixel.h
 *
 *  Created on: 21 déc. 2024
 *      Author: To
 */

#ifndef ANIMATEDPIXEL_H_
#define ANIMATEDPIXEL_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel pixels;

class AnimatedPixel
{
	public:
		enum Type{Static, Swing, FadeIn, FadeOut};

	public:

		AnimatedPixel(uint8_t pos,  uint8_t length = 1, uint32_t baseColor = 0x0FFF, uint8_t type = FadeOut, bool repeat= false, float duration = 1.0);
		void start();
		void setDuration(float duration);
		void run(uint32_t dt);

	private:
		uint8_t mPos;
		uint8_t mLength;

		uint8_t mType;

		bool mSwingDirection;

		uint16_t mElapsedTime;
		float mDuration;

		uint8_t R;
		uint8_t G;
		uint8_t B;
		float A;

		uint32_t mBaseColor;
		uint32_t mCurrentColor;

		bool mStarted;
		bool mFinished;
		bool mRepeat;

};

#endif /* ANIMATEDPIXEL_H_ */
