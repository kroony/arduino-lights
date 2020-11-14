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
  // initialize the LCD screen
  LCD.init();
  delay(100);
  LCD.begin(16, 2);
  delay(100);
  
  LCD.clear();
  LCD.backlight();
  
  //lcd.createChar(0, bell);
  //lcd.createChar(1, emptyHeart);
  //lcd.createChar(2, cross);
  //lcd.createChar(3, flag);
  //lcd.createChar(4, clip);
  LCD.createChar(5, heart);
  LCD.createChar(6, crosshair);
  //lcd.createChar(7, bullet);
  LCD.createChar(8, shield);
  LCD.home();
}

void typeText(char *line1, char *line2)
{
  LCD.home();
  LCD.clear();
  LCD.blink();
  
  for(byte i=0; i<screenWidth; i++)
  {
    if(line1[i] != NULL) //if the character isnt null, print it on screen
    { 
      LCD.print(line1[i]);
      delay(250);
    }
  }
  
  LCD.setCursor(0, 1);
  
  delay(250);
  for(byte i=0; i<screenWidth; i++)
  {
    if(line2[i] != NULL) //if the character isnt null, print it on screen
    {
      LCD.print(line2[i]);
      delay(250);
    }
  }

  LCD.noBlink();
}

void updateKothLCD(){
  LCD.clear();
  LCD.home();

  //display game time remaining
  LCD.setCursor(5, 0);
  LCD.print("0:"); 
  if(KothGameMinutes < 10) {LCD.print("0"); }
  LCD.print(KothGameMinutes);
  LCD.print(":"); 
  if(KothGameSeconds < 10) {LCD.print("0"); }
  LCD.print(KothGameSeconds); 


  //2nd line
  LCD.setCursor(0, 1);

  //display green time
  LCD.print("0:"); 
  if(KothGreenMinutes < 10) {LCD.print("0"); }
  LCD.print(KothGreenMinutes);
  LCD.print(":"); 
  if(KothGreenSeconds < 10) {LCD.print("0"); }
  LCD.print(KothGreenSeconds);
  
  LCD.print("  ");

  //display red time
  LCD.print("0:"); 
  if(KothRedMinutes < 10) {LCD.print("0"); }
  LCD.print(KothRedMinutes);
  LCD.print(":"); 
  if(KothRedSeconds < 10) {LCD.print("0"); }
  LCD.print(KothRedSeconds);
}

void updateBombLCD(){
  LCD.clear();
  LCD.home();

  //display game time remaining
  LCD.setCursor(5, 0);
  LCD.print("0:"); 
  if(BombGameMinutes < 10) {LCD.print("0"); }
  LCD.print(BombGameMinutes);
  LCD.print(":"); 
  if(BombGameSeconds < 10) {LCD.print("0"); }
  LCD.print(BombGameSeconds); 


  //2nd line
  LCD.setCursor(0, 1);

  //display bomb info
  if(!BombPlanted)
  {
    LCD.print("  Press to Set");
  } else {
    LCD.print("Planted ");
    LCD.print("0:00:"); 
    if(BombTimerSeconds < 10) {LCD.print("0"); }
    LCD.print(BombTimerSeconds); 
  }
}

void updateBombLCDButtonPress(){
  LCD.clear();
  LCD.home();

  if(!BombPlanted)
  {
    //display game time remaining
    LCD.setCursor(3, 0);
    LCD.print("Arming Bomb"); 
    
    //2nd line
    LCD.setCursor(0, 1);
    for (int i = 0; i <= redButtonPressCounter; i++) {
      LCD.print("-----"); 
    }
  }
}


void updateMenuKothLCD(){
  LCD.clear();
  LCD.home();
  LCD.print("Duration 0:");
  if(duration == 1) {LCD.print("05");}
  else {LCD.print(duration*5);}
  LCD.print(":00");

  //2nd line
  LCD.setCursor(0, 1);
  //         1234567890123456
  LCD.print("<Change   Start>");
}

void updateMenuBombLCD(){
  LCD.clear();
  LCD.home();
  LCD.print("   Bomb Game");

  //2nd line
  LCD.setCursor(0, 1);
  //         1234567890123456
  LCD.print("<Back     Start>");
}

void updateGameSelectLCD(){
  LCD.clear();
  LCD.home();
  LCD.print("  Game Select");
  
  //2nd line
  LCD.setCursor(0, 1);
  //         1234567890123456
  LCD.print("< KOTH    BOMB >");
}

void updateKothEndLCD()
{
  LCD.clear();
  LCD.home();

  if(KothGreenMinutes < KothRedMinutes) //red wins
  {
    LCD.print(" Red Team Wins");
  }
  else if(KothRedMinutes < KothGreenMinutes) //green wins
  {
    LCD.print("Green Team Wins");
  }
  else //draw on minutes
  {
    if(KothGreenSeconds < KothRedSeconds) //red wins
    {
      LCD.print(" Red Team Wins");
    }
    else if(KothRedSeconds < KothGreenSeconds) //green wins
    {
      LCD.print("Green Team Wins");
    }
    else //draw on seconds
    {
      LCD.print("Draw: Shootout!");
    }
  }

  //2nd line
  LCD.setCursor(0, 1);

  //display green time
  LCD.print("0:"); 
  if(KothGreenMinutes < 10) {LCD.print("0"); }
  LCD.print(KothGreenMinutes);
  LCD.print(":"); 
  if(KothGreenSeconds < 10) {LCD.print("0"); }
  LCD.print(KothGreenSeconds);
  
  LCD.print("  ");

  //display red time
  LCD.print("0:"); 
  if(KothRedMinutes < 10) {LCD.print("0"); }
  LCD.print(KothRedMinutes);
  LCD.print(":"); 
  if(KothRedSeconds < 10) {LCD.print("0"); }
  LCD.print(KothRedSeconds);
}

void updateBombEndLCD()
{
  LCD.clear();
  LCD.home();
  
  if(BombTWin){
    LCD.print("Bomb Detonated");
    LCD.setCursor(0, 1);
    //display green time
    LCD.print("Bombers Win"); 
  } else {
    LCD.print("Bomb Neutralised");
    LCD.setCursor(0, 1);
    //display green time
    LCD.print("Anti Bombers Win");
  }
}
