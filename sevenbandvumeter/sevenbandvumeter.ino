#include <AudioAnalyzer.h>
#include <Adafruit_NeoPixel.h>

const byte LightDataPin = 6;
const int stripLength = 300; //length of led strip
const byte LightBrightness = 255;//light brightness 0 - 255

Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, LightDataPin, NEO_GRB + NEO_KHZ800);

Analyzer Audio = Analyzer(4,5,0);//Strobe pin ->4  RST pin ->5 Analog Pin ->0
//Analyzer Audio = Analyzer();//Strobe->4 RST->5 Analog->5
int BottomVolTrim = 100;
byte quietcounter = 0;

int FreqVal[7];//
byte pitch1, pitch2, pitch3, pitch4, pitch5, pitch6, pitch7 = 0; 
byte peak1, peak2, peak3, peak4, peak5, peak6, peak7 = 0;
byte bassPeak = 0;
byte history[stripLength];

byte peakColour = 0;
byte colourWheel = 0;
bool changedColour = false;

byte pattern = 0;
int patternChangeCounter = 0;

void setup()
{
  Serial.begin(57600);  //Init the baudrate
  Audio.Init();//Init audio  module 

  //Initialize Light Strip
  strip.begin();
  strip.setBrightness(LightBrightness);
  strip.show(); 
  randomSeed(analogRead(5));
}

void loop()
{
  getAudioInput(); // get the audio from the MSGEQ7
  checkForColourChange();

  bassPeak--;
  byte currentBassAverage = pitch2;
  if(bassPeak < currentBassAverage)
  {
    bassPeak = currentBassAverage + 10;
    patternChangeCounter++;
  }
  if(patternChangeCounter > 4) {//try change the pattern every 4 beats
    patternChangeCounter = 0;
    pattern = random(0, 23); //random pattern from 0 to 22
    //if(pattern == 9) {pattern = 13;} else {pattern = 9;} waterfall back and forward
    
    colourWheel = random(0, 10);//change colour when we change pattern 1 10
  }
  //pattern = 22;////FORCE TESTING!!!!
  /*
   * Pattern Map
   * 0 =  7 VU bars
   * 1 =  3 bass VU bars
   * 2 =  3 midd VU bars
   * 3 =  3 high VU bars
   * 4 = 7 pulse blocks
   * 5 = 3 bass pulse blocks
   * 6 = 3 midd pulse blocks
   * 7 = 3 high pulse blocks
   * 8 = 7 waterfall down
   * 9 = 3 bass waterfall down
   * 10 = 3 mid waterfall down
   * 11 = 3 high waterfall down
   * 12 = 7 waterfall up
   * 13 = 3 bass waterfall up
   * 14 = 3 mid waterfall up
   * 15 = 3 high waterfall up
   * 16 - 22 = VU up - block pulse - VU down - waterfall up - block pulse - waterfall down: for all 7 freq
   */
  
  switch (pattern) {
    case 0:
      peak1 = VUUp(0  , 40 , pitch1, peak1, colourWheel);
      peak2 = VUUp(41 , 80 , pitch2, peak2, colourWheel);
      peak3 = VUUp(81 , 120, pitch3, peak3, colourWheel);
      peak4 = VUUp(121, 160, pitch4, peak4, colourWheel);
      peak5 = VUUp(161, 200, pitch5, peak5, colourWheel);
      peak6 = VUUp(201, 240, pitch6, peak6, colourWheel);
      peak7 = VUUp(241, 280, pitch7, peak7, colourWheel);
      break;
    case 1:
      peak1 = VUUp(0  , 100, pitch1, peak1, colourWheel);
      peak2 = VUUp(101, 200, pitch2, peak2, colourWheel);
      peak3 = VUUp(201, 300, pitch3, peak3, colourWheel);
      break;
    case 2:
      peak3 = VUUp(0  , 100, pitch3, peak3, colourWheel);
      peak4 = VUUp(101, 200, pitch4, peak4, colourWheel);
      peak5 = VUUp(201, 300, pitch5, peak5, colourWheel);
      break;
    case 3:
      peak5 = VUUp(0  , 100, pitch5, peak5, colourWheel);
      peak6 = VUUp(101, 200, pitch6, peak6, colourWheel);
      peak7 = VUUp(201, 300, pitch7, peak7, colourWheel);
      break;
    case 4:
      BlockPulse(0  , 40 , pitch1, colourWheel);
      BlockPulse(41 , 80 , pitch2, colourWheel);
      BlockPulse(81 , 120, pitch3, colourWheel);
      BlockPulse(121, 160, pitch4, colourWheel);
      BlockPulse(161, 200, pitch5, colourWheel);
      BlockPulse(201, 240, pitch6, colourWheel);
      BlockPulse(241, 280, pitch7, colourWheel);
      break;
    case 5:
      BlockPulse(0  , 100, pitch1, colourWheel);
      BlockPulse(101, 200, pitch2, colourWheel);
      BlockPulse(201, 300, pitch3, colourWheel);
      break;
    case 6:
      BlockPulse(0  , 100, pitch3, colourWheel);
      BlockPulse(101, 200, pitch4, colourWheel);
      BlockPulse(201, 300, pitch5, colourWheel);
      break;
    case 7:
      BlockPulse(0  , 100, pitch5, colourWheel);
      BlockPulse(101, 200, pitch6, colourWheel);
      BlockPulse(201, 300, pitch7, colourWheel);
      break;
    case 8:
      WaterfallDown(0  , 40 , pitch1, colourWheel);
      WaterfallDown(41 , 80 , pitch2, colourWheel);
      WaterfallDown(81 , 120, pitch3, colourWheel);
      WaterfallDown(121, 160, pitch4, colourWheel);
      WaterfallDown(161, 200, pitch5, colourWheel);
      WaterfallDown(201, 240, pitch6, colourWheel);
      WaterfallDown(241, 280, pitch7, colourWheel);
      break;
    case 9:
      WaterfallDown(0  , 100, pitch1, colourWheel);
      WaterfallDown(101, 200, pitch2, colourWheel);
      WaterfallDown(201, 300, pitch3, colourWheel);
      break;
    case 10:
      WaterfallDown(0  , 100, pitch3, colourWheel);
      WaterfallDown(101, 200, pitch4, colourWheel);
      WaterfallDown(201, 300, pitch5, colourWheel);
      break;
    case 11:
      WaterfallDown(0  , 100, pitch5, colourWheel);
      WaterfallDown(101, 200, pitch6, colourWheel);
      WaterfallDown(201, 300, pitch7, colourWheel);
      break;
    case 12:
      WaterfallUp(0  , 40 , pitch1, colourWheel);
      WaterfallUp(41 , 80 , pitch2, colourWheel);
      WaterfallUp(81 , 120, pitch3, colourWheel);
      WaterfallUp(121, 160, pitch4, colourWheel);
      WaterfallUp(161, 200, pitch5, colourWheel);
      WaterfallUp(201, 240, pitch6, colourWheel);
      WaterfallUp(241, 280, pitch7, colourWheel);
      break;
    case 13:
      WaterfallUp(0  , 100, pitch1, colourWheel);
      WaterfallUp(101, 200, pitch2, colourWheel);
      WaterfallUp(201, 300, pitch3, colourWheel);
      break;
    case 14:
      WaterfallUp(0  , 100, pitch3, colourWheel);
      WaterfallUp(101, 200, pitch4, colourWheel);
      WaterfallUp(201, 300, pitch5, colourWheel);
      break;
    case 15:
      WaterfallDown(0  , 100, pitch5, colourWheel);
      WaterfallDown(101, 200, pitch6, colourWheel);
      WaterfallDown(201, 300, pitch7, colourWheel);
      break;
    case 16:
      WaterfallDown(          0  , 50 , pitch1, colourWheel);
      BlockPulse(             51 , 100, pitch1, colourWheel);
      WaterfallUp(            101, 150, pitch1, colourWheel);
      peak1 = VUDown(151, 200, pitch1, peak1, colourWheel);
      BlockPulse(             201, 250, pitch1, colourWheel);
      peak1 = VUUp(251, 300, pitch1, peak1, colourWheel);
      break;
    case 17:
      WaterfallDown(          0  , 50 , pitch2, colourWheel);
      BlockPulse(             51 , 100, pitch2, colourWheel);
      WaterfallUp(            101, 150, pitch2, colourWheel);
      peak2 = VUDown(151, 200, pitch2, peak2, colourWheel);
      BlockPulse(             201, 250, pitch2, colourWheel);
      peak2 = VUUp(251, 300, pitch2, peak2, colourWheel);
      break;
    case 18:
      WaterfallDown(          0  , 50 , pitch3, colourWheel);
      BlockPulse(             51 , 100, pitch3, colourWheel);
      WaterfallUp(            101, 150, pitch3, colourWheel);
      peak3 = VUDown(151, 200, pitch3, peak3, colourWheel);
      BlockPulse(             201, 250, pitch3, colourWheel);
      peak3 = VUUp(251, 300, pitch3, peak3, colourWheel);
      break;
    case 19:
      WaterfallDown(          0  , 50 , pitch4, colourWheel);
      BlockPulse(             51 , 100, pitch4, colourWheel);
      WaterfallUp(            101, 150, pitch4, colourWheel);
      peak4 = VUDown(151, 200, pitch4, peak4, colourWheel);
      BlockPulse(             201, 250, pitch4, colourWheel);
      peak4 = VUUp(251, 300, pitch4, peak4, colourWheel);
      break;
    case 20:
      WaterfallDown(          0  , 50 , pitch5, colourWheel);
      BlockPulse(             51 , 100, pitch5, colourWheel);
      WaterfallUp(            101, 150, pitch5, colourWheel);
      peak5 = VUDown(151, 200, pitch5, peak5, colourWheel);
      BlockPulse(             201, 250, pitch5, colourWheel);
      peak5 = VUUp(251, 300, pitch5, peak5, colourWheel);
      break;
    case 21:
      WaterfallDown(          0  , 50 , pitch6, colourWheel);
      BlockPulse(             51 , 100, pitch6, colourWheel);
      WaterfallUp(            101, 150, pitch6, colourWheel);
      peak6 = VUDown(151, 200, pitch6, peak6, colourWheel);
      BlockPulse(             201, 250, pitch6, colourWheel);
      peak6 = VUUp(251, 300, pitch6, peak6, colourWheel);
      break;
    case 22:
      WaterfallDown(          0  , 50 , pitch7, colourWheel);
      BlockPulse(             51 , 100, pitch7, colourWheel);
      WaterfallUp(            101, 150, pitch7, colourWheel);
      peak7 = VUDown(151, 200, pitch7, peak7, colourWheel);
      BlockPulse(             201, 250, pitch7, colourWheel);
      peak7 = VUUp(251, 300, pitch7, peak7, colourWheel);
      break;
    default:
      pattern = 0;
      break;
  }
     
  strip.show();
  delay(10);
  ClearStrip();
}

void ClearStrip(){
  for(int i = 0;i<stripLength;i++)
  {
    strip.setPixelColor(i, Color(0,0,0));
  }
}

void getAudioInput()
{
  Audio.ReadFreq(FreqVal);//return 7 value of 7 bands pass filiter 
                          //Frequency(Hz):63  160  400  1K  2.5K  6.25K  16K
                          //FreqVal[]:     0    1    2   3     4      5    6  
  for(int i=0;i<7;i++)            
  {
    int tempFreq = max((FreqVal[i]-BottomVolTrim),0);
    byte tempTrim = map(tempFreq, 0, 1023 - BottomVolTrim, 0, 255);
    
    /*Serial.print(tempFreq);//Transimit the DC value of the seven bands
    Serial.print("->");
    Serial.print(map(tempFreq, 0, 1023 - BottomVolTrim, 0, 255));//Transimit the DC value of the seven bands
    if(i<6) Serial.print(", ");
    else {
      Serial.println();
    }*/
     switch (i) {
      case 0: pitch1 = tempTrim; break;
      case 1: pitch2 = tempTrim; break;
      case 2: pitch3 = tempTrim; break;
      case 3: pitch4 = tempTrim; break;
      case 4: pitch5 = tempTrim; break;
      case 5: pitch6 = tempTrim; break;
      case 6: pitch7 = tempTrim; break;
     }
  }
}

void checkForColourChange()
{
  //change colour wheel when there is no sound
  if (pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7 < 2 && changedColour == false) {
    colourWheel++;
    changedColour = true;
    if(colourWheel > 9) colourWheel = 0;
  }
  else if (pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7 < 2 && changedColour == true) {
    //do nothing
  }
  else changedColour = false;
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
