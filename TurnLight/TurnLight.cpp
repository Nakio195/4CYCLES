// Do not remove the include below
#include "TurnLight.h"
#include "AnimatedPixel.h"
#include <Adafruit_NeoPixel.h>

#define PIN        6
#define NUMPIXELS 50

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

//The setup function is called once at startup of the sketch
unsigned long long previousTime = 0;

AnimatedPixel* mPixels[3];

void setup()
{

	pixels.begin();

	mPixels[0] = new AnimatedPixel(0, 5, pixels.Color(255, 100, 0), AnimatedPixel::FadeOut, true, 666);
	mPixels[1] = new AnimatedPixel(5, 40, pixels.Color(255, 200, 200), AnimatedPixel::Static);
	mPixels[2] = new AnimatedPixel(45, 5, pixels.Color(255, 75, 0), AnimatedPixel::Swing, true, 333);
	mPixels[0]->start();
	mPixels[1]->start();
	mPixels[2]->start();


	pixels.rainbow();
	pixels.show();

	Serial.begin(115200);
	Serial.println("Starting... ");

	delay(2000);
}

void loop()
{
	unsigned long long dt = millis() - previousTime;
	previousTime = millis();

	for(int i=0; i< 3; i++)
		mPixels[i]->run(dt);

	pixels.show();
}
