#include <Adafruit_NeoPixel.h>

//Set Strip Constants
const int length = 5;
const byte brightness = 255;

byte nextcolor = 0;
uint32_t spinnerColor = 0;
byte spinnerLocation = 0;

byte spinnerBrightness = 0;
byte spinnerTail = 11;

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, 6, NEO_GRB + NEO_KHZ800);

void setup()
{
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  
  //Initialize Strip
  strip.begin();
  strip.show(); 
}

void loop()
{
  if(spinnerBrightness == brightness)
  {
    spinnerLocation++;
    spinnerTail++;
    spinnerBrightness = 0;

    if(spinnerLocation > 11)
    {
      spinnerLocation = 0;
    }
    else if(spinnerTail > 11)
    {
      spinnerTail = 0;
      spinnerColor = nextcolor;
    }
  }

  strip.clear();

  strip.setPixelColor(spinnerLocation, WheelBrightness(spinnerColor, spinnerBrightness));
  strip.setPixelColor(spinnerTail, WheelBrightness(spinnerColor, brightness-spinnerBrightness));
  spinnerBrightness++;

  //show static rainbow LED at 0
  strip.setPixelColor(0, WheelBrightness(GetNextColor(),brightness));

  //push
  strip.show();
  //delay ms
  delay(1);
}

int GetNextColor()
{
  nextcolor++;
  if(nextcolor > 255)
  {
    nextcolor = 0;
  }
  return nextcolor;
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

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
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


//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t WheelBrightness(byte WheelPos, byte brightness)
{
  if (WheelPos < 85) {
   return Color(brightness*(WheelPos * 3)/255, 
                brightness*(255 - WheelPos * 3)/255, 
                0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(brightness*(255 - WheelPos * 3)/255, 
                0, 
                brightness*(WheelPos * 3)/255);
  } else {
   WheelPos -= 170; 
   return Color(0, 
                brightness*(WheelPos * 3)/255, 
                brightness*(255 - WheelPos * 3)/255);
  }
}

