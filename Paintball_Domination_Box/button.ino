void KothButtonPressCheck()
{
  if(!KothGreenActive)
  {
    if (buttonGreen.isReleased()) // green takes control
    {
      KothGreenActive = true;
      lightGreen.turnOn();
      lightRed.turnOff();
      KothRedActive = false;
    }
  }

  if(!KothRedActive)
  {
    if (buttonRed.isReleased()) // red takes control
    {
      KothRedActive = true;
      lightRed.turnOn();
      lightGreen.turnOff();
      KothGreenActive = false;
    }
  }
}

void BombButtonPressCheck()
{
  if(!BombPlanted)
  {
    //greenButtonPressCounter = 0;
    if(buttonRed.getState() == 0) //red button is being held down
    {
      if( GameTickCounter == 0) {
        redButtonPressCounter++;
        if(redButtonPressCounter == 3) {
          BombPlanted = true;
          lightGreen.turnOff();
        }
        Serial.print(" redButtonPressCounter: "); Serial.println(redButtonPressCounter);
      }
    } else {
      redButtonPressCounter = 0;
    }
  } else {
    redButtonPressCounter = 0;
    if (buttonGreen.isReleased()) //Green button is pressed to disarm the bomb
    {
      BombDefused = true;
    }
  }
}

void buttonPressCheckMenuKoth() //button press checking for the menu
{
  if (buttonGreen.isReleased()) {
    //change game time
    duration++;
    if(duration == 7) {duration = 1;}
    Serial.print("duration: ");
    Serial.println(duration);
    updateMenuKothLCD();
  }

  if (buttonRed.isReleased()) {
    //start game
    startNewGameKoth();
  }
}

void buttonPressCheckMenuBomb() //button press checking for the bomb game menu
{
  if (buttonGreen.isReleased()) { //back to main menu
    
    gameMode = 0;
    updateGameSelectLCD();
  }

  if (buttonRed.isReleased()) {
    //start game bomb
    startNewGameBomb();
  }
}

void buttonPressCheckGameSelect() //button press checking for the game select menu
{
  if (buttonGreen.isReleased()) {//selected koth
    gameMode = 1;
    updateMenuKothLCD();
  }

  if (buttonRed.isReleased()) {
    gameMode = 2;
    updateMenuBombLCD();
  }
}

void buttonPressEndScreen()
{
  if (buttonRed.isReleased() || buttonGreen.isReleased())
  {
    KothEndScreen = false;
    BombEndScreen = false;
    lightGreen.turnOff();
    lightRed.turnOff();
    duration = 1;
    gameMode = 0;
    delay(1000);
    updateGameSelectLCD();
  }
}
