//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, 6, NEO_GRB + NEO_KHZ800);

//Includes for 16x2 LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x3f for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void displayDigit(int number);
void writeScores();

const byte totalScore = 20;

byte scoreBlue = totalScore;
byte scoreRed = totalScore;
byte previousBlueScore = totalScore;
byte previousRedScore = totalScore;

bool gameActive = false;

const byte DOT_ARRAY_SIZE = 2;

class DotObject
{
  public:
    bool active = false;
    double location;
    double velocity;
    bool teamRed;
    bool attack;
    
    static uint32_t Color(byte r, byte g, byte b)
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
      bright *= bright;
      byte b = colour & 255;
      byte g = (colour >> 8) & 255;
      byte r = (colour >> 16) & 255;
      return Color(r * bright, g * bright, b * bright);
    }
    
    void activate(bool teamRed_, bool attack_) {
      active = true;
      attack = attack_;
      teamRed = teamRed_;

      velocity = random(1, 31)/10.0;
      if (!teamRed) {
        velocity *= -1;
      }
      location = teamRed ? 0 : 300;
      writeScores();
    }

    void loop() {
      if (active) {
        location = location + velocity;

        int locationRounded = round(location);
        byte trailLength = abs(round(velocity)) + 3;
        int trailDirection = velocity > 0 ? -1 : 1;
        float trailBright = 1 / (float)trailLength;
        
        uint32_t dotColor;
        if (!attack) {
          dotColor = Color(0, 255, 0);
        } else if (teamRed) {
          dotColor = Color(255, 0, 0);
        } else {
          dotColor = Color(0, 0, 255);
        }

        for (int i = 0; i < trailLength; ++i) {
          uint32_t trailColor = brightness(dotColor, 1 - (trailBright * i));
          int trailLocation = locationRounded + (i * trailDirection);
          strip.setPixelColor(trailLocation, trailColor);
        }
        
        //change direction of dot when it gets to the end of the strip
        if (attack) {
          if (location > 299 && !teamRed) {
            velocity = -fabs(velocity);
          } else if (location < 0 && teamRed) {
            velocity = fabs(velocity);
          } else if (location > 299 && teamRed) {
            // increment score counter for red
            active = false;
            scoreBlue--;
            writeScores();
            
          } else if (location < 0 && !teamRed) {
            // increment score counter for blue
            active = false;
            scoreRed--;
            writeScores();
          }
        } else {
          if (location < 151 && !teamRed) {
            velocity = fabs(velocity);
          } else if (location > 149 && teamRed) {
            velocity = -fabs(velocity);
          } else if (location > 299 && !teamRed) {
            // dead defense
            active = false;
            writeScores();
          } else if (location < 0 && teamRed) {
            // dead defense
            active = false;
            writeScores();
          }
        }
      }
    }

    void slowdown()
    {
      if (fabs(velocity) > 0.1) {
        velocity *= 0.9;
      }
    }
};

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
  strip.show();

  // initialize the LCD
  initaliseLCD();

  startNewGame();
}

void loop()
{
  strip.clear();

  int inactiveBlue = dotIndexInactive(dotsBlue);
  /*if (inactiveBlue != -1) {
    if (buttonBlueDefend.pressed()) {
      dotsBlue[inactiveBlue].activate(false, false);
    }
    if (buttonBlueAttack.pressed()) {
      dotsBlue[inactiveBlue].activate(false, true);
    }
  }*/

  int inactiveRed = dotIndexInactive(dotsRed);
  /*if (inactiveRed != -1) {
    if (buttonRedDefend.pressed()) {
      dotsRed[inactiveRed].activate(true, false);
    }
    if (buttonRedAttack.pressed()) {
      dotsRed[inactiveRed].activate(true, true);
    }
  }*/

  if(gameActive)
  {
    runDotsLoop();
    collisionDetection();

    // display the board markers
    strip.setPixelColor(0, DotObject::Color(255,0,0));
    strip.setPixelColor(150, DotObject::Color(255,255,255));
    strip.setPixelColor(299, DotObject::Color(0,0,255));

    checkForWin();
    
    if (random(0,100) == 0 && inactiveBlue != -1) {//random create a blue dot
      dotsBlue[inactiveBlue].activate(false, random(0,2));
    }
  
    if (random(0,100) == 0 && inactiveRed != -1) {//random create a red dot
      dotsRed[inactiveRed].activate(true, random(0,2));
    }
  
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

void winner(bool teamRed)
{
  gameActive = false;
  writeWinner(teamRed);
  
  uint32_t col = teamRed ? DotObject::Color(255,0,0) : DotObject::Color(0,0,255);

  for (int i = 0; i != 150; ++i) {
    strip.setPixelColor(i, DotObject::brightness(col, i / 150.0));
    strip.setPixelColor(299 - i, DotObject::brightness(col, i / 150.0));
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

  typeText("Fighting Dots", "A Line Game");
  delay(500);
  typeText("Made By Amos", "& Bill");
  delay(1000);
  typeText("New Game In", "3...2...1...");
  
  gameActive = true;
  writeScores();
}

