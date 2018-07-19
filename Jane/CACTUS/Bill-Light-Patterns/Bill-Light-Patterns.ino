#include <Adafruit_NeoPixel.h>

const byte LightDataPin = 6;
const int stripLength = 90;"? ,-0l;.
const byte frameDelay = 50;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, LightDataPin, NEO_GRB + NEO_KHZ800);

byte history[stripLength];//what was display last frame
bool strobeCurrent;
byte SingleColorChaseCounter = 0;
byte DoubleColorChaseCounter = 0;
byte TrippleColorChaseCounter = 0;

int switchCounter = 0;
byte activePattern = 0;
uint32_t RandomColor1 = 0;
uint32_t RandomColor2 = 0;
uint32_t RandomColor3 = 0;

void setup()
{
  Serial.begin(57600);  //Init the baudrate

  //Initialize Light Strip
  strip.begin();
  strip.setBrightness(255);//set brightness to max
  strip.show(); 

  randomSeed(analogRead(5)); //seed the random function

  colorWipe(Color(255, 0, 0));//yea, nah, nah //RED TEST
  colorWipe(Color(0, 255, 0));//nah, yea, nah //GREEN TEST
  colorWipe(Color(0, 0, 255));//nah, nah, yea //BLUE TEST
  
  delay(500);
  randomiseColors();//give the colors an initial colour
}

void loop()
{
  switchCounter++;
  if(switchCounter == 100) 
  {
    switchCounter = 0;
    activePattern++;
  }

  if(activePattern == 0)
  {
    AlternateColors(0, 101, RandomColor1, RandomColor2);
  }
  else if(activePattern == 1)
  {
    ChaseColors(0, 101, RandomColor1, RandomColor2);
  }
  else if(activePattern == 2)
  {
    ChaseColorGapDown(0, 101, RandomColor1);
  }
  else if(activePattern == 3)
  {
    ChaseColorGapUp(0, 101, RandomColor1);
  }
  else if(activePattern == 4)
  {
    ChaseColorGapDown2(0, 101, RandomColor1, RandomColor2);
  }
  else if(activePattern == 5)
  {
    ChaseColorGapUp2(0, 49, RandomColor1, RandomColor2);
  }
  else if(activePattern == 6)
  {
    ChaseColorGapDown3(0, 101, RandomColor1, RandomColor2, RandomColor3);
  }
  else if(activePattern == 7)
  {
    ChaseColorGapUp3(0, 101, RandomColor1, RandomColor2, RandomColor3);
  }
  else if(activePattern == 8)
  {
    WaterfallUp(0, 49, random(256)); //chaos mode Up
  }
  else if(activePattern == 9)
  {
    WaterfallDown(0, 101, random(256)); //chaos mode Down
  }
  else if(activePattern == 10)
  {
    WaterfallRandom(0, 101, random(256));
  }
  else
  {
    activePattern = 0;
    randomiseColors();
  }
  
  strip.show();//push the array to the lights
  ClearStrip();//set the strip to all off
  delay(frameDelay);
  strobeCurrent = !strobeCurrent; //switch counter for strobe like functions
}

void randomiseColors()
{
  RandomColor1 = Wheel(random(256));
  RandomColor2 = Wheel(random(256));
  RandomColor3 = Wheel(random(256));
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
