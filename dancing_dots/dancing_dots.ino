//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, 6, NEO_GRB + NEO_KHZ800);

bool wasHighLast = false;

class DotObject
{
  public:
    bool active = false;
    double location;
    byte color256;
    double velocity;
    uint32_t color;
    bool explosion = false;
    int explosionLocation;
    byte explosionTimer = 0;

    int GetNextColor()
    {
      color256++;
      if (color256 > 255)
      {
        color256 = 0;
      }
      return color256;
    }

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
      Serial.print("Color ");
      Serial.println(color256);
      color = Wheel(color256);
      velocity = random(1, 31)/10.0;
      Serial.print("velocity ");
      Serial.println(velocity);
      location = 0;
    }

    void loop() {
      if (active) {
        location = location + velocity;

        //change direction of dot when it gets to the end of the strip
        if (location > 300 || location < 0) {
          velocity = velocity * -1;
        }

        //set the dot color itself
        strip.setPixelColor(round(location), Wheel(color256));

        //add a 2 pixel trail
        if (velocity > 0) {
          strip.setPixelColor(round(location - 1), WheelBrightness(color256, 100));
          strip.setPixelColor(round(location - 2), WheelBrightness(color256, 50));
        }
        else if (velocity < 0) {
          strip.setPixelColor(round(location + 1), WheelBrightness(color256, 100));
          strip.setPixelColor(round(location + 2), WheelBrightness(color256, 50));
        }

        //explode the dot and decative it randomly on in 300
        if (random(1, 300) == 5) {
          explosion = true;
          explosionLocation = round(location);
          explosionTimer = 10;
          velocity = 0; // stop moving dot
          location = 350; // move dot far away enough that more explosions dont make weirdness on the ends

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

          strip.setPixelColor(explosionLocation, WheelBrightness(color256, 255));

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

        if (explosion) {
          strip.setPixelColor(explosionLocation + (10 - explosionTimer), WheelBrightness(color256, 25 * explosionTimer));
          strip.setPixelColor(explosionLocation - (10 - explosionTimer), WheelBrightness(color256, 25 * explosionTimer));
          explosionTimer--;
          if (explosionTimer == 0) {
            explosion = false;
            active = false;
          }
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
  delay(250);
  //Initialize Strip
  strip.begin();
  strip.show();
}

void loop()
{
  strip.clear();

  if (digitalRead(10) == HIGH) {//when port 10 pressed, try activate an inactive dot
    if (!wasHighLast) {
      for(byte x = 0; x < 20; x++) {
        if(!dotArray[x].active) {
          dotArray[x].activate();
          x = 20;
        }
      }
    }
    wasHighLast = true;//check for new press of button not just hold down.
  } else {
    wasHighLast = false;
  }

  //run each dots loop
  for(byte x = 0; x < 20; x++) {
    if(dotArray[x].active) {
      dotArray[x].loop();
    }
  }

  //push light array out
  strip.show();
  
  //delay ms
  delay(40);
}

