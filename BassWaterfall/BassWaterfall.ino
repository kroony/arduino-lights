//RGB LED Library
#include <Adafruit_NeoPixel.h>
#include <AudioAnalyzer.h>

//Analog Input Pins
int aux = 0;
Analyzer Audio = Analyzer(4,5,aux);//Strobe pin ->4  RST pin ->5 Analog Pin ->0
//Analyzer Audio = Analyzer();//Strobe->4 RST->5 Analog->5
int FreqVal[7];//

//Set Strip Constants
const int length = 300;

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, 6, NEO_GRB + NEO_KHZ800);

//Set up arrays for cycling through all the pixels.  I'm assuming you have an even number of lights.
int light_array[length];

void setup()
{
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  Audio.Init();//Init module 

  Serial.println("ON");
  
  //Fill pixel arrays with zeros
  for(int i=0; i<length;i++)
  {
    light_array[i] = 0;
    //right_array[i] = 0;
  }
  
  //Initialize Strip
  strip.begin();
  strip.show(); 
}

void loop()
{
  Audio.ReadFreq(FreqVal);//return 7 value of 7 bands pass filiter 
                          //Frequency(Hz):63  160  400  1K  2.5K  6.25K  16K
                          //FreqVal[]:      0    1    2    3    4    5    6  
  //Read audio
  int Read = max((FreqVal[1]-100),0);
  Serial.println(Read);
  int mappedinput = Read;
  //Print out some Debug Info if we raise the max bounds
  /*if(Read > maxinput){
    maxinput = Read;

    Serial.print("In+ ");
    Serial.println(maxinput);  
  }*/

  //Print out some Debug Info if we lower the min bounds
  /*if(Read < mininput)
  {
    mininput = Read;

    Serial.print("In- ");
    Serial.println(mininput);
  }
  
  if(Read <= mininput){//if audio is flat for 10 loops, drop the max input
    resetlagcount++;
    if(resetlagcount >= 25){
      resetlagcount = 0;
      mininput = 0;
      //maxinput = 1;
      Serial.println("Reset");//its been reset
    }
  } else{
    resetlagcount = 0;
  }*/

  mappedinput = constrain(map(Read, 0, 823, 0, 1023), 0, 1023);
  
  /*Serial.print("Raw ");
  Serial.print(Read);
  Serial.print(" Min:");
  Serial.print(mininput);
  Serial.print(" Max:");
  Serial.print(maxinput);
  Serial.print(" Map:");
  Serial.println(mappedinput);*/
  
  //Set the hue (0-1023) and 24-bit color depending on left channel value
  int hue = mappedinput;
  
  //Shift the current values.
  for (int i = 0; i<length-1; i++ )
  {
    light_array[i] = light_array[i+1];
  }
  
  //Fill in the new value at the end of each array
  light_array[length-1] = hue;
  
  //Go through each Pixel on the strip and set its color
  for (int i=0; i<length; i++)
  {
    //set pixel color      /*rgWheel*/
    strip.setPixelColor(i, rgWheel(light_array[i]));
  }

  //display new frame on lights
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

uint32_t bgWheel(byte WheelPos)
{
  if(WheelPos < 127) {
   return Color(0, WheelPos, 255);
  }
  else {
    return Color(0, 255, 255 - WheelPos);
  }
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(int WheelPos)
{
  byte quart = map(WheelPos, 0, 1023, 0, 255);
  if (WheelPos == 0){
    return Color(0, 0, 0);
  } else if (WheelPos <= 340) {
   return Color(quart * 3      , 255 - quart * 3, 0);
  } else if (WheelPos <= 680) {
   WheelPos -= 340;
   quart = map(WheelPos, 0, 1023, 0, 255);
   return Color(255 - quart * 3, 0              , quart * 3);
  } else {
   WheelPos -= 680; 
   quart = map(WheelPos, 0, 1023, 0, 255);
   return Color(0              , quart * 3      , 255 - quart * 3);
  }
}

uint32_t rgWheel(byte WheelPos)
{
  if (WheelPos == 0) {
    return Color(0, 0, 0);
  }
  else if(WheelPos < 85) {
   return Color(0, WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    return Color((WheelPos - 170) * 3, 255, 0);
  }
  else {
    return Color(255, 255 - ((WheelPos - 170) * 3), 0);
  }
}
