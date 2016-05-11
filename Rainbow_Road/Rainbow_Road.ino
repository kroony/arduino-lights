//RGB LED Library
#include <Adafruit_NeoPixel.h>

const byte pirPin = 3;
const byte LightDataPin = 6;
const int length = 300;

bool bgColor = 0;
byte travelingLight1Colour = 0;
byte travelingLight2Colour = 128;
bool lightDirection = 0;
int lightLocation = 5;

int current = 0;
int last = 0;
bool state = true;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, LightDataPin, NEO_GRB + NEO_KHZ800);

void setup() {
  
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  
  //Initialize Strip
  strip.begin();
  strip.setBrightness(100);
  strip.show(); 
}

void loop() {
  current = digitalRead(pirPin);
  Serial.println(current);
  if(current == 1 && last == 0)
  {
    if(state)
    {
      bgColor = 0;
      state = false;
    }
    else
    {
      bgColor = 1;
      state = true;
    }
  }

  last = current;
  
  
  if(bgColor == 0)
  {
    setColour(strip.Color(0, 255, 0)); //Green
  }
  else
  {
    setColour(strip.Color(0, 0, 255)); //Blue
  }
  
  //moving light
  travelingLight1Colour = GetNextColor(travelingLight1Colour);
  travelingLight2Colour = GetNextColor(travelingLight2Colour);
  
  strip.setPixelColor(lightLocation-5, strip.Color(0, 0, 0));
  strip.setPixelColor(lightLocation-4, strip.Color(0, 0, 0));
  
  strip.setPixelColor(lightLocation-3, Wheel(travelingLight2Colour));
  strip.setPixelColor(lightLocation-2, Wheel(travelingLight2Colour));
  
  strip.setPixelColor(lightLocation-1, Wheel(travelingLight1Colour));
  strip.setPixelColor(lightLocation, Wheel(travelingLight1Colour));
  strip.setPixelColor(lightLocation+1, Wheel(travelingLight1Colour));

  strip.setPixelColor(lightLocation+2, Wheel(travelingLight2Colour));
  strip.setPixelColor(lightLocation+3, Wheel(travelingLight2Colour));
  
  strip.setPixelColor(lightLocation+4, strip.Color(0, 0, 0));
  strip.setPixelColor(lightLocation+5, strip.Color(0, 0, 0));
  
  if(lightDirection == 0)
  {
    lightLocation++;
    if(lightLocation >= 295){lightDirection = 1;}
  }
  else
  {
    lightLocation--;
    if(lightLocation <= 5){lightDirection = 0;}
  }
  
  strip.show();
  delay(1);
}

void setColour(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
}

int GetNextColor(int color)
{
  color++;
  if(color > 255)
  {
    color = 0;
  }
  return color;
}

uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
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
