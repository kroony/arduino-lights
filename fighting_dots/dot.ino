int dotIndexInactive(DotObject* dots) {
  for (byte i = 0; i != DOT_ARRAY_SIZE; ++i) {
    if (!dots[i].active) return i;
  }
  return -1;
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

