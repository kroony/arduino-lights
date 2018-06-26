#include <Wire.h>
#include "Adafruit_TCS34725.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, PIN, NEO_RGB + NEO_KHZ800);
//Adafruit_TCS34725 tcs = Adafruit_TCS34725();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  delay(100);
  
  randomSeed(analogRead(0));
  if (tcs.begin()) {
    //found sensor
    setColor(Color(0,255,0)); strip.show(); delay(250);
    setColor(Color(0,0,0)); strip.show(); delay(250);
    setColor(Color(0,255,0)); strip.show(); delay(250);
    setColor(Color(0,0,0)); strip.show(); delay(250);
  } else {
    //No TCS34725 found ... check your connections;
    setColor(Color(255,0,0)); strip.show(); delay(250);
    setColor(Color(0,0,0)); strip.show(); delay(250);
    setColor(Color(255,0,0)); strip.show(); delay(250);
    setColor(Color(0,0,0)); strip.show(); delay(250);
  }

  //test the lights on startup
  colorWipe(Color(255, 0, 0), 10); //red
  delay(100);
  colorWipe(Color(0, 255, 0), 10); //green
  delay(100);
  colorWipe(Color(0, 0, 255), 10); //blue
  delay(100);
  colorWipe(Color(255, 255, 255), 20); //white
  delay(1000);
  rainbow(1);
  colorWipe(Color(0, 0, 0), 5); //white

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  delay(1000);
}

void loop() {

  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  int red, green, blue;
  
  if(r > 100 || g > 100 || b > 100)
  {
    /*Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
    Serial.println(" ");*/
    delay(100);
    int highest = r;
    if(g > highest) { highest = g; }
    if(b > highest) { highest = b; }
    red   = map(r, 100, highest, 0, 255);
    green = map(g, 100, highest, 0, 255);
    blue  = map(b, 100, highest, 0, 255);

    //colorWipe(Color(red, green, blue), 20);
    throbCock(Color(red, green, blue));
  }
  else
  {
    if(random(0, 10000) > 9998)
    {
      strip.setPixelColor(random(49), Color(random(255),random(255),random(255))); 
      strip.show(); 
      delay(random(100,500));
      if(random(100) > 98)
      {
        rainbow(1);
      }

      for (int i = 0; i < 50; i++) { //make shaft solid
        strip.setPixelColor(i, Color(0,0,0));
      }
    }
  }

  

  strip.show();
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


void throbCock(uint32_t ThrobColor) {
  int counter = 0;
  digitalWrite(4, LOW);
  
  for (byte throbWait = random(10, 20); throbWait > 1; throbWait--)//warmup Pump
  {
    Serial.println(throbWait);
    for (int j = 0; j < 256; j++)//bright
    {
      for (int i = 0; i < 45; i++) {
        strip.setPixelColor(i, ThrobColor);
      }
      strip.setBrightness(j);
      strip.show();
      delay(throbWait);
    }

    strip.setBrightness(255);
    for (int i = 0; i < 45; i++) {
      strip.setPixelColor(i, ThrobColor);
    }
  
    for (int j = 255; j > -1; j--)//dim
    {
      strip.setBrightness(j);
      strip.show();
      delay(throbWait);
    }
  }

  for (byte throbWait = random(10, 20); throbWait > 1; throbWait--)//quick Pump
  {
    Serial.println(throbWait);
    for (int j = 0; j < 256; j++)//bright
    {
      strip.setBrightness(j);
      for (int i = 0; i < 45; i++) {
        strip.setPixelColor(i, ThrobColor);
      }
      strip.show();
    }

    strip.setBrightness(255);
    for (int i = 0; i < 45; i++) {
      strip.setPixelColor(i, ThrobColor);
    }
    strip.setPixelColor(random(0, 44), randomColor());
    for (int j = 255; j > -1; j--)//dim
    {
      strip.setBrightness(j);
      strip.show();
    }
  }

  //Jizz
  strip.setBrightness(255);
  for (int i = 0; i < 45; i++) { //make shaft solid
    strip.setPixelColor(i, ThrobColor);
  }
  uint32_t white = Color(255,255,255);
  uint32_t black = Color(0,0,0);
  
  strip.setPixelColor(45, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(46, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(48, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(45, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(46, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(48, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, black); strip.show(); delay(500); strip.setPixelColor(random(0, 44), randomColor());

  strip.setPixelColor(45, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(46, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(48, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, white); strip.setPixelColor(45, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(46, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(48, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, black); strip.show(); delay(500); strip.setPixelColor(random(0, 44), randomColor());

  strip.setPixelColor(45, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(46, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(48, white); strip.setPixelColor(45, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, white); strip.setPixelColor(46, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(48, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, black); strip.show(); delay(500); strip.setPixelColor(random(0, 44), randomColor());

  strip.setPixelColor(45, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(46, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, white); strip.setPixelColor(45, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(48, white); strip.setPixelColor(46, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, white); strip.setPixelColor(47, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(48, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, black); strip.show(); delay(500); strip.setPixelColor(random(0, 44), randomColor());

  strip.setPixelColor(45, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(46, white); strip.setPixelColor(45, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, white); strip.setPixelColor(46, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(48, white); strip.setPixelColor(47, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, white); strip.setPixelColor(48, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(49, black); strip.show(); delay(500); strip.setPixelColor(random(0, 44), randomColor());

  strip.setPixelColor(44, white);  strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(45, white); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(46, white); strip.setPixelColor(45, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, white); strip.setPixelColor(46, black); strip.show(); delay(50); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(47, black); strip.show(); delay(1000); strip.setPixelColor(random(0, 44), randomColor());
  strip.setPixelColor(43, white); strip.show(); delay(1000); strip.setPixelColor(random(0, 44), randomColor());

  //shuffle remove
  int randomIndexArray[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44};
  const size_t n = sizeof(randomIndexArray) / sizeof(randomIndexArray[0]);

  for (size_t i = 0; i < n - 1; i++) {
      size_t j = random(0, n - i);
      int t = randomIndexArray[i];
      randomIndexArray[i] = randomIndexArray[j];
      randomIndexArray[j] = t;
  }
  for (int i = 0; i < 45; i++) { //make shaft random
    strip.setPixelColor(randomIndexArray[i], randomColor());
    strip.show();
    delay(50);
  }
  for (int i = 0; i < 45; i++) { //make shaft black
    strip.setPixelColor(randomIndexArray[i], black);
    strip.show();
    delay(50);
  }
  digitalWrite(4, HIGH);
  delay(1000);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

uint32_t randomColor() {
  return Color(random(255),random(255),random(255));
}

void setColor(uint32_t color)
{
  for (int i = 0; i < 49; i++) {
    strip.setPixelColor(i, color);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


