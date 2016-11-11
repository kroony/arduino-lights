#include <Adafruit_NeoPixel.h>

//Set Strip Constants
const int length = 12;
const byte brightness = 25;

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, 6, NEO_GRB + NEO_KHZ800);

void setup()
{
  //seed random
  randomSeed(analogRead(0));
  
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  
  //Initialize Strip
  strip.begin();
  strip.setBrightness(brightness);
  strip.show(); 
}

void loop()
{
  //solid red
  setColor(strip.Color(255, 0, 0)); // Red
  delay(750);
  
  //red wipe
  colorWipe(strip.Color(0, 0, 0), 150); // Red
  
  //solid yellow
  setColor(strip.Color(255, 200, 0)); // yellow
  delay(750);
  
  //yellow wipe
  colorWipe(strip.Color(0, 0, 0), 150); // yellow

  //random delay
  delay(random(500, 1500));
  
  //green
  setColor(strip.Color(0, 255, 0)); // green
  delay(100);
  setColor(strip.Color(0, 0, 0)); // off
  delay(100);
  setColor(strip.Color(0, 255, 0)); // green
  delay(100);
  setColor(strip.Color(0, 0, 0)); // off
  delay(100);
  setColor(strip.Color(0, 255, 0)); // green
  delay(100);
  setColor(strip.Color(0, 0, 0)); // off
  delay(100);
  setColor(strip.Color(0, 255, 0)); // green
  delay(100);
  setColor(strip.Color(0, 0, 0)); // off
  delay(100);
  setColor(strip.Color(0, 255, 0)); // green
  delay(100);
  setColor(strip.Color(0, 0, 0)); // off
  delay(100);
  setColor(strip.Color(0, 255, 0)); // green
  delay(100);
  setColor(strip.Color(0, 0, 0)); // off
  delay(100);
  setColor(strip.Color(0, 255, 0)); // green
  delay(100);
  setColor(strip.Color(0, 0, 0)); // off
  delay(100);
  setColor(strip.Color(0, 255, 0)); // green
  delay(100);
  setColor(strip.Color(0, 0, 0)); // off
  delay(100);
  setColor(strip.Color(0, 255, 0)); // green
  delay(100);
  
}

void setColor(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
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

