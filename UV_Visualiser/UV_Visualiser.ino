//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Analog Input Pins
int left_channel = 0;
int right_channel = 1;

//Set Strip Constants
const int length = 300;
const byte half = length/2;

byte nextcolor = 0;

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
  //Read audio
  int leftvolume = analogRead(left_channel);
  leftvolume = map(leftvolume, 0, 300, 0, half);
  
  int rightvolume = analogRead(right_channel);
  rightvolume = map(rightvolume, 0, 300, 0, half);

  //left side
  for(uint16_t i=0; i<half; i++) {
      int leftbounds = map(i, 0, 150, 0, 255);

      if (i == leftvolume){
        strip.setPixelColor(half-i, Wheel(GetNextColor()));
      }
  }
  

  //right side
  for(uint16_t i=0; i<half; i++) {
      int rightbounds = map(i, 0, 150, 0, 255);

      if (i == rightvolume){
        strip.setPixelColor(half+i, Wheel(GetNextColor()));
      }
  }

  
  strip.show();
  
  //delay ms
  //delay(10);
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

