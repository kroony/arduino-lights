//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Analog Input Pins
int left_channel = 0;

//dynamic analogue max
int leftvolmax = 1;
int updateDelay = 10;

//Set Strip Constants
const int length = 300;

byte nextcolor = 0;

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, 6, NEO_GRB + NEO_KHZ800);

void setup()
{
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  delay(250);
  //Initialize Strip
  strip.begin();
  strip.show(); 
}

void loop()
{
  //Read audio
  int leftvolume = 0;
  for(int i = 0; i < updateDelay; i++)
  {
    leftvolume += analogRead(left_channel);
    delay(1);
    Serial.print("read " + i);
  }
  leftvolume /= updateDelay;
  Serial.print("end read");
  
  leftvolume = map(leftvolume, 0, 100, 0, length);
  Serial.println(leftvolume);
  
  //Print out some Debug Info if we raise the max bounds
  if(leftvolume > leftvolmax){
    leftvolmax = leftvolume;

    Serial.print("v ");
    Serial.println(leftvolmax);
    
  }else {
    leftvolmax--;

    if(leftvolmax < 0){
      leftvolmax = 0;
    }
  }

  //left side
  for(uint16_t i=0; i<length; i++) {
      int leftbounds = map(i, 0, 300, 0, 255);
      if(i <= leftvolume)
      {
        strip.setPixelColor(i, Color(leftbounds, 255-leftbounds, 0));
        //strip.setPixelColor(half-i, Wheel(GetNextColor()));
        
      }
      else
      {
        strip.setPixelColor(i, Color(0, 0, 0));
      }

      if (i == leftvolmax){
        //strip.setPixelColor(half-i, Color(0, leftbounds, 255-leftbounds));
        strip.setPixelColor(i, Wheel(GetNextColor()));
      }
  }
  strip.show();
  
  //delay ms
  delay(500);
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



