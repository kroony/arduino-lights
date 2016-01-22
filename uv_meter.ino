//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Analog Input Pins
int left_channel = 0;
int right_channel = 1;

//dynamic analogue max
int volmax = 1;

//Set Strip Constants
const int length = 300;
const byte half = length/2;

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
  int volume = (analogRead(left_channel) + analogRead(right_channel)) / 2;
  volume = map(volume, 0, 800, 0, length);
  //Print out some Debug Info if we raise the max bounds
  if(volume > volmax){
    volmax = volume;

    Serial.print("V ");
    Serial.println(volmax);
    
  }else {
    volmax--;

    if(volmax < 0){
      volmax = 0;
    }
  }

  for(uint16_t i=0; i<length; i++) {
      if(i <= volume)
      {
        //strip.setPixelColor(i, Wheel(constrain(map(i, 0, 800, 0, length), 0, length)));
        strip.setPixelColor(i, Color(i, 255-i, 0));
      }
      else
      {
        strip.setPixelColor(i, Color(0, 0, 0));
      }

      if (i == volmax){
        strip.setPixelColor(i, Color(0, i, 255-i));
      }
  }
  strip.show();
  
  //delay ms
  delay(10);
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
  if (WheelPos == 0){
    return Color(0, 0, 0);
  } else if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

