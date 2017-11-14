
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

