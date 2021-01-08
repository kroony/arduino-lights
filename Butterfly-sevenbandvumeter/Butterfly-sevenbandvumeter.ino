#include <AudioAnalyzer.h>
#include <Adafruit_NeoPixel.h>

//PIN Definition
const byte LightDataPin = 6;
const byte BrightnessPotPin = 2;

const int stripLength = 100; //length of led strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, LightDataPin, NEO_RGB + NEO_KHZ800);

Analyzer Audio = Analyzer(4, 5, 0); //Strobe pin ->4  RST pin ->5 Analog Pin ->0
//Analyzer Audio = Analyzer();//Strobe->4 RST->5 Analog->5
int BottomVolTrim = 100;
byte quietcounter = 0;

const byte address[6] = "00001";

int FreqVal[7];//
byte pitch1, pitch2, pitch3, pitch4, pitch5, pitch6, pitch7 = 0;
byte peak1, peak2, peak3, peak4, peak5, peak6, peak7 = 0;
byte bassPeak = 0;
byte bassPeakIncreased = 0;
byte history[stripLength];
byte LightBrightness = 0;//light brightness 0 - 255, read from POT on analogue 2

byte peakColour = 0; // universal colour for all VU peaks
byte colourWheel = 0;
bool changedColour = false;

int patternChangeCounter = 0;

bool waterfall = 0; //Broken
bool VU = 0; //Broken
bool pulse = 0; //Broken
bool strobe = 0;
bool bloompulse = 0;
bool twinkle = 0;
bool twinklePaint = 0;

bool strobeCurrent = 0;
byte strobeCounter = 0;

bool directionUp = false;
byte randomFreq;

bool passiveMode = false;
byte passiveGoalColour = 255;
byte passiveColour = 0;
int passiveSilenceCounter = 0;
byte strobeBuildupCounter = 0;

bool AudioVU = false; //demo or audio based

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
}

void loop()//bloompulse not used anywhere yet!
{
  //set brightness
  SetBrightness();

  
  if(AudioVU) 
  {
    if (random(0, 5000) > 4975) {
      changePattern();
    }
    
    getAudioInput(); // get the audio from the MSGEQ7
    //checkForPassive(); //checks for no audio
  
    //changePatternStrobe();
  
    /*WaterfallUp(0  , 49 , (pitch1 + pitch2 + pitch3) / 3, 9);
      VUUp(50 , 79 , (pitch2 + pitch3 + pitch4) / 3, peak1, 9);
      BlockPulse(80 , 105, (pitch3 + pitch4 + pitch5) / 3, 9);
      BloomPulse(106, 130, (pitch5 + pitch6 + pitch7) / 3, 9);*/
  
  
    if (passiveMode) {
      displayPassive();
    } else {
      beatDetection(); // changes pattern
  
      //pick pattern based on bools
  
      //0 = butt heart
      /*
        WaterfallUp  (1 , 28, (pitch6 + pitch7)/2, 5); //right outside
        WaterfallDown(29, 38, (pitch5 + pitch6)/2, 4); //right middle
        WaterfallUp  (39, 64, (pitch1 + pitch2)/2, 0); //left outside
        WaterfallDown(65, 74, (pitch2 + pitch3)/2, 1); //left middle
        WaterfallUp  (75, 82, (pitch4 + pitch5)/2, 3); //right inside
        WaterfallDown(83, 89, (pitch3 + pitch4)/2, 2); //left inside
      */
  
      VUUp  (1 , 28, (pitch1 + pitch2) / 2, peak1, 9);
      VUDown(29, 38, (pitch2 + pitch3) / 2, peak2, 9);
      VUUp  (39, 64, (pitch3 + pitch4) / 2, peak3, 9);
      VUDown(65, 74, (pitch4 + pitch5) / 2, peak4, 9);
      VUUp  (75, 82, (pitch5 + pitch6) / 2, peak5, 9);
      VUDown(83, 89, (pitch6 + pitch7) / 2, peak6, 9);
  
      /*VUDown(1 , 28, (pitch1 + pitch2)/2, peak1, 1);
        VUUp  (29, 38, (pitch2 + pitch3)/2, peak2, 2);
        VUDown(39, 64, (pitch3 + pitch4)/2, peak3, 3);
        VUUp  (65, 74, (pitch4 + pitch5)/2, peak4, 4);
        VUDown(75, 82, (pitch5 + pitch6)/2, peak5, 5);
        VUUp  (83, 89, (pitch6 + pitch7)/2, peak6, 6);*/
  
  
      /*if(waterfall == 1){
        if(true == directionUp) {
          WaterfallUp  (1 , 28, (pitch6 + pitch7)/2, 5); //right outside
          WaterfallDown(29, 38, (pitch5 + pitch6)/2, 4); //right middle
          WaterfallUp  (39, 64, (pitch1 + pitch2)/2, 0); //left outside
          WaterfallDown(65, 74, (pitch2 + pitch3)/2, 1); //left middle
          WaterfallUp  (75, 82, (pitch4 + pitch5)/2, 3); //right inside
          WaterfallDown(83, 89, (pitch3 + pitch4)/2, 2); //left inside
        } else {
          WaterfallDown(1 , 28, (pitch6 + pitch7)/2, 5); //right outside
          WaterfallUp  (29, 38, (pitch5 + pitch6)/2, 4); //right middle
          WaterfallDown(39, 64, (pitch1 + pitch2)/2, 0); //left outside
          WaterfallUp  (65, 74, (pitch2 + pitch3)/2, 1); //left middle
          WaterfallDown(75, 82, (pitch4 + pitch5)/2, 3); //right inside
          WaterfallUp  (83, 89, (pitch3 + pitch4)/2, 2); //left inside
        }
        } else if (pulse == 1) {
        switch(randomFreq){
          case 1:
            BlockPulse(0, stripLength, (pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7) / 7, colourWheel); //overall total volume single block pulse. simple as it gets
            break;
          case 2:
            BlockPulse(0, stripLength, (pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7) / 7, colourWheel); //overall total volume single block pulse. simple as it gets
            break;
          case 3:
            BlockPulse(0, stripLength, (pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7) / 7, colourWheel); //overall total volume single block pulse. simple as it gets
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
            peak2 = VUUp(  0  , 150, (pitch1 + pitch2 + pitch3 + pitch4)/4, peak2, colourWheel);
            peak6 = VUDown(151, 299, (pitch4 + pitch5 + pitch6 + pitch7)/4, peak6, colourWheel);
            break;
          case 2:
            peak2 = VUUp(  0  , 150, (pitch1 + pitch2 + pitch3 + pitch4)/4, peak2, colourWheel);
            peak6 = VUDown(151, 299, (pitch4 + pitch5 + pitch6 + pitch7)/4, peak6, colourWheel);
            break;
          case 3:
            peak2 = VUUp(  0  , 150, (pitch1 + pitch2 + pitch3 + pitch4)/4, peak2, colourWheel);
            peak6 = VUDown(151, 299, (pitch4 + pitch5 + pitch6 + pitch7)/4, peak6, colourWheel);
            break;
          default:
            peak2 = VUDown(0  , 150, (pitch1 + pitch2 + pitch3 + pitch4)/4, peak2, colourWheel);
            peak6 = VUUp(  151, 299, (pitch4 + pitch5 + pitch6 + pitch7)/4, peak6, colourWheel);
            break;
        }
        } else if (strobe == 1) { //just fuckin' strobe the whole thing, dont get fancy
        switch(randomFreq){
          case 1: Strobe(0, stripLength, pitch1, colourWheel); break;
          case 2: Strobe(0, stripLength, pitch2, colourWheel); break;
          case 3: Strobe(0, stripLength, pitch3, colourWheel); break;
          case 4: Strobe(0, stripLength, pitch4, colourWheel); break;
          case 5: Strobe(0, stripLength, pitch5, colourWheel); break;
          case 6: Strobe(0, stripLength, pitch6, colourWheel); break;
          case 7: Strobe(0, stripLength, pitch7, colourWheel); break;
        }
        } else if (bloompulse == 1) { //just bloom pulse, but, chill bro
        BloomPulse(0  , 43 , pitch1, colourWheel);
        BloomPulse(44 , 86 , pitch2, colourWheel);
        BloomPulse(87 , 129, pitch3, colourWheel);
        BloomPulse(130, 171, pitch4, colourWheel);
        BloomPulse(172, 214, pitch5, colourWheel);
        BloomPulse(215, 257, pitch6, colourWheel);
        BloomPulse(258, 299, pitch7, colourWheel);
        } else if (twinkle == 1) {//just twinkle that shit all bands all at once
        Twinkle(0  , 43 , pitch1, colourWheel);
        Twinkle(44 , 86 , pitch2, colourWheel);
        Twinkle(87 , 129, pitch3, colourWheel);
        Twinkle(130, 171, pitch4, colourWheel);
        Twinkle(172, 214, pitch5, colourWheel);
        Twinkle(215, 257, pitch6, colourWheel);
        Twinkle(258, 299, pitch7, colourWheel);
        } else if (twinklePaint == 1) {
        TwinklePaint(0  , 43 , pitch1, colourWheel);
        TwinklePaint(44 , 86 , pitch2, colourWheel);
        TwinklePaint(87 , 129, pitch3, colourWheel);
        TwinklePaint(130, 171, pitch4, colourWheel);
        TwinklePaint(172, 214, pitch5, colourWheel);
        TwinklePaint(215, 257, pitch6, colourWheel);
        TwinklePaint(258, 299, pitch7, colourWheel);
        } else { changePattern(); }*/
    }
  } else { //demo mode

    byte selector = random(0, 4);

    switch (selector) {
      case 0: demoWaterfallDot(50); break;
      case 1: demoPaint(50); break;
      case 2: theaterChase(50); break;
      case 3: Flap(100); break;
    }
    
    /*for(byte i=0; i<255; i++){
      WaterfallDown(1, 28, i, 5); //right outer
      WaterfallDown(39, 64, i, 5); //left outer

      WaterfallUp(65, 74, i, 4); //left middle
      WaterfallUp(29, 38, i, 4); //right middle

      WaterfallDown(75, 82, i, 3); //right inner
      WaterfallUp(83, 89, i, 3); //left inner
      
      strip.show();
      delay(15);
      ClearStrip();
    }*/

    /*byte randFreq = 0;
    for(byte i=0; i<255; i++){
      randFreq = random(0, 255);
      WaterfallDown(1, 28, randFreq, 5); //right outer
      WaterfallDown(39, 64, randFreq, 5); //left outer

      WaterfallUp(65, 74, randFreq, 4); //left middle
      WaterfallUp(29, 38, randFreq, 4); //right middle

      WaterfallDown(75, 82, randFreq, 3); //right inner
      WaterfallUp(83, 89, randFreq, 3); //left inner
      strip.show();
      delay(15);
      ClearStrip();
    }*/

    /*for(byte i=255; i>0; i--){
      WaterfallDown(1, 28, i, 5); //right outer
      WaterfallDown(39, 64, i, 5); //left outer

      WaterfallUp(65, 74, i, 4); //left middle
      WaterfallUp(29, 38, i, 4); //right middle

      WaterfallDown(75, 82, i, 3); //right inner
      WaterfallUp(83, 89, i, 3); //left inner
      strip.show();
      delay(15);
      ClearStrip();
    }*/

    /*ClearStrip();
    strip.show();
    delay(1000);

    randFreq = 0;
    for(byte i=0; i<255; i++){
      randFreq = random(0, 255);
      if(randFreq < 250) { randFreq = 0;}
      WaterfallDown(1, 28, randFreq, 5); //right outer
      WaterfallDown(39, 64, randFreq, 5); //left outer

      WaterfallUp(65, 74, randFreq, 4); //left middle
      WaterfallUp(29, 38, randFreq, 4); //right middle

      WaterfallDown(75, 82, randFreq, 3); //right inner
      WaterfallUp(83, 89, randFreq, 3); //left inner
      strip.show();
      delay(15);
      ClearStrip();
    }*/
      
  }


  strip.show();
  delay(15);
  ClearStrip();
}

void SetBrightness() {
  LightBrightness = map(analogRead(BrightnessPotPin), 0, 1023, 0, 255);
  strip.setBrightness(LightBrightness);
}

void ClearStrip() {
  for (int i = 0; i < stripLength; i++)
  {
    strip.setPixelColor(i, Color(0, 0, 0));
  }
}

void displayPassive()
{
  if (passiveColour == passiveGoalColour) {
    passiveGoalColour = random(0, 255);
  } else if (passiveColour < passiveGoalColour) {
    if (random(0, 5000) == 0) {
      passiveColour++;
    }
  } else {
    if (random(0, 5000) == 0) {
      passiveColour--;
    }
  }
  WaterfallUp(0, 299, passiveColour, 9);
}

void getAudioInput()
{
  Audio.ReadFreq(FreqVal);//return 7 value of 7 bands pass filiter
  //Frequency(Hz):63  160  400  1K  2.5K  6.25K  16K
  //FreqVal[]:     0    1    2   3     4      5    6
  for (int i = 0; i < 7; i++)
  {
    int tempFreq = max((FreqVal[i] - BottomVolTrim), 0);
    byte tempTrim = map(tempFreq, 0, 1023 - BottomVolTrim, 0, 255);

    Serial.print(tempFreq);//Transimit the DC value of the seven bands
    Serial.print("->");
    Serial.print(map(tempFreq, 0, 1023 - BottomVolTrim, 0, 255));//Transimit the DC value of the seven bands
    if (i < 6) Serial.print(", ");
    else {
      Serial.println();
    }
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
  if ((pitch1 + pitch2 + pitch3 + pitch4 + pitch5 + pitch6 + pitch7) / 7 < 10) {
    passiveSilenceCounter++;
  }
  else {
    passiveSilenceCounter = 0;
    passiveMode = false;
  }

  if (passiveSilenceCounter == 1000) {
    passiveMode = true;
  }
}

void beatDetection()
{
  bassPeak--;
  byte currentBassAverage = (pitch1 + pitch2 + pitch3) / 3; //bass beat detection off 160Hz range
  if (bassPeak < currentBassAverage)
  {
    if (bassPeakIncreased == false)
    {
      bassPeak = max(currentBassAverage + 10, 255);
      patternChangeCounter++; //count a beat
      bassPeakIncreased = true;
    }
  } else {
    bassPeakIncreased = false;
  }
  if (patternChangeCounter > 16) { //try change the pattern every 8 beats
    patternChangeCounter = 0;
    changePattern();
  }
}

void changeColour()
{
  colourWheel = random(0, 10);//change colour between 0 - 9
}

void clearPattern()               {
  waterfall = 0;
  pulse = 0;
  VU = 0;
  strobe = 0;
  bloompulse = 0;
  twinkle = 0;
  twinklePaint = 0;
}

void changePatternVU()            {
  clearPattern();
  VU = 1;
}
void changePatternPulse()         {
  clearPattern();
  pulse = 1;
}
void changePatternWaterfall()     {
  clearPattern();
  waterfall = 1;
}
//void changePatternStrobe()        { clearPattern(); strobe=1; } // noone likes strobe so its gone
void changePatternStrobe()        {
  changePattern();
}

void changePatternBloomPulse()    {
  clearPattern();
  bloompulse = 1;
}
void changePatternTwinkle()       {
  clearPattern();
  twinkle = 1;
}
void changePatternTwinklePaint()  {
  clearPattern();
  twinklePaint = 1;
}

void changePattern()
{
  //if all bools are false, randomly pick one
  if (waterfall + VU + pulse + strobe + bloompulse + twinkle + twinklePaint != 1) { //if none, or more then one, is picked
    switch (random(0, 7)) {
      case 0: changePatternWaterfall(); break;
      case 1: changePatternVU(); break;
      case 2: changePatternPulse(); break;
      case 3: changePatternStrobe(); break;
      case 4: changePatternBloomPulse(); break;
      case 5: changePatternTwinkle(); break;
      default: changePatternTwinklePaint(); break; //also case 6
    }
  }
  else if (waterfall) //if we are already on waterfall
  {
    if (random(0, 2) == 0) {
      directionUp = !directionUp; //50% chance we just change direction
    }
    else {
      switch (random(0, 4)) {
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
    switch (random(0, 7)) {
      case 0: changePatternWaterfall(); break;
      case 1: changePatternVU(); break;
      case 2: changePatternPulse(); break;
      case 3: changePatternStrobe(); break;
      case 4: changePatternBloomPulse(); break;
      case 5: changePatternTwinkle(); break;
      default: changePatternTwinklePaint(); break; //also case 6
    }
    changeColour();
  }
  randomFreq = random(1, 8);
  debugOutput();
}

void debugOutput()
{
  Serial.println("-----");
  if (VU)          {
    Serial.print("VU");
  }
  if (pulse)       {
    Serial.print("Pulse");
  }
  if (strobe)      {
    Serial.print("Strobe");
  }
  if (bloompulse)  {
    Serial.print("Bloom Pulse");
  }
  if (twinkle)     {
    Serial.print("Twinkle");
  }
  if (twinklePaint) {
    Serial.print("Twinkle Paint");
  }
  if (waterfall) {
    Serial.print("Waterfall");
    Serial.print(" DirectionUP:");
    Serial.print(directionUp);
  }
  Serial.print(" | Freq:");  Serial.print(randomFreq);
  Serial.print(" Colour Wheel:");  Serial.print(colourWheel);
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
