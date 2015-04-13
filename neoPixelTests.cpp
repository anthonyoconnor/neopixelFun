#include <Adafruit_NeoPixel.h>

#define PIN            6
#define NUMPIXELS      16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
	Serial.begin(9600);
	pixels.begin(); 
	randomSeed(analogRead(0));
}

byte currentTask = 3;
void loop()
{
	switch (currentTask) {
    case 1:
		doNothing();
      break;
    default: 
      doNothing();
  }
}

void doNothing()
{
	
}

byte randomColor()
{
  return random(10, 255);
}

const int redPin = A2;
const int greenPin = A3;
const int bluePin = A4;

const int globalBrightness = 30;

void controlColor()
{
	int redVal = analogRead(redPin);
	int greenVal = analogRead(greenPin);
	int blueVal = analogRead(bluePin);

	int setRed = map(redVal, 0, 1023, 10, 255);
	int setBlue = map(blueVal, 0, 1023, 10, 255);
	int setGreen = map(greenVal, 0, 1023, 10, 255);
	
	for(int i = 0; i< NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(setRed,setGreen,setBlue)); 
	}
	
	pixels.setBrightness(25);
	pixels.show(); 
}



//*******************************************
// Random Colors and Spin
//*******************************************

const int ledsToLight = 8;
const int timesToSpin = 2;
const int delayDuringSpin = 200;

void randomColorsAndSpin()
{
	struct led
	{
		byte r;
		byte g;
		byte b;
	};

	led leds[NUMPIXELS];
	for(int i = 0; i< NUMPIXELS; i++)
	{
		if(i <= ledsToLight)
		{
			leds[i].r = randomColor();
			leds[i].g = randomColor();
			leds[i].b = randomColor();
		}
		else
		{
			leds[i].r = leds[i].g = leds[i].b = 0;
		}
	}
	for(int spin = 0; spin <timesToSpin; spin++)
	{
		for(int i = 0; i< NUMPIXELS; i++)
		{
			for(int j = 0; j< NUMPIXELS; j++)
			{
				int pixel = j - i;
				if(pixel < 0)
				{
					pixel += NUMPIXELS;
				}
				
				pixels.setPixelColor(pixel, pixels.Color(leds[j].r,leds[j].g,leds[j].b)); 
			}
			
			pixels.setBrightness(globalBrightness);
			pixels.show(); 
			delay(delayDuringSpin); 
		}
	}
}



//*******************************************
// Cycling Random Colors on all LEDS 
//*******************************************

const int totalCycles = 5;
const int timeBetweenCycles = 1000;

void randomColorsForAllLeds()
{
	for(int count = 0; count < totalCycles; count++)
	{
		for(int i = 0; i< NUMPIXELS; i++)
		{
			pixels.setPixelColor(i, pixels.Color(randomColor(),randomColor(),randomColor())); 
		}
		
		pixels.setBrightness(globalBrightness);
		pixels.show(); 
		
		delay(timeBetweenCycles); 
	}
}



//*******************************************
// Pulse - Out and In
//*******************************************

const int totalPulses = 5;
const byte maxBrightness = 40;
const byte minBrightness = 1;
const byte steps = 2;
const int stepDelay = 50;

void pulseRed()
{
	byte red = 255;
	byte green = 0;
	byte blue = 0;
	
	pulse(red, green, blue);
}

void pulseGreen()
{
	byte red = 0;
	byte green = 255;
	byte blue = 0;
	
	pulse(red, green, blue);
}

void pulseBlue()
{
	byte red = 0;
	byte green = 0;
	byte blue = 255;
	
	pulse(red, green, blue);
}

void pulseRandomColor()
{
	byte red = randomColor();
	byte green = randomColor();
	byte blue = randomColor();
	
	pulse(red, green, blue);
}

void pulse(byte red, byte green, byte blue)
{	
	for(int i = 0; i< NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(red,green,blue)); 
	}
		
	for(int i = 0; i < totalPulses; i++)
	{	
		//Pulse up
		int currentBrightness = minBrightness;
		while(currentBrightness <= maxBrightness)
		{
			pixels.setBrightness(currentBrightness);
			pixels.show(); 
			
			delay(stepDelay); 
			
			currentBrightness += steps;
		}
		
		//Pulse down
		currentBrightness = maxBrightness;
		while(currentBrightness >= minBrightness)
		{
			pixels.setBrightness(currentBrightness);
			pixels.show(); 
			
			delay(stepDelay); 
			
			currentBrightness -= steps;
		}	
	}
}

/*

- start 2 colors at top and "fall" down and crash into each other at bottom

*/