#include <AudioAnalyzer.h>
#include <Adafruit_NeoPixel.h>

//PIN Definition
const byte LightDataPin = 6;
const byte BrightnessPotPin = 2;

const int stripLength = 300; //length of led strip

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
byte LightBrightness = 0;//light brightness 0 - 255, read from POT on analogue 2

byte peakColour = 0; // universal colour for all VU peaks
byte colourWheel = 0;
bool changedColour = false;

int patternChangeCounter = 0;

bool waterfall = 0;
bool VU = 0;
bool pulse = 0;
bool strobe = 0;

bool strobeCurrent = 0;

bool directionUp = false;
byte randomFreq;

bool passiveMode = true;
byte passiveGoalColour = 255;
byte passiveColour = 0;
int passiveSilenceCounter = 0;

void setup()
{
  Serial.begin(57600);  //Init the baudrate
  Audio.Init();//Init audio  module 

  //Initialize Light Strip
  strip.begin();
  SetBrightness();
  strip.show(); 
  
  randomSeed(analogRead(5)); //seed the random function
  changeColour(); //start with a random colour
}

void loop()//bloompulse and strobe not used anywhere yet!
{
  //set brightness
  SetBrightness();
  
  getAudioInput(); // get the audio from the MSGEQ7
  checkForPassive(); //checks for no audio 

  /*BloomPulse(0  , 50 , pitch2, colourWheel);
  BloomPulse(51 , 100, pitch2, colourWheel);
  BloomPulse(101, 150, pitch2, colourWheel);
  BloomPulse(151, 200, pitch2, colourWheel);
  BloomPulse(201, 250, pitch2, colourWheel);
  BloomPulse(251, 299, pitch2, colourWheel);*/

  if(passiveMode){
    displayPassive();
  } else {
    beatDetection(); // changes pattern
    //pick pattern based on bools
    if(waterfall == 1){
      if(true == directionUp) {
        switch(randomFreq){
          case 1: WaterfallUp(0, stripLength, pitch1, colourWheel); break;
          case 2: WaterfallUp(0, stripLength, pitch2, colourWheel); break;
          case 3: WaterfallUp(0, stripLength, pitch3, colourWheel); break;
          case 4: WaterfallUp(0, stripLength, pitch4, colourWheel); break;
          case 5: WaterfallUp(0, stripLength, pitch5, colourWheel); break;
          case 6: WaterfallUp(0, stripLength, pitch6, colourWheel); break;
          case 7: WaterfallUp(0, stripLength, pitch7, colourWheel); break;
        }
      } else {
        switch(randomFreq){
          case 1: WaterfallDown(0, stripLength, pitch1, colourWheel); break;
          case 2: WaterfallDown(0, stripLength, pitch2, colourWheel); break;
          case 3: WaterfallDown(0, stripLength, pitch3, colourWheel); break;
          case 4: WaterfallDown(0, stripLength, pitch4, colourWheel); break;
          case 5: WaterfallDown(0, stripLength, pitch5, colourWheel); break;
          case 6: WaterfallDown(0, stripLength, pitch6, colourWheel); break;
          case 7: WaterfallDown(0, stripLength, pitch7, colourWheel); break;
        }
      }
    } else if (pulse == 1) {
      switch(randomFreq){
        case 1: 
          BlockPulse(0, stripLength, (pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7) / 7, colourWheel); //overall total volume single block pulse. simple as it gets
          break;
        case 2:
          BlockPulse(0  , stripLength / 2, (pitch1 + pitch2 + pitch3 + pitch4) / 4, colourWheel);
          BlockPulse(stripLength / 2 + 1, stripLength, (pitch4 + pitch5 + pitch6 + pitch7) / 4, colourWheel);
          break;
        case 3:
          BlockPulse(0  , stripLength / 3, (pitch1 + pitch2 + pitch3 + pitch4) / 4, colourWheel);
          BlockPulse(stripLength / 3 + 1, stripLength / 3 * 2, (pitch4 + pitch5 + pitch6 + pitch7) / 4, min(colourWheel+1,9));
          BlockPulse(stripLength / 3 * 2 + 1, stripLength, (pitch1 + pitch2 + pitch3 + pitch4) / 4, colourWheel);
          break;
        case 4:
          BlockPulse(0  , 60 , (pitch1 + pitch2) / 2, colourWheel);
          BlockPulse(61 , 120, (pitch2 + pitch3) / 2, min(colourWheel+1,9));
          BlockPulse(121, 180, (pitch3 + pitch4 + pitch5) / 3, min(colourWheel+2,9));
          BlockPulse(181, 240, (pitch2 + pitch3) / 2, min(colourWheel+1,9));
          BlockPulse(241, 299, (pitch1 + pitch2) / 2, colourWheel);
          break;
        default: 
          BlockPulse(0  , 43 , pitch1, colourWheel);
          BlockPulse(44 , 86 , pitch2, colourWheel);
          BlockPulse(87 , 129, pitch3, colourWheel);
          BlockPulse(130, 171, pitch4, colourWheel);
          BlockPulse(172, 214, pitch5, colourWheel);
          BlockPulse(215, 257, pitch6, colourWheel);
          BlockPulse(258, 299, pitch7, colourWheel);
          break;
      }
    } else if (VU == 1) {
      switch(randomFreq){
        case 1: 
          peak1 = VUDown(0  , 43 , pitch1, peak1, colourWheel);
          peak2 = VUUp(  42 , 86 , pitch2, peak2, colourWheel);
          peak3 = VUDown(87 , 129, pitch3, peak3, colourWheel);
          BlockPulse(    130, 171, pitch4, colourWheel);
          peak5 = VUUp(  172, 214, pitch5, peak5, colourWheel);
          peak6 = VUDown(215, 257, pitch6, peak6, colourWheel);
          peak7 = VUUp(  258, 299, pitch7, peak7, colourWheel);
          break;
        case 2:
          BlockPulse(    0  , 50,  pitch1, colourWheel);
          peak2 = VUUp(  51 , 100, pitch2, peak2, colourWheel);
          peak3 = VUDown(101, 150, pitch3, peak4, colourWheel);
          peak4 = VUUp(  151, 200, pitch4, peak4, colourWheel);
          peak5 = VUDown(201, 250, pitch5, peak5, colourWheel);
          BlockPulse(    251, 299, pitch6, colourWheel);
          break;
        case 3: 
          peak1 = VUDown(0  , 50 , pitch1, peak1, colourWheel);
          BlockPulse(    51 , 100, pitch2, colourWheel);
          peak3 = VUUp(  101, 150, pitch3, peak3, colourWheel);
          peak4 = VUDown(151, 200, pitch4, peak4, colourWheel);
          BlockPulse(    201, 250, pitch5, colourWheel);
          peak6 = VUUp(  251, 299, pitch6, peak6, colourWheel);
          break;
        case 4: 
          peak2 = VUDown(0  , 75 , pitch2, peak2, colourWheel);
          peak3 = VUUp(  76 , 150, pitch3, peak3, colourWheel);
          peak4 = VUDown(151, 225, pitch4, peak4, colourWheel);
          peak5 = VUUp(  226, 299, pitch5, peak5, colourWheel);
          break;
        case 5:
          peak2 = VUUp(  0  , 75 , pitch2, peak2, colourWheel);
          peak3 = VUDown(76 , 150, pitch3, peak3, colourWheel);
          peak4 = VUUp(  151, 225, pitch4, peak4, colourWheel);
          peak5 = VUDown(226, 299, pitch5, peak5, colourWheel);
          break;
        case 6:
          peak2 = VUUp(  0  , 150, (pitch1 + pitch2 + pitch3 + pitch4)/4, peak2, colourWheel);
          peak6 = VUDown(151, 299, (pitch4 + pitch5 + pitch6 + pitch7)/4, peak6, colourWheel); 
          break;
        case 7:
          peak2 = VUDown(0  , 150, (pitch1 + pitch2 + pitch3 + pitch4)/4, peak2, colourWheel);
          peak6 = VUUp(  151, 299, (pitch4 + pitch5 + pitch6 + pitch7)/4, peak6, colourWheel); 
          break;
      }
    } else if (strobe == 1) {
      switch(randomFreq){
        case 1: Strobe(0, stripLength, pitch1, colourWheel); break;
        case 2: Strobe(0, stripLength, pitch2, colourWheel); break;
        case 3: Strobe(0, stripLength, pitch3, colourWheel); break;
        case 4: Strobe(0, stripLength, pitch4, colourWheel); break;
        case 5: Strobe(0, stripLength, pitch5, colourWheel); break;
        case 6: Strobe(0, stripLength, pitch6, colourWheel); break;
        case 7: Strobe(0, stripLength, pitch7, colourWheel); break;
      }
    } else {changePattern();}
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

void checkForPassive()
{
  if((pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7) / 7 < 10) { passiveSilenceCounter++;}
  else { passiveSilenceCounter = 0; passiveMode=false; }

  if(passiveSilenceCounter == 1000) {passiveMode=true;}
}

void beatDetection()
{
  bassPeak--;
  byte currentBassAverage = pitch2; //bass beat detection off 160Hz range
  if(bassPeak < currentBassAverage)
  {
    bassPeak = max(currentBassAverage + 10, 255);
    patternChangeCounter++; //count a beat
  }
  if(patternChangeCounter > 64) {//try change the pattern every 16 beats
    patternChangeCounter = 0;
    changePattern();
  }
}

void changeColour()
{
  colourWheel = random(0, 10);//change colour between 0 - 9
}

void changePatternVU()        { waterfall=0; pulse=0; VU=1; strobe=0; }
void changePatternPulse()     { waterfall=0; pulse=1; VU=0; strobe=0; }
void changePatternWaterfall() { waterfall=1; pulse=0; VU=0; strobe=0; }
void changePatternStrobe()    { waterfall=0; pulse=0; VU=0; strobe=1; }

void changePattern()
{  
  //if all bools are false, randomly pick one
  if(waterfall + VU + pulse + strobe != 1) {//if none, or more then one, is picked
    switch(random(0, 4)){
      case 0: changePatternWaterfall(); break;
      case 1: changePatternVU(); break;
      case 2: changePatternPulse(); break; 
      default: changePatternStrobe(); break; //also case 3
    }
  }
  else if(waterfall) //if we are already on waterfall
  {
    if(random(0,1) == 0) {directionUp = !directionUp;} //50% chance we just change direction
    else{
      switch(random(0, 3)) {
        case 0: changePatternVU(); break;
        case 1: changePatternPulse(); break;
        default: changePatternStrobe(); break;
      }  
    }
  }
  else if(VU) //if we are already on VU
  {
    switch(random(0, 3)) {
      case 0: changePatternWaterfall(); break;
      case 1: changePatternPulse(); break;
      default: changePatternStrobe(); break;
    }
  }
  else if(pulse) //if we are already on pulse
  {
    switch(random(0, 3)) {
      case 0: changePatternWaterfall(); break;
      case 1: changePatternVU(); break;
      default: changePatternStrobe(); break;
    }
  }
  else if(strobe) //if we are already on strobe
  {
    switch(random(0, 3)) {
      case 0: changePatternWaterfall(); break;
      case 1: changePatternVU(); break;
      default: changePatternPulse(); break;
    }
  }
  changeColour();
  randomFreq = random(1,8);
  debugOutput();
}

void debugOutput()
{
  Serial.println("-----");
  Serial.print("VU:");
  Serial.print(VU);
  Serial.print(" Pulse:");
  Serial.print(pulse);
  Serial.print(" Strobe:");
  Serial.print(strobe);
  Serial.print(" Waterfall:");
  Serial.print(waterfall);
  Serial.print(" Waterfall Direction:");
  Serial.print(directionUp);
  Serial.println("");
  
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
