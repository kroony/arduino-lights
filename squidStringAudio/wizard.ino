void displayGhost()
{
  uint32_t white = Color(255, 255, 255);
  uint32_t black = Color(0, 0, 0);
  uint32_t eye = Color(0, 0, 255);

  byte randGhostColour = random(1, 5);
  uint32_t ghostColour = 0;
  if(randGhostColour == 1) {
    ghostColour = Color(255, 0, 0);
  } else if(randGhostColour == 2) {
    ghostColour = Color(0, 255, 0);
  } else if(randGhostColour == 3) {
    ghostColour = Color(0, 0, 255);
  } else {
    ghostColour = Color(255, 0, 255);
  }
  
  /*uint32_t cyan = Color(0, 204, 255);
  uint32_t orange = Color(255, 128, 0);
  uint32_t pink = Color(128, 64, 175);*/
  
  //strip.setBrightness(255);

  for(int8_t i=-64; i<64; i = i+16)
  {
    //row 1 top down
    strip.setPixelColor( 0+i, black);
    strip.setPixelColor( 1+i, black);
    strip.setPixelColor( 2+i, ghostColour);
    strip.setPixelColor( 3+i, ghostColour);
    strip.setPixelColor( 4+i, ghostColour);
    strip.setPixelColor( 5+i, ghostColour);
    strip.setPixelColor( 6+i, ghostColour);
    strip.setPixelColor( 7+i, ghostColour);
  
    //row 2 bottom up
    strip.setPixelColor( 8+i, ghostColour);
    strip.setPixelColor( 9+i, ghostColour);
    strip.setPixelColor(10+i, ghostColour);
    strip.setPixelColor(11+i, ghostColour);
    strip.setPixelColor(12+i, white);
    strip.setPixelColor(13+i, white);
    strip.setPixelColor(14+i, ghostColour);
    strip.setPixelColor(15+i, black);
  
    //row 3 top down
    strip.setPixelColor(16+i, ghostColour);
    strip.setPixelColor(17+i, ghostColour);
    strip.setPixelColor(18+i, white);
    strip.setPixelColor(19+i, eye);
    strip.setPixelColor(20+i, ghostColour);
    strip.setPixelColor(21+i, ghostColour);
    strip.setPixelColor(22+i, ghostColour);
    strip.setPixelColor(23+i, black);
    
    //row 4 bottom up
    strip.setPixelColor(24+i, ghostColour);
    strip.setPixelColor(25+i, ghostColour);
    strip.setPixelColor(26+i, ghostColour);
    strip.setPixelColor(27+i, ghostColour);
    strip.setPixelColor(28+i, ghostColour);
    strip.setPixelColor(29+i, ghostColour);
    strip.setPixelColor(30+i, ghostColour);
    strip.setPixelColor(31+i, ghostColour);
  
    //row 5 top down
    strip.setPixelColor(32+i, ghostColour);
    strip.setPixelColor(33+i, ghostColour);
    strip.setPixelColor(34+i, ghostColour);
    strip.setPixelColor(35+i, ghostColour);
    strip.setPixelColor(36+i, ghostColour);
    strip.setPixelColor(37+i, ghostColour);
    strip.setPixelColor(38+i, ghostColour);
    strip.setPixelColor(39+i, ghostColour);
  
    //row 6 bottom up
    strip.setPixelColor(40+i, black);
    strip.setPixelColor(41+i, ghostColour);
    strip.setPixelColor(42+i, ghostColour);
    strip.setPixelColor(43+i, ghostColour);
    strip.setPixelColor(44+i, white);
    strip.setPixelColor(45+i, white);
    strip.setPixelColor(46+i, ghostColour);
    strip.setPixelColor(47+i, ghostColour);
  
    //row 7 top down
    strip.setPixelColor(48+i, black);
    strip.setPixelColor(49+i, ghostColour);
    strip.setPixelColor(50+i, white);
    strip.setPixelColor(51+i, eye);
    strip.setPixelColor(52+i, ghostColour);
    strip.setPixelColor(53+i, ghostColour);
    strip.setPixelColor(54+i, ghostColour);
    strip.setPixelColor(55+i, ghostColour);
  
    //row 8 bottom up
    strip.setPixelColor(56+i, ghostColour);
    strip.setPixelColor(57+i, ghostColour);
    strip.setPixelColor(58+i, ghostColour);
    strip.setPixelColor(59+i, ghostColour);
    strip.setPixelColor(60+i, ghostColour);
    strip.setPixelColor(61+i, ghostColour);
    strip.setPixelColor(62+i, black);
    strip.setPixelColor(63+i, black);
    
    strip.show();
    delay(200);
    ClearStrip();
  }
  
  strip.setBrightness(255);
  ClearStrip();  
}




