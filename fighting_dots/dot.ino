
void DotObject::slowdown() //if the dot velocity is more then 0.1, slow it by 10%
{
  if (fabs(velocity) > 0.1) {
    velocity *= 0.9;
  }
}

void DotObject::activate(bool teamRed_, bool attack_)
{
  active = true;
  attack = attack_;
  teamRed = teamRed_;
  megaDot = false;
  if(random(1,1000) < 5) {
    megaDot = true;
  }
  megaDot

  velocity = random(5, 31)/10.0;
  if (!teamRed) {
    velocity *= -1;
  }
  location = teamRed ? 0 + scoreRed : strip.numPixels() - scoreBlue;//set the start location, offset by the scores
  writeScores();
}

static uint32_t DotObject::brightness(uint32_t colour, float bright)//return 32bit int colour at brightness 0 - 255
{
  bright *= bright;
  byte b = colour & 255;
  byte g = (colour >> 8) & 255;
  byte r = (colour >> 16) & 255;
  return Color(r * bright, g * bright, b * bright);
}

static uint32_t DotObject::Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

void DotObject::loop()
{
  if (active) {// if the dot is active update it. otherwise do nothing

    //move the dot based on its velocity
    location = location + velocity;

    int locationRounded = round(location);
    byte trailLength = abs(round(velocity)) + 3;
    int trailDirection = velocity > 0 ? -1 : 1;
    float trailBright = 1 / (float)trailLength;

    uint32_t dotColor; //set the dot colour
    if (!attack)      { dotColor = Color(  0, 255,   0); } //green for defence
    else if (teamRed) { dotColor = Color(255,   0,   0); } //red for red attack
    else              { dotColor = Color(  0,   0, 255); } //blue for blue attack

    for (byte i = 0; i < trailLength; ++i) { //set the fading brightness trail of the dot
      uint32_t trailColor = brightness(dotColor, 1 - (trailBright * i));
      int trailLocation = locationRounded + (i * trailDirection);
      strip.setPixelColor(trailLocation, trailColor);
    }
    
    //change direction of dot when it gets to the end of the strip
    if (attack) {
      if (location > strip.numPixels() - (scoreBlue + 1) && !teamRed) { //if a blue attack dot moves off the board on blue base side
        velocity = -fabs(velocity); //bounce back
      } else if (location < 0 + scoreRed && teamRed) { //if a red attack dot moves off the board on red base side
        velocity = fabs(velocity); //bounce back
      } else if (location > strip.numPixels() - (1 + scoreBlue) && teamRed) { //if a red dot makes it to blue base
        // increment score counter for red and deactivate dot
        active = false;
        scoreBlue--;
        writeScores();
      } else if (location < 0 + scoreRed && !teamRed) { //if a blue dot makes it to the red base
        // increment score counter for blue and deactivate dot
        active = false;
        scoreRed--;
        writeScores();
      }
    } else { //bounce defence dot back at mid way point
      if (location < strip.numPixels()/2+1 && !teamRed) { //if the blue defence dot location is over the half way from blue start
        velocity = fabs(velocity);
      } else if (location > strip.numPixels()/2-1 && teamRed) { //if the red defence dot location is over the half way from red start
        velocity = -fabs(velocity);
      } else if (location > strip.numPixels()-1 && !teamRed) { //if blue defence moves off board on blue side
        // dead defense
        active = false;
        writeScores();
      } else if (location < 0 && teamRed) { //if red defence moves off board on red side
        // dead defense
        active = false;
        writeScores();
      }
    }
  }
}

int dotIndexInactive(DotObject* dots) { //get index of inactive dot
  for (byte i = 0; i != DOT_ARRAY_SIZE; ++i) {
    if (!dots[i].active) return i;
  }
  return -1;
}

void setFieldPixels()
{
  for(byte i = 0; i < scoreRed; i++) //
  {
    strip.setPixelColor(i, DotObject::Color(255,0,0));
  }
  
  strip.setPixelColor(strip.numPixels() / 2, DotObject::Color(255,255,255));

  for(byte i = 0; i < scoreBlue; i++)
  {
    strip.setPixelColor(strip.numPixels() - i + 1, DotObject::Color(0,0,255));
  }
}

byte dotsCountActive(DotObject* dots) {
  byte active = 0;
  for (byte i = 0; i != DOT_ARRAY_SIZE; ++i) {
    if (dots[i].active) active++;
  }
  return active;
}

bool testOverlap(int x1, int x2, int y1, int y2) {
  return (x1 >= y1 && x1 <= y2) ||
         (x2 >= y1 && x2 <= y2) ||
         (y1 >= x1 && y1 <= x2) ||
         (y2 >= x1 && y2 <= x2);
}

void runDotsLoop()
{
  for(byte x = 0; x < DOT_ARRAY_SIZE; x++) {
    dotsBlue[x].loop();
    dotsRed[x].loop();
  }
}

void collisionDetection()
{
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

        if(testOverlap(min(loc1_o, loc1_n), max(loc1_o, loc1_n), min(loc2_o, loc2_n), max(loc2_o, loc2_n))){//do the dots move in or through the same space next tick?
          if (blue.attack != red.attack) { //are the dots different? 
            if (signbit(blue.velocity) == signbit(red.velocity)) { //if they are moving in the same direction
              if (blue.attack) {
                blue.slowdown(); //slowdown blue attack when it moves over a retreating red defence dot
              } else {
                red.slowdown(); //slowdown red attack when it moves over a retreating blue defence dot
              }
            } else { //the are moving towards each other
              if (blue.attack) { //red defended successfully, destroy blue attack dot, turn red defence around and into an attack dot
                blue.active = false;
                red.attack = true;
                red.velocity *= -1;
              } else { //blue defended successfully, destroy red attack dot, turn blue defence around and into an attack dot
                red.active = false;
                blue.attack = true;
                blue.velocity *= -1;
              }
            }
          } else { //the dots are the same (attack + attack because defence cant interact past midpoint)
            red.slowdown();
            blue.slowdown();
          }
        }
      }
    }
  }
}

