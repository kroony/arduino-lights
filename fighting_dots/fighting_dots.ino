//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(350, 6, NEO_RGB + NEO_KHZ800);

//Includes for 16x2 LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const byte screenWidth = 16;
// Set the LCD address to 0x3f for a 16 chars and 2 line display
//make a screen for red and blue team
LiquidCrystal_I2C redLCD(0x3F, screenWidth, 2);
LiquidCrystal_I2C blueLCD(0x3E, screenWidth, 2);

void displayDigit(int number);
void writeScores();

const byte totalScore = 20;

byte scoreBlue = totalScore;
byte scoreRed = totalScore;
byte previousBlueScore = totalScore;
byte previousRedScore = totalScore;

bool gameActive = false;

const byte DOT_ARRAY_SIZE = 3;

const char space = ' ';
const byte charDelay = 250;
const uint16_t screenDelay = 1000;

//Intro Strings
//       SCREEN LIMIT       = "1234567890123456"
const char intro1[17]       = "Fighting Dots";
const char intro2[17]       = "A Line Game";

const char intro3[17]       = "Made By Amos,";
const char intro4[17]       = "Bill, & Matt";

/*
const char intro5red[17]    = "Red Button";
const char intro6red[17]    = "Attacks";

const char intro5blue[17]   = "Blue Button";
const char intro6blue[17]   = "Attacks";

const char intro7[17]       = "Green Button";
const char intro8[17]       = "Defends";

const char intro9red[17]    = "Hit Blue 20";
const char intro10red[17]   = "times to win";

const char intro9blue[17]   = "Hit Red 20";
const char intro10blue[17]  = "times to win";
*/

const char intro11[17]      = "New Game In";
const char intro12[17]      = "3...2...1...";

const char win1[17]         = "Congratulations";
const char win2[17]         = "You Won! W00t!";

const char lose1[17]        = "Yea Nah Mate";
const char lose2[17]        = "You Lost. Sorry";

class DotObject
{
  public:
    bool active = false;
    double location;
    double velocity;
    bool teamRed;
    bool attack;
    
    static uint32_t Color(byte r, byte g, byte b);// return 32bit int colour from RGB 0 - 255

    static uint32_t brightness(uint32_t colour, float bright);//return 32bit int colour at brightness 0 - 255
    
    void activate(bool teamRed_, bool attack_);//is the team red? //is it an attack dot
    
    void loop(); //update location and print to strip. 

    void slowdown(); //if the dot velocity is more then 0.1, slow it by 10%
};

class Button
{
public:
  const byte pin;
  bool last;

  Button(byte pin_); //Digital pin for pullup button
  bool pressed(); //return true when state changes to pressed
};

//initiate buttons
Button buttonBlueDefend(9); 
Button buttonBlueAttack(10);
Button buttonRedDefend(11);
Button buttonRedAttack(12);

//initiate dots
DotObject dotsBlue[DOT_ARRAY_SIZE];
DotObject dotsRed[DOT_ARRAY_SIZE];

void setup()
{
  randomSeed(analogRead(0));
  
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  delay(250);
  
  //Initialize Strips
  strip.begin();

  //display the board markers
  strip.setPixelColor(0, DotObject::Color(255,0,0));
  strip.setPixelColor(strip.numPixels() / 2, DotObject::Color(255,255,255));
  strip.setPixelColor(strip.numPixels() - 1, DotObject::Color(0,0,255));
    
  strip.show();

  // initialize the LCD
  initaliseLCD();

  delay(1000);

  startNewGame();
}

void loop()
{
  strip.clear();

  if(gameActive)
  {
    //Button press detection
    int inactiveBlue = dotIndexInactive(dotsBlue);
    if (inactiveBlue != -1) {
      if (buttonBlueDefend.pressed()) {
        dotsBlue[inactiveBlue].activate(false, false);
      }
      if (buttonBlueAttack.pressed()) {
        dotsBlue[inactiveBlue].activate(false, true);
      }
    }

    //Serial.print(digitalRead(12));
    //Serial.print(' ');
    //Serial.println(digitalRead(11));
  
    int inactiveRed = dotIndexInactive(dotsRed);
    if (inactiveRed != -1) {
      if (buttonRedDefend.pressed()) {
        dotsRed[inactiveRed].activate(true, false);
      }
      if (buttonRedAttack.pressed()) {
        dotsRed[inactiveRed].activate(true, true);
      }
    }
    
    runDotsLoop();
    collisionDetection();

    // display the board markers - team colours at the ends and white in the middle
    strip.setPixelColor(0, DotObject::Color(255,0,0));
    strip.setPixelColor(strip.numPixels() / 2, DotObject::Color(255,255,255));
    strip.setPixelColor(strip.numPixels() - 1, DotObject::Color(0,0,255));

    checkForWin();
  }
  else
  {
    //check for button press to start new game perhaps coin mech?
    startNewGame();
  }
  
  strip.show();
  
  //delay ms
  delay(15);
}

void checkForWin()
{
  if (scoreBlue == 0) { winner(true); }
  if (scoreRed == 0) { winner(false); }
}

void winner(bool teamRed) //is the winner red?
{
  gameActive = false;
  writeWinner(teamRed);
  
  uint32_t col = teamRed ? DotObject::Color(255,0,0) : DotObject::Color(0,0,255);

  for (int i = 0; i != strip.numPixels()/2; ++i) {
    strip.setPixelColor(i, DotObject::brightness(col, i / 150.0));
    strip.setPixelColor(strip.numPixels() - i, DotObject::brightness(col, i / 150.0));
    strip.show();
    delay(50);
  }
  delay(1000);
}

void startNewGame()
{
  scoreRed = totalScore;
  scoreBlue = totalScore;
  for (int i = 0; i != DOT_ARRAY_SIZE; ++i) {
    dotsRed[i].active = false;
    dotsBlue[i].active = false;
  }
  typeTextBoth(intro1, intro2);
  delay(screenDelay);
  
  typeTextBoth(intro3, intro4);
  delay(screenDelay);
  
  /*typeTextSeperate(intro5red, intro6red, intro5blue, intro6blue);
  redLCD.print(space); delay(charDelay); redLCD.write(6); delay(charDelay);//crosshair
  blueLCD.print(space); delay(charDelay); blueLCD.write(6); delay(charDelay);//crosshair
  delay(screenDelay);
  
  typeTextBoth(intro7, intro8);
  redLCD.print(space); delay(charDelay); redLCD.write(8); delay(charDelay);//sheild
  blueLCD.print(space); delay(charDelay); blueLCD.write(8); delay(charDelay);//sheild
  delay(screenDelay);*/
  /*
  typeTextSeperate(intro9red, intro10red, intro9blue, intro10blue);
  delay(screenDelay);*/
 
  typeTextBoth(intro11, intro12);
  
  
  gameActive = true;
  writeScores();
}

