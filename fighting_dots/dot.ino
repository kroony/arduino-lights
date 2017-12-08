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
}

