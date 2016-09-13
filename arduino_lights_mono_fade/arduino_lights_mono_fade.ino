//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Analog Input Pins
int left_channel = 0;

//dynamic analogue max
int leftmaxinput = 1;
byte leftmaxresetlagcount = 0;
int flashdifference = 40;

//Set Strip Constants
const int length = 300;
const bool bassFlash = 0;
//const byte half = length/2;

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, 6, NEO_GRB + NEO_KHZ800);

//Set up arrays for cycling through all the pixels.  I'm assuming you have an even number of lights.
byte left_array[length];
//byte right_array[half];

//unsigned int left_array[half];
//unsigned int right_array[half];

void setup()
{
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  
  //Fill pixel arrays with zeros
  for(int i=0; i<length;i++)
  {
    left_array[i] = 0;
    //right_array[i] = 0;
  }
  
  //Initialize Strip
  strip.begin();
  strip.show(); 
}

void loop()
{
  //Read audio
  int leftRead = analogRead(left_channel);
  
  //Print out some Debug Info if we raise the max bounds
  if(leftRead > leftmaxinput){
    leftmaxinput = leftRead;

    Serial.print("L ");
    Serial.println(leftmaxinput);  
  }
  
  if(leftRead == 0){//if audio is flat for 10 loops, drop the max input
    leftmaxresetlagcount++;
    if(leftmaxresetlagcount >= 20 && leftmaxinput != 10){
      leftmaxresetlagcount = 0;
      leftmaxinput = 10;
      Serial.println("VL");//V for down arrow, its been reset
    }
  } else{
    leftmaxresetlagcount = 0;
  }
  
  if(leftmaxinput < 1){
    leftmaxinput = 1;
  }


  if(bassFlash)
  {
    if(leftRead > leftmaxinput/10*8){
      flashLeft(Wheel(constrain(map(leftRead, leftmaxinput/10*8, leftmaxinput, 0, 255), 0, 255)));
      //delay(10);
    }
  }
  
  //Set the hue (0-1023) and 24-bit color depending on left channel value
  int hue_left = constrain(map(leftRead, 0, leftmaxinput, 0, 255), 0, 255);
  
  //Shift the current values.
  for (int i = 0; i<length-1; i++)
  {
    if(left_array[i+1] == 0)
    {
      left_array[i] = left_array[i+1]; 
    }
    else
    {
      left_array[i] = left_array[i+1]-1; 
    }
  }
  
  //Fill in the new value at the end of each array
  left_array[length-1] = hue_left;
  //right_array[half-1] = hue_right;
  
  //Go through each Pixel on the strip and set its color
  for (int i=0; i<length; i++)
  {
    //set pixel color
    strip.setPixelColor(i, Wheel(left_array[i]));
    //strip.setPixelColor(length-i-1, Wheel(right_array[i]));
  }

  //display new frame on lights
  strip.show();
  
  //delay ms
  //delay(10);
}

void flashLeft(uint32_t c) {
  for(uint16_t i=0; i<length; i++) {
      strip.setPixelColor(i, c);
  }
  strip.show();
}

void flashRight(uint32_t c) {
  for(uint16_t i=0; i<length; i++) {
      strip.setPixelColor(length-i-1, c);
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
