//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, 6, NEO_GRB + NEO_KHZ800);

void displayDigit(int number);

const byte totalScore = 19;

byte scoreBlue = totalScore;
byte scoreRed = totalScore;

class Button
{
public:
  const byte pin;
  bool last;

  Button(byte pin_) : pin(pin_), last(false)
  { }

  bool pressed()
  {
    if (digitalRead(pin) == HIGH)
    {
      if (last) return false;
      last = true;
      return true;
    } else {
      last = false;
      return false;
    }
  }
};

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
            
            Serial.println("Red scores!");
            Serial.print(scoreRed);
            Serial.print(" R / B ");
            Serial.println(scoreBlue);
          } else if (location < 0 && !teamRed) {
            // increment score counter for blue
            active = false;
            scoreRed--;

            Serial.println("Blue scores!");
            Serial.print(scoreRed);
            Serial.print(" R / B ");
            Serial.println(scoreBlue);
          }
        } else {
          if (location < 151 && !teamRed) {
            velocity = fabs(velocity);
          } else if (location > 149 && teamRed) {
            velocity = -fabs(velocity);
          } else if (location > 299 && !teamRed) {
            // dead defense
            active = false;
          } else if (location < 0 && teamRed) {
            // dead defense
            active = false;
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



int outputCount = 0;

const byte DOT_ARRAY_SIZE = 2;
//initiate dots
DotObject dotsBlue[DOT_ARRAY_SIZE];
DotObject dotsRed[DOT_ARRAY_SIZE];

void setup()
{
  randomSeed(analogRead(0));
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  delay(250);
  //Initialize Strip
  strip.begin();
  strip.show();

  digitDisplaySetup();
}


Button buttonBlueDefend(1), buttonBlueAttack(2),
       buttonRedDefend(3), buttonRedAttack(4);

byte dotsCountActive(DotObject* dots) {
  byte active = 0;
  for (byte i = 0; i != DOT_ARRAY_SIZE; ++i) {
    if (dots[i].active) active++;
  }
  return active;
}

int dotIndexInactive(DotObject* dots) {
  for (byte i = 0; i != DOT_ARRAY_SIZE; ++i) {
    if (!dots[i].active) return i;
  }
  return -1;
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

  int inactiveRed= dotIndexInactive(dotsRed);
  /*if (inactiveRed != -1) {
    if (buttonRedDefend.pressed()) {
      dotsRed[inactiveRed].activate(true, false);
    }
    if (buttonRedAttack.pressed()) {
      dotsRed[inactiveRed].activate(true, true);
    }
  }*/

  //run each dots loop
  for(byte x = 0; x < DOT_ARRAY_SIZE; x++) {
    dotsBlue[x].loop();
    dotsRed[x].loop();
  }
  displayDigit(scoreRed);

  //collision detection
  for(byte x = 0; x != DOT_ARRAY_SIZE; ++x) {
    for(byte y = 0; y != DOT_ARRAY_SIZE; ++y) {
      DotObject &blue = dotsBlue[x];
      DotObject &red  = dotsRed[y];
      if(blue.active && red.active){//dont check unless they are both active
        double loc1_o = blue.location;
        double loc1_n = loc1_o + blue.velocity;
        double loc2_o = red.location;
        double loc2_n = loc2_o + red.velocity;

        if(testOverlap(min(loc1_o, loc1_n), max(loc1_o, loc1_n), min(loc2_o, loc2_n), max(loc2_o, loc2_n))){
          if (blue.attack != red.attack) {
            if (signbit(blue.velocity) == signbit(red.velocity)) {
              if (blue.attack) {
                blue.slowdown();
              } else {
                red.slowdown();
              }
            } else {
              if (blue.attack) {
                blue.active = false;
                red.attack = true;
                red.velocity *= -1;
              } else {
                red.active = false;
                blue.attack = true;
                blue.velocity *= -1;
              }
            }
          } else {
            red.slowdown();
            blue.slowdown();
          }
        }
      }
    }
  }
  

  //push light array out
  strip.setPixelColor(0, DotObject::Color(255,0,0));
  strip.setPixelColor(150, DotObject::Color(255,255,255));
  strip.setPixelColor(299, DotObject::Color(0,0,255));
  strip.show();
  
  //delay ms
  delay(15);

  //output debug
  outputCount++;

  //outputDebug(outputCount % 20);
  if(outputCount > 500) {
    outputCount = 0;
  }
  
    if (random(0,100) == 0 && inactiveBlue != -1) {
      dotsBlue[inactiveBlue].activate(false, random(0,2));
    }

    if (random(0,100) == 0 && inactiveRed != -1) {
      dotsRed[inactiveRed].activate(true, random(0,2));
    }

  if (scoreBlue == 0) {
    winner(true);
  }

  if (scoreRed == 0) {
    winner(false);
  }

}

void winner(bool teamRed)
{
  uint32_t col = teamRed ? DotObject::Color(255,0,0) : DotObject::Color(0,0,255);

  for (int i = 0; i != 150; ++i) {
    strip.setPixelColor(i, DotObject::brightness(col, i / 150.0));
    strip.setPixelColor(299 - i, DotObject::brightness(col, i / 150.0));
    strip.show();

    displayDigit(scoreRed);
    delay(50);
  }
  delay(1000);

  scoreRed = totalScore;
  scoreBlue = totalScore;
  for (int i = 0; i != DOT_ARRAY_SIZE; ++i) {
    dotsRed[i].active = false;
    dotsBlue[i].active = false;
  }
}

bool testOverlap(int x1, int x2, int y1, int y2) {
  return (x1 >= y1 && x1 <= y2) ||
         (x2 >= y1 && x2 <= y2) ||
         (y1 >= x1 && y1 <= x2) ||
         (y2 >= x1 && y2 <= x2);
}
/*
void outputDebug(int x) {
  if (dotArray[x].active) {
    Serial.print("Dot ");
    Serial.print(x);
    Serial.print(" - Active: ");
    Serial.print(dotArray[x].active);
    Serial.print(", Collisions: ");
    Serial.print(dotArray[x].collisions);
    Serial.print(", Location: ");
    Serial.println(dotArray[x].location);
  } else {
    Serial.print("");
  }
}
*/
