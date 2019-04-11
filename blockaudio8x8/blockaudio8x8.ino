#include <AudioAnalyzer.h>
#include <Adafruit_NeoPixel.h>

//PIN Definition
const byte LightDataPin = 6;
const byte BrightnessPotPin = 2;

const int stripLength = 200; //length of led strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, LightDataPin, NEO_GRB + NEO_KHZ800);

Analyzer Audio = Analyzer(4,5,0);//Strobe pin ->4  RST pin ->5 Analog Pin ->0
//Analyzer Audio = Analyzer();//Strobe->4 RST->5 Analog->5
int BottomVolTrim = 100;
byte quietcounter = 0;

bool debug = true;

int FreqVal[7];//
byte pitch1, pitch2, pitch3, pitch4, pitch5, pitch6, pitch7 = 0; 
byte peak1, peak2, peak3, peak4, peak5, peak6, peak7, peak8 = 0;
byte bassPeak = 0;
byte history[stripLength];
byte LightBrightness = 0;//light brightness 0 - 255, read from POT on analogue 2

byte peakColour = 0; // universal colour for all VU peaks
byte colourWheel = 0;
bool changedColour = false;

int patternChangeCounter = 0;

bool waterfall = 0;
bool VU = 0;
bool pulse = 0;
bool strobe = 0;
bool bloompulse = 0;
bool twinkle = 0;
bool twinklePaint = 0;

bool strobeCurrent = 0;
byte strobeCounter = 0;

bool directionUp = false;
byte randomFreq;

bool passiveMode = true;
byte passiveGoalColour = 255;
byte passiveColour = 0;
int passiveSilenceCounter = 0;
byte strobeBuildupCounter = 0;

void setup()
{
  Serial.begin(57600);  //Init the baudrate
  Audio.Init();//Init audio  module 

  //Initialize Light Strip
  strip.begin();
  //SetBrightness();
  strip.setBrightness(128);
  strip.show(); 
  
  randomSeed(analogRead(5)); //seed the random function
  changeColour(); //start with a random colour

  //Run LED test
  /*colorWipe(strip.Color(255, 0, 0)); // Red
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Off
  
  colorWipe(strip.Color(0, 255, 0)); // Green
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Off
  
  colorWipe(strip.Color(0, 0, 255)); // Blue
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Off*/
  
  /*colorWipe(strip.Color(255, 255, 255)); // White
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Off*/

  displayGhost();

  //initalise all randoms
  randomStart();
  changePatternVU();
}

void loop()//bloompulse not used anywhere yet!
{
  //set brightness
  SetBrightness();
  
  getAudioInput(); // get the audio from the MSGEQ7
  checkForPassive(); //checks for no audio 
  

  
  //changePatternVU();
  //changePatternPulse();
  //changePatternWaterfall();
  //changePatternStrobe();
  //changePatternBloomPulse();
  //changePatternTwinkle();
  //changePatternTwinklePaint();
  
  
  if(passiveMode){
    displayPassive();
  } else {
    beatDetection(); // changes pattern
    
    //pick pattern based on bools
    if(waterfall == 1){
      if(directionUp) {
        WaterfallDown(0 , 7 , pitch7, colourWheel);
        WaterfallUp  (8 , 15, pitch6, colourWheel);
        WaterfallDown(16, 23, pitch5, colourWheel);
        WaterfallUp  (24, 31, pitch4, colourWheel);
        WaterfallDown(32, 39, pitch3, colourWheel);
        WaterfallUp  (40, 47, pitch2, colourWheel);
        WaterfallDown(48, 55, (pitch2 + pitch1) / 2, colourWheel);
        WaterfallUp  (56 ,63, pitch1, colourWheel);
      } else {
        WaterfallUp  (0 , 7 , pitch7, colourWheel);
        WaterfallDown(8 , 15, pitch6, colourWheel);
        WaterfallUp  (16, 23, pitch5, colourWheel);
        WaterfallDown(24, 31, pitch4, colourWheel);
        WaterfallUp  (32, 39, pitch3, colourWheel);
        WaterfallDown(40, 47, pitch2, colourWheel);
        WaterfallUp  (48, 55, (pitch2 + pitch1) / 2, colourWheel);
        WaterfallDown(56 ,63, pitch1, colourWheel);
      }
    } else if (pulse == 1) {
      switch(randomFreq){
        case 1: 
          BlockPulse(0, stripLength - 1, (pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7) / 7, colourWheel); //overall total volume single block pulse. simple as it gets
          break;
        case 2:
          BlockPulse(0 , 31, (pitch4 + pitch5 + pitch6 + pitch7) / 4, colourWheel);
          BlockPulse(32, 63, (pitch1 + pitch2 + pitch3 + pitch4) / 4, colourWheel);
          break;
        case 3:
          BlockPulse(0 , 23, (pitch1 + pitch2 + pitch3 + pitch4) / 4, colourWheel);
          BlockPulse(24, 39, (pitch4 + pitch5 + pitch6 + pitch7) / 4, min(colourWheel+1,9));
          BlockPulse(40, 63, (pitch1 + pitch2 + pitch3 + pitch4) / 4, colourWheel);
          break;
        case 4:
          BlockPulse(0 , 7 , (pitch6 + pitch7) / 2, colourWheel);
          BlockPulse(8 , 23, (pitch5 + pitch6) / 2, min(colourWheel+1,9));
          BlockPulse(24, 39, (pitch3 + pitch4 + pitch5) / 3, min(colourWheel+2,9));
          BlockPulse(40, 55, (pitch2 + pitch3) / 2, min(colourWheel+1,9));
          BlockPulse(56, 63, (pitch1 + pitch2) / 2, colourWheel);
          break;
        default: 
          BlockPulse(0 , 7 , pitch7, colourWheel);
          BlockPulse(8 , 15, pitch6, colourWheel);
          BlockPulse(16, 23, pitch5, colourWheel);
          BlockPulse(24, 39, pitch4, colourWheel);//take the middle 2 rows
          BlockPulse(40, 47, pitch3, colourWheel);
          BlockPulse(48, 55, pitch2, colourWheel);
          BlockPulse(56, 63, pitch1, colourWheel);
          break;
      }
    } else if (VU == 1) {
      VUDown(0 , 7 , pitch7, peak7, colourWheel);
      VUUp  (8 , 15, pitch6, peak6, colourWheel);
      VUDown(16, 23, pitch5, peak5, colourWheel);
      VUUp  (24, 31, pitch4, peak4, colourWheel);
      VUDown(32, 39, (pitch4 + pitch3) / 2, peak8, colourWheel);
      VUUp  (40, 47, pitch3, peak3, colourWheel);
      VUDown(48, 55, pitch2, peak2, colourWheel);
      VUUp  (56, 63, pitch1, peak1, colourWheel);
    } else if (strobe == 1) { //just fuckin' strobe the whole thing, dont get fancy
      changePattern();//people dont like strobe
      /*switch(randomFreq){
        case 1: Strobe(0, stripLength, pitch1, colourWheel); break;
        case 2: Strobe(0, stripLength, pitch2, colourWheel); break;
        case 3: Strobe(0, stripLength, pitch3, colourWheel); break;
        case 4: Strobe(0, stripLength, pitch4, colourWheel); break;
        case 5: Strobe(0, stripLength, pitch5, colourWheel); break;
        case 6: Strobe(0, stripLength, pitch6, colourWheel); break;
        case 7: Strobe(0, stripLength, pitch7, colourWheel); break;
      }*/
    } else if (bloompulse == 1) { //bloom pulse does not work well on this few leds
      changePattern();
    } else if (twinkle == 1) {//just twinkle that shit all bands all at once
      Twinkle(0 , 7 , pitch7, colourWheel);
      Twinkle(8 , 15, pitch6, colourWheel);
      Twinkle(16, 23, pitch5, colourWheel);
      Twinkle(24, 39, pitch4, colourWheel);//take the middle 2 rows
      Twinkle(40, 47, pitch3, colourWheel);
      Twinkle(48, 55, pitch2, colourWheel);
      Twinkle(56, 63, pitch1, colourWheel);
    } else if (twinklePaint == 1) {
      TwinklePaint(0 , 7 , pitch7, colourWheel);
      TwinklePaint(8 , 15, pitch6, colourWheel);
      TwinklePaint(16, 23, pitch5, colourWheel);
      TwinklePaint(24, 39, pitch4, colourWheel);
      TwinklePaint(40, 47, pitch3, colourWheel);
      TwinklePaint(48, 55, pitch2, colourWheel);
      TwinklePaint(56, 63, pitch1, colourWheel);
    } else { changePattern(); }
  }

  strip.show();
  delay(20);
  ClearStrip();
}

void SetBrightness()
{
  LightBrightness = map(analogRead(BrightnessPotPin), 0, 1023, 0, 255);
  strip.setBrightness(LightBrightness);
}

void ClearStrip(){
  for(int i = 0;i<stripLength;i++)
  {
    strip.setPixelColor(i, Color(0,0,0));
  }
}

void displayPassive()
{
  if(passiveColour == passiveGoalColour) {
    passiveGoalColour = random(0, 255);
  } else if(passiveColour < passiveGoalColour) {
    if(random(0,5000) == 0) {passiveColour++;}
  } else {
    if(random(0,5000) == 0) {passiveColour--;}
  }
  WaterfallUp(0, 299, passiveColour, 9);
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

    /*if(debug) {
      Serial.print(tempFreq);//Transimit the DC value of the seven bands
      Serial.print("->");
      Serial.print(map(tempFreq, 0, 1023 - BottomVolTrim, 0, 255));//Transimit the DC value of the seven bands
      if(i<6) Serial.print(", ");
      else {
        Serial.println();
      }
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

void checkForPassive()
{
  if((pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7) / 7 < 10) { passiveSilenceCounter++;}
  else { passiveSilenceCounter = 0; passiveMode=false; }

  if(passiveSilenceCounter == 1000) {passiveMode=true;}
}

void beatDetection()
{
  bassPeak--;
  byte currentBassAverage = (pitch1 + pitch2) / 2; //bass beat detection off 160Hz range
  if(bassPeak < currentBassAverage)
  {
    bassPeak = min(currentBassAverage + 10, 255);
    patternChangeCounter++; //count a beat
  }
  if(patternChangeCounter > 15) {//try change the pattern every 16 beats
    patternChangeCounter = 0;
    changePattern();
  }
}

void changeColour()
{
  colourWheel = random(0, 10);//change colour between 0 - 9
}

void randomStart()
{
  clearPattern();
  changeColour();
  changePattern();
}

void clearPattern()               { waterfall=0; pulse=0; VU=0; strobe=0; bloompulse=0; twinkle=0; twinklePaint=0; }

void changePatternVU()            { clearPattern(); VU=1; } 
void changePatternPulse()         { clearPattern(); pulse=1; } 
void changePatternWaterfall()     { clearPattern(); waterfall=1; } 
void changePatternStrobe()        { clearPattern(); strobe=1; } 
void changePatternBloomPulse()    { clearPattern(); bloompulse=1; } 
void changePatternTwinkle()       { clearPattern(); twinkle=1; }
void changePatternTwinklePaint()  { clearPattern(); twinklePaint=1; }

void changePattern()
{
  displayGhost();  
  //if all bools are false, randomly pick one
  if(waterfall + VU + pulse + strobe + bloompulse + twinkle + twinklePaint != 1) {//if none, or more then one, is picked
    switch(random(0, 7)){
      case 0: changePatternWaterfall(); break;
      case 1: changePatternVU(); break;
      case 2: changePatternPulse(); break; 
      case 3: changePattern(); break; //changePatternStrobe(); break;
      case 4: changePatternBloomPulse(); break;
      case 5: changePatternTwinkle(); break;
      default: changePatternTwinklePaint(); break; //also case 6
    }
  }
  else if(waterfall) //if we are already on waterfall
  {
    if(random(0,2) == 0) {directionUp = !directionUp;} //50% chance we just change direction
    else{
      switch(random(0, 4)) {
        case 0: changePatternVU(); break;
        case 1: changePatternPulse(); break;
        case 2: changePatternBloomPulse(); break;
        default: changePatternStrobe(); break;
      } 
      changeColour(); 
    }
  }
  else //if if we are on anything other then waterfall
  {
    switch(random(0, 7)){
      case 0: changePatternWaterfall(); break;
      case 1: changePatternVU(); break;
      case 2: changePatternPulse(); break; 
      case 3: changePattern(); break; //changePatternStrobe(); break;
      case 4: changePatternBloomPulse(); break;
      case 5: changePatternTwinkle(); break;
      default: changePatternTwinklePaint(); break; //also case 6
    }
    changeColour();
  }
  randomFreq = random(1,8);
  debugOutput();
}

void debugOutput()
{
  if(debug){
    Serial.println("-----");
    if(VU)        { Serial.print("VU"); }
    if(pulse)     { Serial.print("Pulse"); }
    if(strobe)    { Serial.print("Strobe"); }
    if(bloompulse){ Serial.print("Bloom Pulse"); }
    if(waterfall) { Serial.print("Waterfall");  Serial.print(" DirectionUP:");  Serial.print(directionUp); }
    Serial.print(" | Freq:");  Serial.print(randomFreq);
    Serial.print(" Colour Wheel:");  Serial.print(colourWheel);
    Serial.println("");
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
