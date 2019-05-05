//Custom Characters for LCD
//uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
//uint8_t emptyHeart[8]  = {0x00,0x0A,0x15,0x11,0x0A,0x04,0x00,0x00};
//uint8_t cross[8] = {0x04,0x04,0x1F,0x04,0x04,0x04,0x04,0x00};
//uint8_t flag[8] = {0x1C,0x1F,0x1F,0x13,0x10,0x10,0x10,0x00};
//uint8_t clip[8]  = {0x1C,0x14,0x14,0x12,0x0A,0x0A,0x0F,0x00};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t crosshair[8] = {0x00, 0x0E, 0x15, 0x1F, 0x15, 0x0E, 0x00, 0x00};
//uint8_t bullet[8] = {0x0E, 0x1F, 0x1D, 0x1D, 0x1F, 0x1F, 0x0E, 0x1F};

byte shield[8] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B00100,
  B00000,
};

//start animations
void initaliseLCD(){
  // initialize the LCDs
  redLCD.init();
  delay(100);
  blueLCD.init();
  delay(100);
  redLCD.begin(16, 2);
  delay(100);
  blueLCD.begin(16, 2);
  delay(100);
  
  redLCD.clear();
  blueLCD.clear();
  redLCD.backlight();
  blueLCD.backlight();

  //lcd.createChar(0, bell);
  //lcd.createChar(1, emptyHeart);
  //lcd.createChar(2, cross);
  //lcd.createChar(3, flag);
  //lcd.createChar(4, clip);
  redLCD.createChar(5, heart);
  blueLCD.createChar(5, heart);
  redLCD.createChar(6, crosshair);
  blueLCD.createChar(6, crosshair);
  //lcd.createChar(7, bullet);
  redLCD.createChar(8, shield);
  blueLCD.createChar(8, shield);
  redLCD.home();
  blueLCD.home();
}

void typeTextBoth(char *line1, char *line2)
{
  redLCD.home();
  blueLCD.home();
  redLCD.clear();
  blueLCD.clear();
  redLCD.blink();
  blueLCD.blink();
  for(byte i=0; i<screenWidth; i++)
  {
    if(line1[i] != NULL) //if the character isnt null, print it on screen
    { 
      redLCD.print(line1[i]);
      blueLCD.print(line1[i]);
      delay(250);
    }
  }
  
  redLCD.setCursor(0, 1);
  blueLCD.setCursor(0, 1);
  delay(250);
  for(byte i=0; i<screenWidth; i++)
  {
    if(line2[i] != NULL) //if the character isnt null, print it on screen
    {
      redLCD.print(line2[i]);
      blueLCD.print(line2[i]);
      delay(250);
    }
  }

  redLCD.noBlink();
  blueLCD.noBlink();
}
/*
void typeTextSeperate(char *redLine1, char *redLine2, char *blueLine1, char *blueLine2)
{
  redLCD.home();
  blueLCD.home();
  redLCD.clear();
  blueLCD.clear();
  redLCD.blink();
  blueLCD.blink();
  for(byte i=0; i<screenWidth; i++)
  {
    if(redLine1[i] != NULL || blueLine1[i] != NULL)
    {
      if(redLine1[i] != NULL) { redLCD.print(redLine1[i]); }
      if(blueLine1[i] != NULL) { blueLCD.print(blueLine1[i]); }
      delay(250);
    }
  }
  
  redLCD.setCursor(0, 1);
  blueLCD.setCursor(0, 1);
  delay(250);
  for(byte i=0; i<screenWidth; i++)
  {
    if(redLine2[i] != NULL || blueLine2[i] != NULL)
    {
      redLCD.print(redLine2[i]);
      blueLCD.print(blueLine2[i]);
      delay(250);
    }
  }

  redLCD.noBlink();
  blueLCD.noBlink();
}
*/
void updateLCD(){
  //check if scores have changed
  if(scoreBlue != previousBlueScore || scoreRed != previousRedScore)
  {
    writeScores();
    previousBlueScore = scoreBlue;
    previousRedScore = scoreRed;
  }
}

void writeScores()
{
  //clear screen
  redLCD.clear();
  blueLCD.clear();
  redLCD.home();
  blueLCD.home();

  //1st line
  redLCD.write(5); //heart
  blueLCD.write(5); //heart
  redLCD.print("Red ");
  blueLCD.print("Red ");
  if(scoreRed < 10) {redLCD.print(space);}
  if(scoreRed < 10) {blueLCD.print(space);}
  redLCD.print(String(scoreRed));
  blueLCD.print(String(scoreRed));
  
  redLCD.print("/");
  blueLCD.print("/");
  if(scoreBlue < 10) {redLCD.print(space);}
  if(scoreBlue < 10) {blueLCD.print(space);}
  redLCD.print(String(scoreBlue) + " Blue");
  blueLCD.print(String(scoreBlue) + " Blue");
  redLCD.write(5); //heart
  blueLCD.write(5); //heart

  //2nd line
  redLCD.setCursor(0, 1);
  blueLCD.setCursor(0, 1);

  printDotInfo(dotsRed[0]);
  printDotInfo(dotsRed[1]);
  printDotInfo(dotsRed[2]);

  redLCD.print(" /");
  blueLCD.print(" /");

  printDotInfo(dotsBlue[0]);
  printDotInfo(dotsBlue[1]);
  printDotInfo(dotsBlue[2]);
}

void writeWinner(bool teamRed)
{
  redLCD.clear();
  blueLCD.clear();
  redLCD.home();
  blueLCD.home();

  if(teamRed)
  {
    redLCD.print(win1);
    blueLCD.print(lose1);
    redLCD.setCursor(0, 1);
    blueLCD.setCursor(0, 1);
    redLCD.print(win2);
    blueLCD.print(lose2);
  } else {
    redLCD.print(lose1);
    blueLCD.print(win1);
    redLCD.setCursor(0, 1);
    blueLCD.setCursor(0, 1);
    redLCD.print(lose2);
    blueLCD.print(win2);
  }
}

void printDotInfo(DotObject dot)
{
  if(dot.active)
  {
    if(dot.attack){ 
      redLCD.print(space); redLCD.write(6); //crosshair
      blueLCD.print(space); blueLCD.write(6); //crosshair
    } else{ 
      redLCD.print(space); redLCD.write(8); //sheild
      blueLCD.print(space); blueLCD.write(8); //sheild
    }
  }
  else
  {
    redLCD.print(" -");
    blueLCD.print(" -");
  }
}


