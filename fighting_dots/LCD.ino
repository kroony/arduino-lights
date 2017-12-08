//Custom Characters for LCD
uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t emptyHeart[8]  = {0x00,0x0A,0x15,0x11,0x0A,0x04,0x00,0x00};
uint8_t cross[8] = {0x04,0x04,0x1F,0x04,0x04,0x04,0x04,0x00};
uint8_t flag[8] = {0x1C,0x1F,0x1F,0x13,0x10,0x10,0x10,0x00};
uint8_t clip[8]  = {0x1C,0x14,0x14,0x12,0x0A,0x0A,0x0F,0x00};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t crosshair[8] = {0x00, 0x0E, 0x15, 0x1F, 0x15, 0x0E, 0x00, 0x00};
uint8_t bullet[8] = {0x0E, 0x1F, 0x1D, 0x1D, 0x1F, 0x1F, 0x0E, 0x1F};

char space = ' ';

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
  // initialize the LCD
  lcd.init();
  lcd.begin(16, 2);
  delay(100);
  lcd.clear();
  lcd.backlight();

  lcd.createChar(0, bell);
  lcd.createChar(1, emptyHeart);
  lcd.createChar(2, cross);
  lcd.createChar(3, flag);
  lcd.createChar(4, clip);
  lcd.createChar(5, heart);
  lcd.createChar(6, crosshair);
  lcd.createChar(7, bullet);
  lcd.createChar(8, shield);
  lcd.home();
}

void typeText(String line1, String line2)
{
  lcd.home();
  lcd.clear();
  lcd.blink();
  for(byte i=0; i<line1.length(); i++)
  {
    lcd.print(line1.charAt(i));
    delay(250);
  }
  
  lcd.setCursor(0, 1);
  for(byte i=0; i<line2.length(); i++)
  {
    lcd.print(line2.charAt(i));
    delay(250);
  }

  lcd.noBlink();
}

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
  lcd.clear();
  lcd.home();

  lcd.write(5);
  lcd.print("Red ");
  if(scoreRed < 10) {lcd.print(space);}
  lcd.print(String(scoreRed));
  
  lcd.print("/");
  if(scoreBlue < 10) {lcd.print(space);}
  lcd.print(String(scoreBlue) + " Blue");
  lcd.write(5);
  lcd.setCursor(0, 1);

  printDotInfo(dotsRed[0]);
  printDotInfo(dotsRed[1]);

  lcd.print(" / ");

  printDotInfo(dotsBlue[0]);
  printDotInfo(dotsBlue[1]);
}

void writeWinner(bool teamRed)
{
  lcd.clear();
  lcd.home();

  if(teamRed)
  {
    lcd.print("Congratulations!");
    lcd.setCursor(0, 1);
    lcd.print(" Red Team Wins!");
  } else {
    lcd.print("Congratulations!");
    lcd.setCursor(0, 1);
    lcd.print(" Blue Team Wins");
  }
}

void printDotInfo(DotObject dot)
{
  if(dot.active)
  {
    if(dot.attack){ 
      lcd.print(space); lcd.write(6); lcd.print(space);
    } else{ 
      lcd.print(space); lcd.write(8); lcd.print(space);
    }
  }
  else
  {
    lcd.print(" - ");
  }
}


