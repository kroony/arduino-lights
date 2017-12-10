//RGB LED Library
#include <Adafruit_NeoPixel.h>


const byte LightDataPin = 6;
const int stripLength = 300; //length of led strip
const byte LightBrightness = 255;//light brightness 0 - 255

int dotDuration = 100;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, LightDataPin, NEO_GRB + NEO_KHZ800);

void setup() {
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  
  //Initialize Strip
  strip.begin();
  strip.setBrightness(LightBrightness);
  strip.show(); 
  
}

void pause()
{
  delay(dotDuration);
}

void letterSpace()
{
  pause();
  pause();
  pause();
}

void wordSpace()//only has to be 4 pause cause each leter already has 3 after it making 7 all up
{
  pause();
  pause();
  pause();
  pause();
}

void dot()
{
  //set strip to red for 1 delay
  for(int i=0; i<stripLength; i++)
  {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
  pause();
  clearStrip();
  letterSpace();
}

void dash()
{
  //set strip to red for 1 delay
  for(int i=0; i<stripLength; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  strip.show();
  letterSpace();//pause for 3 intervalls for a dash
  clearStrip();
  letterSpace();
}

void clearStrip()//no delays
{
  for(int i=0; i<stripLength; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

void MorseF() //..-.
{
  dot(); dot(); dash(); dot();
  letterSpace();
}

void MorseU() //..-
{
  dot(); dot(); dash();
  letterSpace();
}

void MorseC() //-.-.
{
  dash(); dot(); dash(); dot();
  letterSpace();
}

void MorseK() //-.-
{
  dash(); dot(); dash();
  letterSpace();
}

void MorseH() //....
{
  dot(); dot(); dot(); dot();
  letterSpace();
}

void MorseR() //.-.
{
  dot(); dash(); dot();
  letterSpace();
}

void MorseI() //..
{
  dot(); dot();
  letterSpace();
}

void MorseS() //...
{
  dot(); dot(); dot();
  letterSpace();
}

void MorseT() //-
{
  dash();
  letterSpace();
}

void MorseM() //--
{
  dash(); dash();
  letterSpace();
}

void MorseA() //.-
{
  dot(); dash();
  letterSpace();
}


void loop() {
  Serial.print('f');
  MorseF();
  Serial.print('u');
  MorseU();
  Serial.print('c');
  MorseC();
  Serial.print('k');
  MorseK();
  Serial.print(' ');
  wordSpace();
  Serial.print('c');
  MorseC();
  Serial.print('h');
  MorseH();
  Serial.print('r');
  MorseR();
  Serial.print('i');
  MorseI();
  Serial.print('s');
  MorseS();
  Serial.print('t');
  MorseT();
  Serial.print('m');
  MorseM();
  Serial.print('a');
  MorseA();
  Serial.println('s');
  MorseS();
  wordSpace();
  wordSpace();
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
