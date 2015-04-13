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
		pulseColor();
      break;
    case 2:
		pulseColor2();
      break;
	case 3:
		controlColor();
      break;
    default: 
      randomColorsAndSpin();
  }
}

byte randomColor()
{
  return random(10, 255);
}

int const redPin = A2;
int const greenPin = A3;
int const bluePin = A4;

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
		if(i <= NUMPIXELS/2)
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
	
	for(int i = 0; i< NUMPIXELS; i++)
	{
		for(int j = 0; j< NUMPIXELS; j++)
		{
			int pixel = j - i;
			if(pixel < 0)
				pixel += NUMPIXELS;
			pixels.setPixelColor(pixel, pixels.Color(leds[j].r,leds[j].g,leds[j].b)); 
		}
		
			pixels.setBrightness(50);
			pixels.show(); 
			delay(200); 
	}
}


void randomColorsForAllLeds()
{
	for(int i = 0; i< NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(randomColor(),randomColor(),randomColor())); 
	}
	
	pixels.setBrightness(50);
	pixels.show(); 
	
	delay(1000); 
}



void pulseColor()
{
	pixels.setBrightness(40);
	byte green = 0; //random(255);
	byte blue = 0; //random(255);
	byte maxBrightness = 255;
	byte minBrightness = 0;
	byte steps = 10;
	
	
	int totalPulses = 5;
	
	for(int i = 0; i < totalPulses; i++)
	{	
		int currentBrightness = minBrightness;
		while(currentBrightness < maxBrightness)
		{
 
		for(int j = 0; j< NUMPIXELS; j++)
		{
			pixels.setPixelColor(j, pixels.Color(currentBrightness,green,blue)); 
		}
	
			pixels.show(); 
			
			delay(50); 
			
			currentBrightness += steps;
		}
		
	}
	
	currentTask = 2;
}

void pulseColor2()
{
	byte red = random(1, 255);
	byte green = random(1, 255);
	byte blue = random(1, 255);
	
	for(int i = 0; i< NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(red,green,blue)); 
	}
	
	byte maxBrightness = 40;
	byte minBrightness = 1;
	byte steps = 2;
	
	
	int totalPulses = 5;
	
	for(int i = 0; i < totalPulses; i++)
	{	
		int currentBrightness = minBrightness;
		while(currentBrightness <= maxBrightness)
		{
			pixels.setBrightness(currentBrightness);
			pixels.show(); 
			
			delay(50); 
			
			currentBrightness += steps;
		}
		
	}
	
	currentTask = 0;
}

/*

- Random color for each led and rotate
- start 2 colors at top and "fall" down and crash into each other at bottom
- pulse a random color (like a heart beat)

*/