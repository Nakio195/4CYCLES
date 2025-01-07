/*
 * AnimatedPixel.cpp
 *
 *  Created on: 21 déc. 2024
 *      Author: To
 */

#include "AnimatedPixel.h"

AnimatedPixel::AnimatedPixel(uint8_t pos, uint8_t length, uint32_t baseColor, uint8_t type, bool repeat, float duration)
{
	mPos = pos;
	mDuration = duration;
	mBaseColor = baseColor;
	mLength = length;
	mCurrentColor = 0;
	mType = type;
	mElapsedTime = 0;
	mStarted = false;
	mRepeat = repeat;
	mFinished = false;

	mSwingDirection = false;

	R = baseColor >> 16;
	G = baseColor >> 8;
	B = baseColor & 0xFF;
	A = 0;
}

void AnimatedPixel::start()
{
	mStarted = true;
	R = mBaseColor >> 16;
	G = mBaseColor >> 8;
	B = mBaseColor & 0xFF;
	A = (mType == FadeIn) || (mType == Swing) ? 0.f : 255.f;
}

void AnimatedPixel::setDuration(float duration)
{
	mDuration = duration;
}

void AnimatedPixel::run(uint32_t dt)
{
	if(!mStarted || mFinished)
		return;

	if(mType == Static)
	{
		R = mBaseColor >> 16;
		G = mBaseColor >> 8;
		B = mBaseColor & 0xFF;
		pixels.fill(pixels.Color(pixels.gamma8(float(R)*A/255.0), pixels.gamma8(float(G)*A/255.0), pixels.gamma8(float(B)*A/255.0)), mPos, mLength);

		return;
	}

	mElapsedTime += dt;

	float mStep = 2550/mDuration;

	if(mElapsedTime >= 10)
	{
		mElapsedTime = 0;

		if(mType == FadeIn)
		{
			if(A + mStep >= 255.0)
			{
				if(mRepeat)
					start();
				else
					mFinished = true;
			}

			else
				A += mStep;
		}

		if(mType == FadeOut)
		{
			if(A - mStep <= 0)
			{
				if(mRepeat)
					start();
				else
					mFinished = true;
			}

			else
				A -= mStep;
		}

		if(mType == Swing)
		{
			if(mSwingDirection)
			{
				if(A + mStep >= 255.0)
					mSwingDirection = !mSwingDirection;
				else
					A += mStep;
			}

			else
			{
				if(A - mStep <= 0)
					mSwingDirection = !mSwingDirection;
				else
					A -= mStep;
			}
		}
		pixels.fill(pixels.Color(pixels.gamma8(float(R)*A/255.0), pixels.gamma8(float(G)*A/255.0), pixels.gamma8(float(B)*A/255.0)), mPos, mLength);
	}
}
