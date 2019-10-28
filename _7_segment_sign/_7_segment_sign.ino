//RGB LED Library
#include <Adafruit_NeoPixel.h>

const byte segmentLightLength = 7;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(segmentLightLength*7, 6, NEO_RGB + NEO_KHZ800);

byte currentPrice = 3;

void displayDigit(byte price);

byte digitColour = 0;

byte lightOrder[56] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 48, 47, 46, 45, 44, 43, 42, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 48, 47, 46, 45, 44, 43, 42, 35, 36, 37, 38, 39, 40, 41};

/*
 * 7 segment display values for 0 - 9
 * +-A-+
 * F   B
 * +-G-+
 * E   C
 * +-D-+
 */
//                    {A, B, C, D, E, F, G}
const bool zero[7] =  {1, 1, 1, 1, 1, 1, 0};
const bool one[7] =   {0, 1, 1, 0, 0, 0, 0};
const bool two[7] =   {1, 1, 0, 1, 1, 0, 1};
const bool three[7] = {1, 1, 1, 1, 0, 0, 1};
const bool four[7] =  {0, 1, 1, 0, 0, 1, 1};
const bool five[7] =  {1, 0, 1, 1, 0, 1, 1};
const bool six[7] =   {1, 0, 1, 1, 1, 1, 1};
const bool seven[7] = {1, 1, 1, 0, 0, 0, 0};
const bool eight[7] = {1, 1, 1, 1, 1, 1, 1};
const bool nine[7] =  {1, 1, 1, 1, 0, 1, 1};

void setup()
{
  randomSeed(analogRead(0));
  
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  delay(250);
  
  //Initialize Strips
  strip.begin();
  strip.setBrightness(255);
}

void loop()
{
  strip.clear();

  //change the price
  changePrice();

  //display the price
  displayPrice();
  
  strip.show();
  
  //delay ms
  delay(60000 * random(10, 21)); //rnadom change between 10 and 20 minutes
}

void changePrice()
{
  changeAnimation(); //show price change animation
  digitColour = random(0, 256); //randomise the colour
  if(currentPrice == 9) {
    if(random(0, 2) == 0) { //50/50 currentPrice decreases normally or randomises
      currentPrice = random(2, 6); //a random value between 2 and 5 inclusive
    } else {
      currentPrice--;
    }
  } else if(currentPrice == 1) {
    if(random(0, 2) == 0) { //50/50 currentPrice gets increases normally or randomises
      currentPrice++;
    } else {
      currentPrice = random(5, 9); //a random value between 5 and 8 inclusive
    }
  } else {
    if(random(0, 2) == 0) { //50/50 currentPrice gets incremented or decremented 
      currentPrice++;
    } else {
      currentPrice--;
    }
  }
  /*Serial.print(digitColour);
  Serial.print(" ");
  Serial.println(currentPrice);*/
}

void displayPrice()
{
  strip.clear();
  switch (currentPrice) {
    case 0:
      paintDigit(zero);
      break;
    case 1:
      paintDigit(one);
      break;
    case 2:
      paintDigit(two);
      break;
    case 3:
      paintDigit(three);
      break;
    case 4:
      paintDigit(four);
      break;
    case 5:
      paintDigit(five);
      break;
    case 6:
      paintDigit(six);
      break;
    case 7:
      paintDigit(seven);
      break;
    case 8:
      paintDigit(eight);
      break;
    case 9:
      paintDigit(nine);
      break;
    default:
      // there was some sort of problem
      break;
  }
}

void changeAnimation()
{
  byte randSwitch = random(0, 5);
  switch(randSwitch) {
    case 0:
      segmentSpiral();
      break;
    case 1:
      segmentSpiralBackwards();
      break;
    case 2:
      rainbowDot();
      break;
    case 3:
      rainbowDotBackwards();
      break;
    case 4:
      sparkleDot();
      break;
    default:
      segmentSpiral();
      break;
  }
}

void segmentSpiral()
{
  paintSegment(Color(255,   0,   0), 0); strip.show(); delay(125); strip.clear(); //red
  paintSegment(Color(255, 127,   0), 1); strip.show(); delay(125); strip.clear(); //orange
  paintSegment(Color(255, 255,   0), 6); strip.show(); delay(125); strip.clear(); //yellow
  paintSegment(Color(  0, 255,   0), 4); strip.show(); delay(125); strip.clear(); //green
  paintSegment(Color(  0,   0, 255), 3); strip.show(); delay(125); strip.clear(); //blue
  paintSegment(Color( 75,   0, 130), 2); strip.show(); delay(125); strip.clear(); //indigo
  paintSegment(Color(148,   0, 211), 6); strip.show(); delay(125); strip.clear(); //Violet
  paintSegment(Color(255, 255, 255), 5); strip.show(); delay(125); strip.clear(); //White
}

void segmentSpiralBackwards()
{
  paintSegment(Color(255, 255, 255), 5); strip.show(); delay(125); strip.clear(); //White
  paintSegment(Color(148,   0, 211), 6); strip.show(); delay(125); strip.clear(); //Violet
  paintSegment(Color( 75,   0, 130), 2); strip.show(); delay(125); strip.clear(); //indigo
  paintSegment(Color(  0,   0, 255), 3); strip.show(); delay(125); strip.clear(); //blue
  paintSegment(Color(  0, 255,   0), 4); strip.show(); delay(125); strip.clear(); //green
  paintSegment(Color(255, 255,   0), 6); strip.show(); delay(125); strip.clear(); //yellow
  paintSegment(Color(255, 127,   0), 1); strip.show(); delay(125); strip.clear(); //orange
  paintSegment(Color(255,   0,   0), 0); strip.show(); delay(125); strip.clear(); //red
}

void paintSegment(uint32_t colour, byte segmentNumber) //where segments are 0 - 7
{
  byte startPixel = segmentNumber * 7;

  for(byte j = startPixel; j < startPixel + segmentLightLength; j++)//loop through lights in a segment and set their colour
  {
    strip.setPixelColor(j, colour);
  }
}

void rainbowDot()
{
  for(byte i = 0; i < 56; i++)//loop through lights in a segment and set their colour
  {
    strip.clear();
    strip.setPixelColor(lightOrder[i], Wheel(i * 4));
    strip.show();
    delay(20);
  }
}

void rainbowDotBackwards()
{
  for(byte i = 55; i > 0; i--)//loop through lights in a segment and set their colour
  {
    strip.clear();
    strip.setPixelColor(lightOrder[i], Wheel(i * 4));
    strip.show();
    delay(20);
  }
}

void sparkleDot()
{
  for(byte i = 0; i < 255; i++)//loop through lights in a segment and set their colour
  {
    strip.clear();
    strip.setPixelColor(random(0,50), Wheel(i));
    strip.show();
    delay(3);
  }
}
