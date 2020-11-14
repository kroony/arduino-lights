
//Includes for 16x2 LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ezButton.h>

const byte screenWidth = 16;
// Set the LCD address to 0x3f for a 16 chars and 2 line display
//make a screen for red and blue team
LiquidCrystal_I2C LCD(0x3E, 16, 2);

void displayDigit(int number);
void writeScores();

const byte totalScore = 13;

byte duration = 1;

int GameTickCounter = 0;
int ButtonPressCounter = 0;


bool gameActive = false;
byte gameMode = 0;
/*    Game Mode
 * 0 = Menu 
 * 1 = King of the hill
 * 2 = Bomb
 * 
 */
bool KothEndScreen = false;
bool BombEndScreen = false;

int KothGameMinutes = 0;
int KothGameSeconds = 0;

int KothGreenMinutes = 0;
int KothGreenSeconds = 0;
int KothRedMinutes = 0;
int KothRedSeconds = 0;
bool KothGreenActive = false;
bool KothRedActive = false;

int BombGameMinutes = 0;
int BombGameSeconds = 0;
int BombTimerSeconds = 0;
bool BombPlanted = false;
bool BombTWin = false;
bool BombDefused = false;

const char space = ' ';
const byte charDelay = 250;
const uint16_t screenDelay = 1000;
//byte greenButtonPressCounter = 0;
byte redButtonPressCounter = 0;

//Intro Strings
//       SCREEN LIMIT       = "1234567890123456"
const char intro1[17]       = "Domination Box";
const char intro2[17]       = "A Paintball Game";

const char intro3[17]       = "Made By";
const char intro4[17]       = "Bill Kroon";

const char intro11[17]      = "New Game In";
const char intro12[17]      = "3...2...1...";


//initiate buttons
ezButton buttonRed(5);
ezButton buttonGreen(4); 

class Light
{
  public:
  const byte pin;
  bool isOn;

  Light(byte pin_);
  bool getState();//returns if the light is on
  void turnOn();//turns on the light
  void turnOff();//turns off the light
  void switchLight();//switches the light on if off or off if on
};

//initiate Lights
Light lightGreen(8);
Light lightRed(7);


void setup()
{
  buttonRed.setDebounceTime(50); // set debounce time to 50 milliseconds
  buttonGreen.setDebounceTime(50); 
  
  randomSeed(analogRead(0));
  
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  delay(250);

  // initialize the LCD
  initaliseLCD();
  delay(1000);

  buttonRed.loop(); // MUST call the loop() function first
  buttonGreen.loop(); // MUST call the loop() function first
  
  typeText(intro1, intro2);
  lightGreen.turnOn();
  lightRed.turnOn();
  delay(1000);

  buttonRed.loop(); // MUST call the loop() function first
  buttonGreen.loop(); // MUST call the loop() function first

  typeText(intro3, intro4);
  lightGreen.turnOff();
  lightRed.turnOff();
  delay(1000);

  
  //updateMenuLCD();
  updateGameSelectLCD();
}

void loop()
{
  buttonRed.loop(); // MUST call the loop() function first
  buttonGreen.loop(); // MUST call the loop() function first

  /*int redBtnState = buttonRed.getState();
  Serial.print(" Red Button: "); Serial.println(redBtnState);

  int greenBtnState = buttonGreen.getState();
  Serial.print(" Green Button: "); Serial.println(greenBtnState);*/

  
  if(gameMode == 0)//Main Menu
  {
    buttonPressCheckGameSelect();
  }
  else if(gameMode == 1)//KOTH
  {
    if(gameActive)
    {
      //Button press detection
      KothButtonPressCheck();
      GameTickCounter++;
      Serial.println(GameTickCounter);
      if(GameTickCounter > 215)
      {
        GameTickCounter = 0;
        gameTickKoth(); 
        updateKothLCD();
      }
      
      if(KothGameMinutes == -1)
      {
        endGame();
      }
    }
    else if (KothEndScreen) //koth end screen
    {
      buttonPressEndScreen();
    }
    else //koth menu
    {
      //check for button press to start new game perhaps coin mech?
      buttonPressCheckMenuKoth();
    }
  }
  else if(gameMode = 2)//BOMB
  {
    if(gameActive)
    {
      //Button press detection
      BombButtonPressCheck();
      GameTickCounter++;
      Serial.println(GameTickCounter);
      if(GameTickCounter > 215)
      {
        GameTickCounter = 0;
        gameTickBomb(); 
        if(redButtonPressCounter > 0) // pressing the red button display the button press screen
        {
          updateBombLCDButtonPress();  
        } else {
          updateBombLCD();  
        }
      }
      
      if(BombGameMinutes == -1 || BombDefused) //CT Win
      {
        BombTWin = false;
        endBombGame();
      }

      if(BombTimerSeconds == -1) //T win
      {
        BombTWin = true;
        endBombGame();
      }
    }
    else if (BombEndScreen) //bomb end screen
    {
      buttonPressEndScreen();
    }
    else //Bomb menu
    {
      //check for button press to start new game perhaps coin mech?
      buttonPressCheckMenuBomb();
    }
  }  
  
  //delay ms
  //delay(15);*/
}

void startNewGameKoth()
{
  typeText(intro1, intro2);
  delay(10000);
  typeText(intro11, intro12);
  
  KothGreenMinutes = 0;
  KothGreenSeconds = 0;
  KothRedMinutes = 0;
  KothRedSeconds = 0;
  KothGreenActive = false;
  KothRedActive = false; 
  
  KothGameMinutes = duration*5;
  //gameMinutes = duration;
  KothGameSeconds = 0;
  gameActive = true;
}

void startNewGameBomb()
{
  typeText(intro1, intro2);
  delay(1000);
  typeText(intro11, intro12);

  BombGameMinutes = 6;
  BombGameSeconds = 0;
  BombTimerSeconds = 30;
  BombPlanted = false;
  GameTickCounter = 0;
  
  gameActive = true;
  BombEndScreen = false;
  BombDefused = false;
}

void endGame()
{ 
  gameActive = false;
  
  updateKothEndLCD();
  KothEndScreen = true;
}

void endBombGame()
{ 
  gameActive = false;
  
  updateBombEndLCD();
  BombEndScreen = true;
  lightRed.turnOff();
  lightGreen.turnOff();
}

void gameTickKoth()
{
  KothGameSeconds--;
  if(KothGameSeconds == -1) {
    KothGameSeconds = 59;
    KothGameMinutes--;
  }

  if(KothGreenActive)
  {
    KothGreenSeconds++;
    if(KothGreenSeconds == 60) 
    {
      KothGreenSeconds = 0;
      KothGreenMinutes++;
    }
  } else if(KothRedActive)
  {
    KothRedSeconds++;
    if(KothRedSeconds == 60) 
    {
      KothRedSeconds = 0;
      KothRedMinutes++;
    }
  }
}

void gameTickBomb()
{
  BombGameSeconds--;
  if(BombGameSeconds == -1) {
    BombGameSeconds = 59;
    BombGameMinutes--;
  }

  if(BombPlanted) {
    BombTimerSeconds--;
    if ( (BombGameSeconds & 0x01) == 0) { 
      lightRed.turnOn();
    } else {
      lightRed.turnOff();
    }
  } else {
    lightGreen.turnOn();
  }
}
