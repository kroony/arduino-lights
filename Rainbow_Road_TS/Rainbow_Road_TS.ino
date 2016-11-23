//RGB LED Library
#include <Adafruit_NeoPixel.h>

const byte pirPin = 3;
const byte LightDataPin = 6;
const int length = 300; //length of led strip
const byte LoopDelay = 10;//delay between 0 - 255
const byte LightBrightness = 255;//light brightness 0 - 255

bool bgColor = 0;
byte rainbowLightColour = 0;

int current = 0;
int last = 0;
bool state = true;

byte whiteCount = 0;
byte whiteCountLeft = 50;

byte spacing = 22;
byte rightColorCount1 = 0;
byte rightColorCount2 = rightColorCount1 + spacing;
byte rightColorCount3 = rightColorCount2 + spacing;
byte rightColorCount4 = rightColorCount3 + spacing;
byte rightColorCount5 = rightColorCount4 + spacing;

int leftColorCount1 = 200;
int leftColorCount2 = leftColorCount1 + spacing;
int leftColorCount3 = leftColorCount2 + spacing;
int leftColorCount4 = leftColorCount3 + spacing;
int leftColorCount5 = leftColorCount4 + spacing;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, LightDataPin, NEO_GRB + NEO_KHZ800);

void setup() {
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  
  //Initialize Strip
  strip.begin();
  strip.setBrightness(LightBrightness);
  strip.show(); 
}

void loop() {
  //check to see we have motion or not
  current = digitalRead(pirPin);
  Serial.println(current);
  
  if(current == 1 && last == 0) //if this is new motion, change the state (colours)
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
  
  //save current state as last to check against next pass
  last = current;
  
  showTSMiddle();
  whiteCount++;
  whiteCountLeft--;

  //blackout Sides
  for(byte i=0; i<100; i++)//black right
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  for(int i=200; i<300; i++)//black left
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  
  showRightSides(rightColorCount1);
  showRightSides(rightColorCount2);
  showRightSides(rightColorCount3);
  showRightSides(rightColorCount4);
  showRightSides(rightColorCount5);
  
  rightColorCount1++;
  if(rightColorCount1 > 100)  {rightColorCount1 = 0;}
  rightColorCount2++;
  if(rightColorCount2 > 100)  {rightColorCount2 = 0;}
  rightColorCount3++;
  if(rightColorCount3 > 100)  {rightColorCount3 = 0;}
  rightColorCount4++;
  if(rightColorCount4 > 100)  {rightColorCount4 = 0;}
  rightColorCount5++;
  if(rightColorCount5 > 100)  {rightColorCount5 = 0;}

  showLeftSides(leftColorCount1);
  showLeftSides(leftColorCount2);
  showLeftSides(leftColorCount3);
  showLeftSides(leftColorCount4);
  showLeftSides(leftColorCount5);
  
  leftColorCount1--;
  if(leftColorCount1 < 200)  {leftColorCount1 = 300;}
  leftColorCount2--;
  if(leftColorCount2 < 200)  {leftColorCount2 = 300;}
  leftColorCount3--;
  if(leftColorCount3 < 200)  {leftColorCount3 = 300;}
  leftColorCount4--;
  if(leftColorCount4 < 200)  {leftColorCount4 = 300;}
  leftColorCount5--;
  if(leftColorCount5 < 200)  {leftColorCount5 = 300;}
  
  //push to lights
  strip.show();
  delay(LoopDelay);
}

void setColour(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
}

void showLeftSides(int counter){
  for(byte i=0; i<5; i++)//white
  {
    if(i+counter-5 < 300 && i+counter-5 >= 200)
    {
      if(current == 1)//if currently detecting motion, rainbow the sides
      {
        strip.setPixelColor(i+counter-5, Wheel(GetNextColor()));
      }
      else
      {
        if(bgColor == 1)
        {
          strip.setPixelColor(i+counter-5, strip.Color(255, 0, 0));
        }
        else
        {
          strip.setPixelColor(i+counter-5, strip.Color(0, 255, 0));
        }
      }
    }
  }
}

void showRightSides(byte counter){
  for(byte i=0; i<5; i++)//white
  {
    if(i+counter-5 < 100 && i+counter-5 >= 0)
    {
      if(current == 1)//if currently detecting motion, rainbow the sides
      {
        strip.setPixelColor(i+counter-5, Wheel(GetNextColor()));
      }
      else
      {
        if(bgColor == 1)
        {
          strip.setPixelColor(i+counter-5, strip.Color(255, 0, 0));
        }
        else
        {
          strip.setPixelColor(i+counter-5, strip.Color(0, 255, 0));
        }
      }
    }
  }
}

void showTSMiddle(){
  for(byte i=100; i<200; i++)//blue
  {
    strip.setPixelColor(i, strip.Color(0, 0, 255));
  }
  
  //Solid Top off
  strip.setPixelColor(148, strip.Color(0, 0, 0));
  strip.setPixelColor(149, strip.Color(0, 0, 0));
  strip.setPixelColor(150, strip.Color(0, 0, 0));
  strip.setPixelColor(151, strip.Color(0, 0, 0));
  strip.setPixelColor(152, strip.Color(0, 0, 0));
  strip.setPixelColor(153, strip.Color(0, 0, 0));

  for(byte i=0; i<5; i++)//white
  {
    if(i+whiteCount+100 <= 150 && i+whiteCount+105 >= 100)
    {
      strip.setPixelColor(i+whiteCount+100, strip.Color(255, 255, 255));
    }
    
    if(whiteCount > 45)
    {
      whiteCount = 0;
    }
  }
  for(byte i=0; i<5; i++)//white
  {
    if(i+whiteCountLeft+145 <= 200 && i+whiteCountLeft+145 >= 150)
    {
      strip.setPixelColor(i+whiteCountLeft+145, strip.Color(255, 255, 255));
    }
    
    if(whiteCountLeft < 1)
    {
      whiteCountLeft = 46;
    }
  }
}

int GetNextColor()
{
  rainbowLightColour++;
  if(rainbowLightColour > 255)
  {
    rainbowLightColour = 0;
  }
  return rainbowLightColour;
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
