#include <AudioAnalyzer.h>
#include <Adafruit_NeoPixel.h>

const byte LightDataPin = 6;
const int stripLength = 300; //length of led strip
const byte LightBrightness = 100;//light brightness 0 - 255

Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, LightDataPin, NEO_GRB + NEO_KHZ800);

Analyzer Audio = Analyzer(4,5,0);//Strobe pin ->4  RST pin ->5 Analog Pin ->0
//Analyzer Audio = Analyzer();//Strobe->4 RST->5 Analog->5
int TopVolTrim = 0;
int BottomVolTrim = 100;
int maxVol = 10;
byte quietcounter = 0;

int FreqVal[7];//
byte pitch1, pitch2, pitch3, pitch4, pitch5, pitch6, pitch7; 

void setup()
{
  Serial.begin(57600);  //Init the baudrate
  Audio.Init();//Init module 

  //Initialize Light Strip
  strip.begin();
  strip.setBrightness(LightBrightness);
  strip.show(); 
  pitch1 = 0;
  pitch2 = 0;
  pitch3 = 0;
  pitch4 = 0;
  pitch5 = 0;
  pitch6 = 0;
  pitch7 = 0;
}

void loop()
{
  Audio.ReadFreq(FreqVal);//return 7 value of 7 bands pass filiter 
                          //Frequency(Hz):63  160  400  1K  2.5K  6.25K  16K
                          //FreqVal[]:      0    1    2    3    4    5    6  
  for(int i=0;i<7;i++)            
  {
    int tempFreq = max((FreqVal[i]-BottomVolTrim),0);
    byte tempTrim = map(tempFreq, 0, 1023 - TopVolTrim - BottomVolTrim, 0, 255);
    /*if(tempTrim > maxVol)
    {
      maxVol = tempTrim;
      TopVolTrim = 1023 - BottomVolTrim - maxVol;
    }
    if(tempTrim == 0) {
      quietcounter++;
      if(quietcounter == 50) {
        quietcounter = 0;
        TopVolTrim = max(TopVolTrim - 50, 0);
        maxVol = max(maxVol - 50, 0);
      }
    }
    else {
      quietcounter = 0;
    }*/
    
    Serial.print(tempFreq);//Transimit the DC value of the seven bands
    Serial.print("->");
    Serial.print(map(tempFreq, 0, 1023 - TopVolTrim - BottomVolTrim, 0, 255));//Transimit the DC value of the seven bands
    if(i<6) Serial.print(", ");
    else {
      Serial.print(", ");
      Serial.print("Max ");
      Serial.print(maxVol);
      Serial.print(" TopTrim ");
      Serial.print(TopVolTrim);
      Serial.println();
    }

    if(i==0) pitch1 = tempTrim;
    if(i==1) pitch2 = tempTrim;
    if(i==2) pitch3 = tempTrim;
    if(i==3) pitch4 = tempTrim;
    if(i==4) pitch5 = tempTrim;
    if(i==5) pitch6 = tempTrim;
    if(i==6) pitch7 = tempTrim;
  }
  
  blendOutput(0, 50, pitch1, pitch2);
  blendOutput(50, 100, pitch2, pitch3);
  blendOutput(100, 150, pitch3, pitch4);
  blendOutput(150, 200, pitch4, pitch5);
  blendOutput(200, 250, pitch5, pitch6);
  blendOutput(250, 300, pitch6, pitch7);
  
  strip.show();
  delay(20);
}

uint32_t Wheel(byte WheelPos)
{
  if (WheelPos == 0) {
    return Color(0, 0, 0);
  }
  else if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

uint32_t rgWheel(byte WheelPos)
{
  if (WheelPos == 0) {
    return Color(0, 0, 0);
  }
  else if(WheelPos < 85) {
   return Color(0, 0, WheelPos * 3);
  }
  else if (WheelPos < 170) {
    return Color((WheelPos - 170) * 3, 255, 0);
  }
  else {
    return Color(255, 255 - ((WheelPos - 170) * 3), 0);
  }
}

void blendOutput(int StartPos, int EndPos, byte StartFreq, byte EndFreq)
{
  int totalLength = EndPos - StartPos;
  for(int i=0; i<totalLength; i++)
  {
    strip.setPixelColor(StartPos + i, rgWheel(((StartFreq * (totalLength - i)) + (EndFreq * i)) / (totalLength)));
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
