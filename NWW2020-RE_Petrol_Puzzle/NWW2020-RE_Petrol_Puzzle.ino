#include <EasyButton.h>

//button Pins
#define B1Pin     23
#define B2Pin     25
#define B3Pin     27
#define B4Pin     29
#define B5Pin     31
#define B6Pin     33
#define B7Pin     35
#define B8Pin     37
#define B9Pin     39
#define B10Pin    41
#define B11Pin    43
#define B12Pin    45
#define B13Pin    47
#define B14Pin    49
#define B15Pin    51
#define B16Pin    53

//all the light Transistor Pins
#define L1Pin     22
#define L2Pin     24 
#define L3Pin     26 
#define L4Pin     28 
#define L5Pin     30 
#define L6Pin     32 
#define L7Pin     34 
#define L8Pin     36 
#define L9Pin     38 
#define L10Pin    40 
#define L11Pin    42 
#define L12Pin    44 
#define L13Pin    46 
#define L14Pin    48 
#define L15Pin    50 
#define L16Pin    52 

byte speakerPin    = 4;

//initiate buttons ... the name cant seem to end in numbers so 'a' *shrug*
EasyButton B1a(B1Pin); 
EasyButton B2a(B2Pin);
EasyButton B3a(B3Pin);
EasyButton B4a(B4Pin);
EasyButton B5a(B5Pin); 
EasyButton B6a(B6Pin);
EasyButton B7a(B7Pin);
EasyButton B8a(B8Pin);
EasyButton B9a(B9Pin); 
EasyButton B10a(B10Pin);
EasyButton B11a(B11Pin);
EasyButton B12a(B12Pin);
EasyButton B13a(B13Pin); 
EasyButton B14a(B14Pin);
EasyButton B15a(B15Pin);
EasyButton B16a(B16Pin);



bool light1 = false;
bool light2 = true;
bool light3 = true;
bool light4 = true;

byte stage = 1;

void onB1Pressed() { }
void onB2Pressed() { }
void onB3Pressed() { }
void onB4Pressed() { }
void onB5Pressed() { }
void onB6Pressed() { }
void onB7Pressed() { }
void onB8Pressed() { }
void onB9Pressed() { }
void onB10Pressed() { }
void onB11Pressed() { }
void onB12Pressed() { }
void onB13Pressed() { }
void onB14Pressed() { }
void onB15Pressed() { }
void onB16Pressed() { }


void setup() {
  //start serial connection
  Serial.begin(9600);

  //Initalise buttons
  B1a.begin();
  B2a.begin();
  B3a.begin();
  B4a.begin();
  B5a.begin();
  B6a.begin();
  B7a.begin();
  B8a.begin();
  B9a.begin();
  B10a.begin();
  B11a.begin();
  B12a.begin();
  B13a.begin();
  B14a.begin();
  B15a.begin();
  B16a.begin();
  
  B1a.onPressed (onB1Pressed);
  B2a.onPressed (onB2Pressed);
  B3a.onPressed (onB3Pressed);
  B4a.onPressed (onB4Pressed);
  B5a.onPressed (onB5Pressed);
  B6a.onPressed (onB6Pressed);
  B7a.onPressed (onB7Pressed);
  B8a.onPressed (onB8Pressed);
  B9a.onPressed (onB9Pressed);
  B10a.onPressed(onB10Pressed);
  B11a.onPressed(onB11Pressed);
  B12a.onPressed(onB12Pressed);
  B13a.onPressed(onB13Pressed);
  B14a.onPressed(onB14Pressed);
  B15a.onPressed(onB15Pressed);
  B16a.onPressed(onB16Pressed);

  pinMode(speakerPin, OUTPUT);

  //set light pinmodes
  pinMode(L1Pin, OUTPUT); 
  pinMode(L2Pin, OUTPUT); 
  pinMode(L3Pin, OUTPUT); 
  pinMode(L4Pin, OUTPUT); 
  pinMode(L5Pin, OUTPUT); 
  pinMode(L6Pin, OUTPUT); 
  pinMode(L7Pin, OUTPUT); 
  pinMode(L8Pin, OUTPUT); 
  pinMode(L9Pin, OUTPUT); 
  pinMode(L10Pin, OUTPUT); 
  pinMode(L11Pin, OUTPUT); 
  pinMode(L12Pin, OUTPUT); 
  pinMode(L13Pin, OUTPUT); 
  pinMode(L14Pin, OUTPUT); 
  pinMode(L15Pin, OUTPUT); 
  pinMode(L16Pin, OUTPUT); 

  //UpdateLights();
}

void loop() {
  
  
  // Continuously read the status of the buttons
  /*B1.read();
  B2.read();
  B3.read();
  B4.read();*/


  //Manual demo mode
  digitalWrite(L16Pin, LOW);
  digitalWrite(L1Pin, HIGH);
  delay(1000);

  digitalWrite(L1Pin, LOW);
  digitalWrite(L2Pin, HIGH);
  delay(1000);

  digitalWrite(L2Pin, LOW);
  digitalWrite(L3Pin, HIGH);
  delay(1000);

  digitalWrite(L3Pin, LOW);
  digitalWrite(L4Pin, HIGH);
  delay(1000);

  digitalWrite(L4Pin, LOW);
  digitalWrite(L5Pin, HIGH);
  delay(1000);

  digitalWrite(L5Pin, LOW);
  digitalWrite(L6Pin, HIGH);
  delay(1000);

  digitalWrite(L6Pin, LOW);
  digitalWrite(L7Pin, HIGH);
  delay(1000);

  digitalWrite(L7Pin, LOW);
  digitalWrite(L8Pin, HIGH);
  delay(1000);

  digitalWrite(L8Pin, LOW);
  digitalWrite(L9Pin, HIGH);
  delay(1000);

  digitalWrite(L9Pin, LOW);
  digitalWrite(L10Pin, HIGH);
  delay(1000);

  digitalWrite(L10Pin, LOW);
  digitalWrite(L11Pin, HIGH);
  delay(1000);

  digitalWrite(L11Pin, LOW);
  digitalWrite(L12Pin, HIGH);
  delay(1000);

  digitalWrite(L12Pin, LOW);
  digitalWrite(L13Pin, HIGH);
  delay(1000);

  digitalWrite(L13Pin, LOW);
  digitalWrite(L14Pin, HIGH);
  delay(1000);

  digitalWrite(L14Pin, LOW);
  digitalWrite(L15Pin, HIGH);
  delay(1000);

  digitalWrite(L15Pin, LOW);
  digitalWrite(L16Pin, HIGH);
  delay(1000);

  digitalWrite(L16Pin, LOW);

  delay(1000);
  digitalWrite(L1Pin, HIGH);
  delay(100);
  digitalWrite(L2Pin, HIGH);
  delay(100);
  digitalWrite(L3Pin, HIGH);
  delay(100);
  digitalWrite(L4Pin, HIGH);
  delay(100);
  digitalWrite(L5Pin, HIGH);
  delay(100);
  digitalWrite(L6Pin, HIGH);
  delay(100);
  digitalWrite(L7Pin, HIGH);
  delay(100);
  digitalWrite(L8Pin, HIGH);
  delay(100);
  digitalWrite(L9Pin, HIGH);
  delay(100);
  digitalWrite(L10Pin, HIGH);
  delay(100);
  digitalWrite(L11Pin, HIGH);
  delay(100);
  digitalWrite(L12Pin, HIGH);
  delay(100);
  digitalWrite(L13Pin, HIGH);
  delay(100);
  digitalWrite(L14Pin, HIGH);
  delay(100);
  digitalWrite(L15Pin, HIGH);
  delay(100);
  digitalWrite(L16Pin, HIGH);
  delay(100);

  delay(1000);
  digitalWrite(L1Pin, LOW);
  delay(100);
  digitalWrite(L2Pin, LOW);
  delay(100);
  digitalWrite(L3Pin, LOW);
  delay(100);
  digitalWrite(L4Pin, LOW);
  delay(100);
  digitalWrite(L5Pin, LOW);
  delay(100);
  digitalWrite(L6Pin, LOW);
  delay(100);
  digitalWrite(L7Pin, LOW);
  delay(100);
  digitalWrite(L8Pin, LOW);
  delay(100);
  digitalWrite(L9Pin, LOW);
  delay(100);
  digitalWrite(L10Pin, LOW);
  delay(100);
  digitalWrite(L11Pin, LOW);
  delay(100);
  digitalWrite(L12Pin, LOW);
  delay(100);
  digitalWrite(L13Pin, LOW);
  delay(100);
  digitalWrite(L14Pin, LOW);
  delay(100);
  digitalWrite(L15Pin, LOW);
  delay(100);
  digitalWrite(L16Pin, LOW);
  delay(100);

  delay(3000);
  
  
}

/*void UpdateLights()
{
  (light1) ? digitalWrite(light1Pin, HIGH) : digitalWrite(light1Pin, LOW);
  (light2) ? digitalWrite(light2Pin, HIGH) : digitalWrite(light2Pin, LOW);
  (light3) ? digitalWrite(light3Pin, HIGH) : digitalWrite(light3Pin, LOW);
  (light4) ? digitalWrite(light4Pin, HIGH) : digitalWrite(light4Pin, LOW);

  checkForWin();
}
void checkForWin()
{
  if(stage == 1) { //button4 on
    if(!light1 && !light2 && !light3 && light4)
    {
      success();
      light1 = false;
      light2 = true;
      light3 = false;
      light4 = true;
      stage = 2;
      UpdateLights();
    }
  } else if(stage == 2) {//button2 on
    if(!light1 && light2 && !light3 && !light4)
    {
      success();
      light1 = true;
      light2 = false;
      light3 = false;
      light4 = true;
      stage = 3;
      UpdateLights();
    }
  } else if(stage == 3) {//button1 on
    if(light1 && !light2 && !light3 && !light4)
    {
      success();

      digitalWrite(light1Pin, HIGH);
      digitalWrite(light2Pin, HIGH);
      digitalWrite(light3Pin, HIGH);
      digitalWrite(light4Pin, HIGH);
      playTone(500, 200);
      delay(200);
      playTone(500, 200);
      delay(5000);

      
      light1 = false;
      light2 = true;
      light3 = true;
      light4 = true;
      stage = 1;

      UpdateLights();
    }
  }
}

void success()
{
  playTone(1000, 200);
  delay(200);
  playTone(1000, 200);
  delay(200);
  playTone(1000, 200);
  delay(100);
  
  playTone(500, 200);
  delay(200);
  
}

//function for playing tones
void playTone(int tone, int duration) { // A sub routine for playing tones like the standard arduino melody example
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}*/
