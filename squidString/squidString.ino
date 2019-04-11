//#include <AudioAnalyzer.h>
#include <Adafruit_NeoPixel.h>

//PIN Definition
const byte LightDataPin = 6;
const byte BrightnessPotPin = 2;

const int stripLength = 700; //length of led strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, LightDataPin, NEO_RGB + NEO_KHZ800);

bool debug = true;

byte LightBrightness = 255;//light brightness 0 - 255, read from POT on analogue 2

byte peakColour = 0; // universal colour for all VU peaks
byte colourWheel = 0;
bool changedColour = false;

byte history[stripLength];

int outputCount = 0;

bool directionUp = false;
byte randomFreq;

bool passiveMode = true;
byte passiveGoalColour = 255;
byte passiveColour = 0;
int passiveSilenceCounter = 0;
byte strobeBuildupCounter = 0;

class DotObject
{
  public:
    byte age = 1;
    byte location;
    uint32_t dotColour;
    byte type = 0; 
    /*
     *  0 - Inactive
     *  1 - Stationary increasing
     *  2 - Stationary decreasing
     *  3 - Pulse dot
     *  4 - Fading pulse dot forwards
     *  5 - Fading pulse dot backwards
     *  6 - flash
     *  7 - blinker
     */

    static uint32_t Color(byte r_, byte g_, byte b_)
    {
      uint32_t c;
      c = r_;
      c <<= 8;
      c |= g_;
      c <<= 8;
      c |= b_;
      return c;
    }

    static uint32_t brightness(uint32_t colour, byte bright)
    {
      float brightnessModifyer = float(bright / 255.00);
      float b_ = colour & 255;
      float g_ = (colour >> 8) & 255;
      float r_ = (colour >> 16) & 255;
      return Color(r_ * brightnessModifyer, g_ * brightnessModifyer, b_ * brightnessModifyer);
    }

    uint32_t getRandomColour()
    {
       byte colourpicker = random(0,7);
    
        switch (colourpicker) {
          case 0: return Color(255, 0  , 0  ); break; //red
          case 1: return Color(0  , 255, 0  ); break; //green
          case 2: return Color(0  , 0  , 255); break; //blue
          case 3: return Color(255, 255, 0  ); break; //yellow
          case 4: return Color(255, 0  , 255); break; //purple
          case 5: return Color(0  , 255, 255); break; //orange
          case 6: return Color(255, 255, 255); break; //white
        }
    }

    void initaliseStationary()
    {
      type = 1;
      age = 1;
      location = random(0, stripLength);
      dotColour = getRandomColour();
    }

    void initaliseFlash()
    {
      type = 6;
      age = 1;
      location = random(0, stripLength);
      dotColour = getRandomColour();
    }

    void initaliseBlinker()
    {
      type = 7;
      age = 1;
      location = random(0, stripLength);
      dotColour = getRandomColour();
    }

    void initalisePulse()
    {
      type = 3;
      age = 1;
      location = 0;
      dotColour = getRandomColour();
    }

    void initaliseFading()
    {
      type = random(4, 6);
      Serial.println(type);
      age = random(100, 255);
      if(type == 4) { location = random(0, 50); }
      else { location = random(150, 200); }
      
      dotColour = getRandomColour();
    }

    void paint()
    {
      if(type == 1 || type == 2) //Stationary Dot
      {
        strip.setPixelColor(location-9, brightness(dotColour,(age/10)*1));
        strip.setPixelColor(location-8, brightness(dotColour,(age/10)*2));
        strip.setPixelColor(location-7, brightness(dotColour,(age/10)*3));
        strip.setPixelColor(location-6, brightness(dotColour,(age/10)*4));
        strip.setPixelColor(location-5, brightness(dotColour,(age/10)*5));
        strip.setPixelColor(location-4, brightness(dotColour,(age/10)*6));
        strip.setPixelColor(location-3, brightness(dotColour,(age/10)*7));
        strip.setPixelColor(location-2, brightness(dotColour,(age/10)*8));
        strip.setPixelColor(location-1, brightness(dotColour,(age/10)*9));
        strip.setPixelColor(location, brightness(dotColour,age));
        strip.setPixelColor(location+1, brightness(dotColour,(age/10)*9));
        strip.setPixelColor(location+2, brightness(dotColour,(age/10)*8));
        strip.setPixelColor(location+3, brightness(dotColour,(age/10)*7));
        strip.setPixelColor(location+4, brightness(dotColour,(age/10)*6));
        strip.setPixelColor(location-5, brightness(dotColour,(age/10)*5));
        strip.setPixelColor(location-6, brightness(dotColour,(age/10)*4));
        strip.setPixelColor(location-7, brightness(dotColour,(age/10)*3));
        strip.setPixelColor(location-8, brightness(dotColour,(age/10)*2));
        strip.setPixelColor(location-9, brightness(dotColour,(age/10)*1));

        if       (type == 1) {
          age++;
          if(age == 255) { type = 2; }
          
        } else if(type == 2) { 
          age--; 
          if(age==0)  { type = 0;}
        }
      } 
      else if(type == 3) // pulse dot 
      {
        strip.setPixelColor(location  , brightness(dotColour,255));
        strip.setPixelColor(location-1, brightness(dotColour,200));
        strip.setPixelColor(location-2, brightness(dotColour,150));
        strip.setPixelColor(location-3, brightness(dotColour,100));
        strip.setPixelColor(location-4, brightness(dotColour,50));

        location++;
        if(location == stripLength+5) { type = 0; }
      }
      else if(type == 4) //fading forward
      {
        strip.setPixelColor(location  , brightness(dotColour,age));
        age--;
        location++;
        if(location == stripLength || age == 0) { type = 0; }
      }
      else if(type == 5) //fading backwards
      {
        strip.setPixelColor(location  , brightness(dotColour,age));
        age--;
        location--;
        if(location == 0 || age == 0) { type = 0; }
      }
      else if(type == 6) //flash
      {
        strip.setPixelColor(location, dotColour);
        age++;
        if(age == 2) { type = 0; }
      }

      else if(type == 7) //flash
      {
        age++;

        switch (age) {
          case 0   ... 20 : strip.setPixelColor(location, dotColour);
          case 21  ... 40 : break;
          case 41  ... 60 : strip.setPixelColor(location, dotColour);
          case 61  ... 80 : break;
          case 81  ... 100: strip.setPixelColor(location, dotColour);
          case 101 ... 120: break;
          case 121 ... 125: strip.setPixelColor(location, dotColour);
          case 126 ... 130: break;
          case 131 ... 135: strip.setPixelColor(location, dotColour);
          case 136 ... 140: break;
          case 141 ... 145: strip.setPixelColor(location, dotColour);
          case 146 ... 150: break;
          case 151 ... 155: strip.setPixelColor(location, dotColour);
          case 156 ... 160: break;
          case 161 ... 165: strip.setPixelColor(location, dotColour);
          case 166 ... 170: break;
        }
        
        if(age == 171) { type = 0; }
      }
    }
};

DotObject dotArray[20];

void setup()
{
  Serial.begin(57600);  //Init the baudrate

  //Initialize Light Strip
  strip.begin();
  //SetBrightness();
  strip.setBrightness(128);
  strip.show(); 
  
  randomSeed(analogRead(5)); //seed the random function

  //Run LED test
  
  colorWipe(strip.Color(255, 0, 0)); // Red
  delay(1000);
  //colorWipe(strip.Color(0, 0, 0)); // Off
  
  colorWipe(strip.Color(0, 255, 0)); // Green
  delay(1000);
  //colorWipe(strip.Color(0, 0, 0)); // Off
  
  colorWipe(strip.Color(0, 0, 255)); // Blue
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Off
  
  
  /*colorWipe(strip.Color(255, 255, 255)); // White
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Off*/

}

void loop()//bloompulse not used anywhere yet!
{
  //set brightness
  //SetBrightness();

  //paint dots
  for(byte x = 0; x < 20; x++) {
    if (dotArray[x].type != 0) dotArray[x].paint();
  }


  //random create a new fading dot
  if(random(0, 150) == 1) {
    for(byte x = 0; x < 20; x++) {
      if(dotArray[x].type == 0) {
        dotArray[x].initaliseFading();
        
        Serial.println("Create Fading Dot");
        //debugOutput();
        x = 20;
      }
    }
  }
  
  //random create a new stationary dot
  if(random(0, 300) == 1) {
    for(byte x = 0; x < 20; x++) {
      if(dotArray[x].type == 0) {
        dotArray[x].initaliseStationary();
        
        //Serial.println("Create Stationary Dot");
        //debugOutput();
        x = 20;
      }
    }
  }

  //random create a new pulse dot
  if(random(0, 1000) == 1) {
    for(byte x = 0; x < 20; x++) {
      if(dotArray[x].type == 0) {
        dotArray[x].initalisePulse();

        //Serial.println("Create Pulse Dot");
        //debugOutput();
        x = 20;
      }
    }
  }

  //random create a new flash dot
  if(random(0, 50) == 1) {
    for(byte x = 0; x < 20; x++) {
      if(dotArray[x].type == 0) {
        dotArray[x].initaliseFlash();

        //Serial.println("Create Flash Dot");
        //debugOutput();
        x = 20;
      }
    }
  }

  //random create a new Blinker dot
  if(random(0, 250) == 1) {
    for(byte x = 0; x < 20; x++) {
      if(dotArray[x].type == 0) {
        dotArray[x].initaliseBlinker();

        //Serial.println("Create Blinker Dot");
        //debugOutput();
        x = 20;
      }
    }
  }
  
  strip.show();
  delay(GetDelayDuration());
  strip.clear();
}

byte GetDelayDuration()
{
  return map(analogRead(BrightnessPotPin), 0, 1023, 0, 255);
}

void SetBrightness()
{
  LightBrightness = map(analogRead(BrightnessPotPin), 0, 1023, 0, 255);
  strip.setBrightness(LightBrightness);
}

void debugOutput()
{
  if(debug){
    Serial.println("-----");
    for(byte x = 0; x < 20; x++) {
      Serial.print("Dot: "); Serial.print(x);
      Serial.print(" type: "); Serial.print(dotArray[x].type);
      Serial.println("");
      
    }
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

static uint32_t brightness(uint32_t colour, float bright)
{
  //bright *= bright;
  byte b_ = colour & 255;
  byte g_ = (colour >> 8) & 255;
  byte r_ = (colour >> 16) & 255;
  return Color(r_ * bright, g_ * bright, b_ * bright);
}
