#include <AudioAnalyzer.h>
#include <Adafruit_NeoPixel.h>
const byte LightDataPin = 6;
const int stripLength = 39; //length of led strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, LightDataPin, NEO_GRB + NEO_KHZ800);

// Digital IO's
//pin 0
//pin 1
//byte IRtransmitPin          = 2;      // Primary fire mode IR transmitter pin: Use pins 2,4,7,8,12 or 13. DO NOT USE PWM pins!! More info: http://j44industries.blogspot.com/2009/09/arduino-frequency-generation.html#more
//byte triggerPin             = 3;      // Push button for primary fire. Low = pressed
//byte speakerPin             = 4;      // Direct output to piezo sounder/speaker
//byte ammoPin                = 5;      // An analogue output (PWM) level corresponds to remaining ammunition. Use PWM pin: 3,5,6,9,10 or 11.  //Commented out cause no lights
//byte lifePin                = 6;      // An analogue output (PWM) level corresponds to remaining life. Use PWM pin: 3,5,6,9,10 or 11. Can be used to drive LED bar graphs. eg LM3914N ////Commented out cause no lights
//byte hitPin                 = 7;      // LED output pin used to indicate when the player has been hit. //commented out cause no lights
//byte IRtransmit2Pin         = 8;      // Secondary fire mode IR transmitter pin:  Use pins 2,4,7,8,12 or 13. DO NOT USE PWM pins!!
//byte audioPin               = 9;      // Audio Trigger. Can be used to set off sounds recorded in the kind of electronics you can get in greetings card that play a custom message.
//byte muzzelLedPin           = 10;     // Flashes when a shot is fired simulating muzzel flash
//byte cyclePin               = 11;     // Push button for cycling through function to change. Low = pressed
byte IRreceive2Pin          = 11;     // Allows for checking external sensors are attached as well as distinguishing between sensor locations (eg spotting head shots)
byte IRreceivePin           = 12;     // The pin that incoming IR signals are read from
//byte cyclePin               = 13;     // 

// Minimum gun requirements: trigger, IR led, piezo speaker, 2 menu buttons.

// Player and Game details
byte myTeamID               = 1;      // 1 - 7 (0 = system message)
byte myPlayerID             = 3;      // 1 - 31 (0 = team base) 
byte myGameID               = 0;      // Interprited by configureGane subroutine; allows for quick change of game types.
byte myWeaponID             = 0;      // Deffined by gameType and configureGame subroutine.
byte myWeaponDamage         = 0;      // Deffined by gameType and configureGame subroutine.
byte maxAmmo                = 0;      // Deffined by gameType and configureGame subroutine.
byte maxAmmoClips           = 0;      // Deffined by gameType and configureGame subroutine.
byte maxHp                  = 0;      // Deffined by gameType and configureGame subroutine.
byte automatic              = 0;      // Deffined by gameType and configureGame subroutine. Automatic fire 0 = Semi Auto, 1 = Fully Auto.
byte automatic2             = 0;      // Deffined by gameType and configureGame subroutine. Secondary fire auto?

//Incoming signal Details
int received[18];                     // Received data: received[0] = which sensor, received[1] - [17] byte1 byte2 parity (Miles Tag structure)
int check                   = 0;      // Variable used in parity checking

// Stats
byte ammo                   = 0;      // Current ammunition
byte clips                  = 0;      // Current Clips (each clip contains max ammo)
byte hp                     = 0;      // Current hp
bool alive                  = true;   // set alive
long shotFired              = 0;      // Count of shots fired, could be used to calc accuracy ect later

// Code Variables
int timeOut                 = 0;      // Defined in frequencyCalculations (IRpulse + 50)
//byte FIRE                   = 0;      // 0 = don't fire, 1 = Fire
//byte SELECT                 = 0;      // 0 = dont select, 1 = select
//byte CYCLE                  = 0;      // 0 = dont cycle, 1 = cycle
//byte triggerRead            = 0;      // Trigger Reading
//byte lastTriggerRead        = 0;      // Last Trigger Reading
//byte selectRead             = 0;      // Select Button Reading
//byte lastSelectRead         = 0;      // Last Select Button Reading
//byte cycleRead              = 0;      // Cycle Button Reading
//byte lastCycleRead          = 0;      // Last Cycle Button Reading

//menu varibles
byte currentSetting         = 0;      //0 = Pitch, 1 = Pattern, 2 = Colour
byte currentPitch           = 0;      //0 - 6 for Low to High
byte currentPattern         = 0;      //0 - 6 for waterfall, VU, Pulse, Strobe, BloomPulse
byte currentColourWheel     = 0;      //0 - 9 for colour wheels
byte dummyFreq              = 0;      //fade from 0 - 255 to demo patern
byte dummyMaxFreq           = 0;      //holder for max freq
byte strobeCounter          = 0;      //strobe count
bool strobeCurrent          = true;   //strobe state
byte peakColour             = 0;      // universal colour for all VU peaks
byte history[stripLength];            // history of strip for waterfall
unsigned int sleepTimer     = 0;      //counter for the gun going into sleep mode
bool sleepState             = false;  //flag for sleep mode

// Signal Properties
int IRpulse                 = 600;    // Basic pulse duration of 600uS MilesTag standard 4*IRpulse for header bit, 2*IRpulse for 1, 1*IRpulse for 0.
int IRfrequency             = 38;     // Frequency in kHz Standard values are: 38kHz, 40kHz. Choose dependant on your receiver characteristics
int IRt                     = 0;      // LED on time to give correct transmission frequency, calculated in setup.
int IRpulses                = 0;      // Number of oscillations needed to make a full IRpulse, calculated in setup.
int header                  = 4;      // Header lenght in pulses. 4 = Miles tag standard
//int maxSPS                  = 10;     // Maximum Shots Per Seconds. Not yet used.
//int TBS                     = 0;      // Time between shots. Not yet used.

// Transmission data
int byte1[8];                         // String for storing byte1 of the data which gets transmitted when the player fires.
int byte2[8];                         // String for storing byte1 of the data which gets transmitted when the player fires.
int myParity                = 0;      // String for storing parity of the data which gets transmitted when the player fires.

// Received data
//int memory                  = 25;     // Number of signals to be recorded: Allows for the game data to be reviewed after the game, no provision for transmitting / accessing it yet though.
//int hitNo                   = 0;      // Hit number
// Byte1
int player[25];                      // Array must be as large as memory
int team[25];                        // Array must be as large as memory
// Byte2
int weapon[25];                      // Array must be as large as memory
int damage[25];                          // Array must be as large as memory
int parity[25];                      // Array must be as large as memory

void setup() {    
  // Serial coms set up to help with debugging.
  Serial.begin(9600);              
  Serial.println("Startup...");
  

  // Pin declarations
  //pinMode(triggerPin, INPUT_PULLUP);
  //pinMode(selectPin, INPUT_PULLUP);
  //pinMode(cyclePin, INPUT_PULLUP);
  //pinMode(speakerPin, OUTPUT);
  //pinMode(IRtransmitPin, OUTPUT);
  //pinMode(muzzelLedPin, OUTPUT);
  pinMode(IRreceivePin, INPUT);
 
  frequencyCalculations();   // Calculates pulse lengths etc for desired frequency
  tagCode();                 // Based on game details etc works out the data that will be transmitted when a shot is fired
 
  //digitalWrite(triggerPin, HIGH);      // Not really needed if your circuit has the correct pull up resistors already but doesn't harm
  //digitalWrite(reloadPin, HIGH);     // Not really needed if your circuit has the correct pull up resistors already but doesn't harm

  //output gun data
  Serial.print("Team: ");           Serial.println(myTeamID);
  Serial.print("Player: ");         Serial.println(myPlayerID);
  Serial.print("myWeaponHP: ");     Serial.println(myWeaponDamage);
  Serial.print("maxAmmo: ");        Serial.println(maxAmmo);
  Serial.print("maxAmmoClips: ");   Serial.println(maxAmmoClips);
  Serial.print("maxHP: ");          Serial.println(maxHp);
  Serial.println();
  Serial.print("Ammo: ");           Serial.print(ammo);
  Serial.print(" Clips: ");         Serial.print(clips);
  Serial.print(" HP: ");            Serial.println(hp);
  Serial.println("Ready....");
  Serial.println();
  
  //Initialize Light Strip
  strip.begin();
  strip.setBrightness(100);
  strip.show();

  //Run LED test
  /*colorWipe(strip.Color(255, 0, 0)); // Red
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Off
  
  colorWipe(strip.Color(0, 255, 0)); // Green
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Off
  
  colorWipe(strip.Color(0, 0, 255)); // Blue
  delay(1000);
  colorWipe(strip.Color(0, 0, 0)); // Off
  
  colorWipe(strip.Color(255, 255, 255)); // White
  delay(5000);
  colorWipe(strip.Color(0, 0, 0)); // Off*/

  colorWipe(strip.Color(0, 0, 0));
}


void loop(){
  receiveIR();
    
  displayStrip();
  delay(20);
}

void displayStrip() {
  dummyFreq++;
  if(dummyFreq == 255) { dummyFreq = 0; }
  switch (currentPattern) {
    case 0: WaterfallUp(0 , 38, dummyFreq, currentColourWheel); break; //waterfall
    case 1: dummyMaxFreq = VUUp(0, 38, dummyFreq, dummyMaxFreq, currentColourWheel); break; //VU
    case 2: BlockPulse(0, 38, dummyFreq, currentColourWheel); break; //pulse
    case 3: Strobe(0, 38, dummyFreq, currentColourWheel); break; //strobe
    case 4: BloomPulse(0, 38, dummyFreq, currentColourWheel); break; //BloomPulse
    case 5: Twinkle(0, 38, dummyFreq, currentColourWheel); break; //Twinkle
    case 6: TwinklePaint(0, 38, dummyFreq, currentColourWheel); break; //Twinkle Paint
    default: currentPattern = 6; break;
  }
  strip.show();
}

uint32_t getCurrentColourFromActiveWheel(byte Freq)
{
  switch (currentColourWheel) {
      case 0: return grFadeWheel(Freq); break;
      case 1: return rbFadeWheel(Freq); break;
      case 2: return bgFadeWheel(Freq); break;
      case 3: return gbFadeWheel(Freq); break;
      case 4: return brFadeWheel(Freq); break;
      case 5: return rgFadeWheel(Freq); break;
      case 6: return gpWheel(Freq); break;
      case 7: return byWheel(Freq); break;
      case 8: return rcWheel(Freq); break;
      case 9: return Wheel(Freq); break;
      default: currentColourWheel = 9; break;
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

void ClearStrip(){
  for(int i = 0;i<stripLength;i++)
  {
    strip.setPixelColor(i, Color(0,0,0));
  }
}


