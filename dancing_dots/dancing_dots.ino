//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, 6, NEO_GRB + NEO_KHZ800);

int outputCount = 0;

bool wasHighLast = false;

int active;







class DotObject
{
  public:
    bool active = false;
    double location;
    byte color256;
    double velocity;
    bool explosion = false;
    int explosionLocation;
    byte explosionTimer = 0;
    byte collisions = 0;

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

    uint32_t Wheel(byte WheelPos)
    {
      if (WheelPos < 85) {
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      } else if (WheelPos < 170) {
        WheelPos -= 85;
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
      } else {
        WheelPos -= 170;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
    }

    uint32_t WheelBrightness(byte WheelPos, byte brightness)
    {
      if (WheelPos < 85) {
        return Color(brightness * (WheelPos * 3) / 255,
                     brightness * (255 - WheelPos * 3) / 255,
                     0);
      } else if (WheelPos < 170) {
        WheelPos -= 85;
        return Color(brightness * (255 - WheelPos * 3) / 255,
                     0,
                     brightness * (WheelPos * 3) / 255);
      } else {
        WheelPos -= 170;
        return Color(0,
                     brightness * (WheelPos * 3) / 255,
                     brightness * (255 - WheelPos * 3) / 255);
      }
    }

    void activate() {
      active = true;
      color256 = random(0, 256);
      Serial.print(" - New Dot: Color: ");
      Serial.print(color256);
      velocity = random(1, 31)/10.0;
      Serial.print(", Velocity ");
      Serial.println(velocity);
      location = random(0, 2) * 300; //start at either end
      if (location == 300) {
        color256 = 1;
      } else {
        color256 = 170;
      }
      collisions = 0;
    }

    bool addCollision() {//returns true if it explodes
      collisions++;
      if (collisions == random(collisions, 51)) {
        explode();
        return true;
      }
      return false;
    }

    void checkForLost() {
      if(active) {
        if(location < -3 || location > 303){
          explode();
        }
      }
    }

    void explode() {
      explosion = true;
      explosionLocation = round(location);
      explosionTimer = 10;
      velocity = 0; // stop moving dot
      location = 350; // move dot far away enough that more explosions dont make weirdness on the ends
      active = false;

      //single frame explosion + and - 10 pixel from the last location
      strip.setPixelColor(explosionLocation + 10, WheelBrightness(color256, 25));
      strip.setPixelColor(explosionLocation + 9, WheelBrightness(color256, 50));
      strip.setPixelColor(explosionLocation + 8, WheelBrightness(color256, 75));
      strip.setPixelColor(explosionLocation + 7, WheelBrightness(color256, 100));
      strip.setPixelColor(explosionLocation + 6, WheelBrightness(color256, 125));
      strip.setPixelColor(explosionLocation + 5, WheelBrightness(color256, 150));
      strip.setPixelColor(explosionLocation + 4, WheelBrightness(color256, 175));
      strip.setPixelColor(explosionLocation + 3, WheelBrightness(color256, 200));
      strip.setPixelColor(explosionLocation + 2, WheelBrightness(color256, 225));
      strip.setPixelColor(explosionLocation + 1, WheelBrightness(color256, 250));

      strip.setPixelColor(explosionLocation, WheelBrightness(Color(255, 255, 255), 255));

      strip.setPixelColor(explosionLocation - 10, WheelBrightness(color256, 25));
      strip.setPixelColor(explosionLocation - 9, WheelBrightness(color256, 50));
      strip.setPixelColor(explosionLocation - 8, WheelBrightness(color256, 75));
      strip.setPixelColor(explosionLocation - 7, WheelBrightness(color256, 100));
      strip.setPixelColor(explosionLocation - 6, WheelBrightness(color256, 125));
      strip.setPixelColor(explosionLocation - 5, WheelBrightness(color256, 150));
      strip.setPixelColor(explosionLocation - 4, WheelBrightness(color256, 175));
      strip.setPixelColor(explosionLocation - 3, WheelBrightness(color256, 200));
      strip.setPixelColor(explosionLocation - 2, WheelBrightness(color256, 225));
      strip.setPixelColor(explosionLocation - 1, WheelBrightness(color256, 250));
    }

    void loop() {
      if (active) {
        location = location + velocity;

        //change direction of dot when it gets to the end of the strip
        if ((location > 300 && velocity > 0) || (location < 0 && velocity < 0)) {
          velocity = velocity * -1;
        }

        //set the dot color itself
        strip.setPixelColor(round(location), Wheel(color256));

        //add a dynamic length pixel trail
        if (velocity > 0) {//negative trail
          if       (velocity >= 2) {
            strip.setPixelColor(round(location - 1), WheelBrightness(color256, 200));
            strip.setPixelColor(round(location - 2), WheelBrightness(color256, 150));
            strip.setPixelColor(round(location - 3), WheelBrightness(color256, 100));
            strip.setPixelColor(round(location - 4), WheelBrightness(color256, 50));
          } else if(velocity >= 1) {
            strip.setPixelColor(round(location - 1), WheelBrightness(color256, 150));
            strip.setPixelColor(round(location - 2), WheelBrightness(color256, 100));
            strip.setPixelColor(round(location - 3), WheelBrightness(color256, 50));
          } else if(velocity > 0) {
            strip.setPixelColor(round(location - 1), WheelBrightness(color256, 100));
            strip.setPixelColor(round(location - 2), WheelBrightness(color256, 50));
          }
        }
        else if (velocity < 0) {//positive trail
          if       (velocity <= -2) {
            strip.setPixelColor(round(location + 1), WheelBrightness(color256, 200));
            strip.setPixelColor(round(location + 2), WheelBrightness(color256, 150));
            strip.setPixelColor(round(location + 3), WheelBrightness(color256, 100));
            strip.setPixelColor(round(location + 4), WheelBrightness(color256, 50));
          } else if(velocity <= -1) {
            strip.setPixelColor(round(location + 1), WheelBrightness(color256, 150));
            strip.setPixelColor(round(location + 2), WheelBrightness(color256, 100));
            strip.setPixelColor(round(location + 3), WheelBrightness(color256, 50));
          } else if(velocity < 0) {
            strip.setPixelColor(round(location + 1), WheelBrightness(color256, 100));
            strip.setPixelColor(round(location + 2), WheelBrightness(color256, 50));
          }
        }
        checkForLost();
      }
      if (explosion) {//do the after explode shock wave
        strip.setPixelColor(explosionLocation + (10 - explosionTimer), WheelBrightness(color256, 25 * explosionTimer));
        strip.setPixelColor(explosionLocation - (10 - explosionTimer), WheelBrightness(color256, 25 * explosionTimer));
        explosionTimer--;
        if (explosionTimer == 0) {
          explosion = false;
          active = false;
        }
      }
    }
};








//initiate dots
DotObject dotArray[20];

void setup()
{
  randomSeed(analogRead(0));
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  Serial.println("Hello");
  delay(250);
  //Initialize Strip
  strip.begin();
  strip.show();

  digitDisplaySetup();
}

void loop()
{
  strip.clear();

  if (random(0, 1000) < (20 - active)) {//when port 10 pressed, try activate an inactive dot
    if (!wasHighLast) {
      bool activatedSomething = 0;
      for(byte x = 0; x < 20; x++) {
        if(!dotArray[x].active) {
          dotArray[x].activate();
          x = 20;
          activatedSomething = 1;
        }
      }
      if(!activatedSomething) {
        Serial.println("At Max Dots!");
      }
    }
    wasHighLast = true;//check for new press of button not just hold down.
  } else {
    wasHighLast = false;
  }

  //run each dots loop
  active = 0;
  for(byte x = 0; x < 20; x++) {
    dotArray[x].loop();
    if (dotArray[x].active) active++;
  }
  displayDigit(active);

  //collision detection
  for(byte x = 0; x != 20; ++x) {
    for(byte y = 0; y != x; ++y) {
      if(dotArray[x].active && dotArray[y].active){//dont check unless they are both active
        double loc1_o = dotArray[x].location;
        double loc1_n = loc1_o + dotArray[x].velocity;
        double loc2_o = dotArray[y].location;
        double loc2_n = loc2_o + dotArray[y].velocity;

        if(testOverlap(min(loc1_o, loc1_n), max(loc1_o, loc1_n), min(loc2_o, loc2_n), max(loc2_o, loc2_n))){
          
          if(signbit(dotArray[x].velocity) == signbit(dotArray[y].velocity))
          {//traveling in the same direction
            //Serial.println("same direction collision");
            //swap velocity
            double tmpVelocity = dotArray[x].velocity;
            dotArray[x].velocity = dotArray[y].velocity;
            dotArray[y].velocity = tmpVelocity;

            //change color a little bit
            dotArray[x].color256 = shiftColor(dotArray[x].color256, 2);
            dotArray[y].color256 = shiftColor(dotArray[y].color256, 2);

            //add to collision count
            //dotArray[x].addCollision();
            //dotArray[y].addCollision();
          } 
          else 
          { //traveling towards each other

            //change color a little bit more
            dotArray[x].color256 = shiftColor(dotArray[x].color256, 4);
            dotArray[y].color256 = shiftColor(dotArray[y].color256, 4);

            //add to collision count and turn around if there is no explosion
            if(!dotArray[x].addCollision()) {dotArray[y].velocity = dotArray[y].velocity * -1;} else {dotArray[y].collisions = 0;}
            if(!dotArray[y].addCollision()) {dotArray[x].velocity = dotArray[x].velocity * -1;} else {dotArray[x].collisions = 0;}
            
            //Serial.println("opposing collision");
          }
        }
      }
    }
  }

  //push light array out
  strip.show();
  
  //delay ms
  delay(35);

  //output debug
  outputCount++;

  //outputDebug(outputCount % 20);
  if(outputCount > 500) {
    outputCount = 0;
    
    for(byte x = 0; x < 20; x++) {
      if(!dotArray[x].active) {
        dotArray[x].activate();
        x = 20;
      }
    }
  }
}

bool overlap(double a, double b, double c) {
 return min(b,c) > a && b < max(b,c);
}

bool testOverlap(int x1, int x2, int y1, int y2) {
  return (x1 >= y1 && x1 <= y2) ||
         (x2 >= y1 && x2 <= y2) ||
         (y1 >= x1 && y1 <= x2) ||
         (y2 >= x1 && y2 <= x2);
}

int shiftColor(byte color, byte shift)
{
  color = color + shift;
  if (color > 255)
  {
    color = color - 255;
  }
  return color;
}

void outputDebug(int x) {
  /*if (dotArray[x].active) {
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
  }*/
}




