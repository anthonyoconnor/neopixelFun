#include <Adafruit_NeoPixel.h>

#define NEOPIXELPIN            9
#define NUMPIXELS      16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);

struct led
{
  byte r;
  byte g;
  byte b;
};

const int maxColorValue = 255;
const int minColorValue = 10;
const int globalBrightness = 30;

const int neopixelPower = 7;
const int redButton = 3;
const int yellowButton = 2;
const int toggle = 4;

void setup()
{
  Serial.begin(9600);
  pixels.begin();
  blankPixels();
  randomSeed(analogRead(0));
  pinMode(toggle, INPUT);
  pinMode(redButton, INPUT);
  pinMode(yellowButton, INPUT);
}

void blankPixels()
{
  for (int i = 0; i< NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }

  pixels.setBrightness(0);
  pixels.show();
}

bool redButtonPressed = false;
byte MAXTASKS = 7;
byte currentTask = 0;
byte oldTask = 0;
void loop()
{
  checkDirection();
  checkYellowButton();
  checkRedButton();

  switch (currentTask)
  {
  case 1:
    randomColorsAndSpin();
    break;
  case 2:
    pulseRed();
    break;
  case 3:
    randomColorsForAllLeds();
    break;
  case 4:
    pulseRandomColor();
    break;
  case 5:
    pulseBlue();
    break;
  case 6:
    quarterSpin();
    break;
  case 7:
    pulseGreen();
    break;
  default:
    if (redButtonPressed)
    {
      setAllPixelsToColor();
    }
    else
    {
      spin();
    }
  }
  currentTask = 0;
}

void checkDirection()
{
  if (digitalRead(toggle) == LOW)
  {
    direction = 1;
  }
  else
  {
    direction = -1;
  }
}

bool yellowButtonPressed = false;
bool oldYellowButtonValue = false;

void checkYellowButton()
{
  if (digitalRead(yellowButton) == LOW)
  {
    yellowButtonPressed = false;
    oldYellowButtonValue = false;
  }
  else
  {
    yellowButtonPressed = true;
    if (!oldYellowButtonValue)
    {
      oldYellowButtonValue = true;

      oldTask++;
      if (oldTask > MAXTASKS)
      {
        oldTask = 1;
      }
      currentTask = oldTask;
    }
  }
}


void checkRedButton()
{
  if (digitalRead(redButton) == LOW)
  {
    redButtonPressed = false;
  }
  else
  {
    redButtonPressed = true;
  }
}


byte randomColor()
{
  return random(minColorValue, maxColorValue);
}

const int redPin = A1;
const int greenPin = A2;
const int bluePin = A3;


void setAllPixelsToColor()
{
  int redVal = analogRead(redPin);
  int greenVal = analogRead(greenPin);
  int blueVal = analogRead(bluePin);

  int setRed = map(redVal, 0, 1023, minColorValue, maxColorValue);
  int setBlue = map(blueVal, 0, 1023, minColorValue, maxColorValue);
  int setGreen = map(greenVal, 0, 1023, minColorValue, maxColorValue);

  for (int i = 0; i< NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(setRed, setGreen, setBlue));
  }

  pixels.setBrightness(globalBrightness);
  pixels.show();
}

led globalColor;

void setGlobalColorWithControls()
{
  int redVal = analogRead(redPin);
  int greenVal = analogRead(greenPin);
  int blueVal = analogRead(bluePin);

  globalColor.r = map(redVal, 0, 1023, minColorValue, maxColorValue);
  globalColor.b = map(blueVal, 0, 1023, minColorValue, maxColorValue);
  globalColor.g = map(greenVal, 0, 1023, minColorValue, maxColorValue);
}

//*******************************************
// Spin Control
//*******************************************

const int speedPin = A0;
int direction = 1;
int lastPosition = 0;
const int minSpeed = 0;
const int maxSpeed = 0;

const int slowest = 200;
const int fastest = 20;
unsigned long previousTime = 0;

void spin()
{
  setGlobalColorWithControls();
  unsigned long currentTime = millis();

  int speedVal = analogRead(speedPin);
  int moveTime = map(speedVal, 0, 1023, slowest, fastest);

  for (int i = 0; i< NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }

  pixels.setPixelColor(lastPosition, pixels.Color(globalColor.r, globalColor.g, globalColor.b));

  pixels.setBrightness(globalBrightness);
  pixels.show();

  if (currentTime - previousTime > moveTime && moveTime != slowest)
  {
    previousTime = currentTime;
    lastPosition += direction;

    if (lastPosition >= NUMPIXELS || lastPosition < 0)
    {
      lastPosition -= (NUMPIXELS * direction);
    }
  }
}



//*******************************************
// Random Colors and Spin
//*******************************************

const int ledsToLight = 8;
const int timesToSpin = 5;
const int delayDuringRandomColorsSpin = 100;

void randomColorsAndSpin()
{
  led leds[NUMPIXELS];
  for (int i = 0; i< NUMPIXELS; i++)
  {
    if (i <= ledsToLight)
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
  for (int spin = 0; spin <timesToSpin; spin++)
  {
    for (int i = 0; i< NUMPIXELS; i++)
    {
      for (int j = 0; j< NUMPIXELS; j++)
      {
        int pixel = j - i;
        if (pixel < 0)
        {
          pixel += NUMPIXELS;
        }

        pixels.setPixelColor(pixel, pixels.Color(leds[j].r, leds[j].g, leds[j].b));
      }

      pixels.setBrightness(globalBrightness);
      pixels.show();
      delay(delayDuringRandomColorsSpin);
    }
  }
}



//*******************************************
// Cycling Random Colors on all LEDS 
//*******************************************

const int totalCycles = 20;
const int timeBetweenCycles = 100;

void randomColorsForAllLeds()
{
  for (int count = 0; count < totalCycles; count++)
  {
    for (int i = 0; i< NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(randomColor(), randomColor(), randomColor()));
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
const byte maxBrightness = 30;
const byte minBrightness = 5;
const byte steps = 2;
const int stepDelay = 50;

void pulseRed()
{
  byte red = maxColorValue;
  byte green = 0;
  byte blue = 0;

  pulse(red, green, blue);
}

void pulseGreen()
{
  byte red = 0;
  byte green = maxColorValue;
  byte blue = 0;

  pulse(red, green, blue);
}

void pulseBlue()
{
  byte red = 0;
  byte green = 0;
  byte blue = maxColorValue;

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
  for (int i = 0; i < totalPulses; i++)
  {
    //Pulse out
    int currentBrightness = minBrightness;
    while (currentBrightness <= maxBrightness)
    {
      for (int i = 0; i< NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(red, green, blue));
      }
      pixels.setBrightness(currentBrightness);
      pixels.show();

      delay(stepDelay);

      currentBrightness += steps;
    }

    //Pulse in
    currentBrightness = maxBrightness;
    while (currentBrightness >= minBrightness)
    {
      for (int i = 0; i< NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(red, green, blue));
      }
      pixels.setBrightness(currentBrightness);
      pixels.show();

      delay(stepDelay);

      currentBrightness -= steps;
    }
  }
}

//*******************************************
// Quarter Spin - 4 colours (1 per quarter) and spin them
//*******************************************

void quarterSpin()
{
  led leds[NUMPIXELS];
  for (int i = 0; i< NUMPIXELS; i++)
  {
    if (i >= 0 && i < 4)
    {
      leds[i].r = maxColorValue;
      leds[i].g = 0;
      leds[i].b = 0;
    }
    if (i >= 4 && i < 8)
    {
      leds[i].r = 0;
      leds[i].g = maxColorValue;
      leds[i].b = 0;
    }
    if (i >= 8 && i < 12)
    {
      leds[i].r = 0;
      leds[i].g = 0;
      leds[i].b = maxColorValue;
    }
    if (i >= 12 && i < 16)
    {
      leds[i].r = maxColorValue;
      leds[i].g = maxColorValue;
      leds[i].b = maxColorValue;
    }
  }
  for (int spin = 0; spin <timesToSpin; spin++)
  {
    for (int i = 0; i< NUMPIXELS; i++)
    {
      for (int j = 0; j< NUMPIXELS; j++)
      {
        int pixel = j + (i * direction);
        if (pixel < 0 || pixel >= NUMPIXELS) 
        {
          pixel -= (NUMPIXELS * direction);
        }

        pixels.setPixelColor(pixel, pixels.Color(leds[j].r, leds[j].g, leds[j].b));
      }

      pixels.setBrightness(globalBrightness);
      pixels.show();
      delay(delayDuringRandomColorsSpin);
    }
  }
}
