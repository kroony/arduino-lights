// Basic 4 Realy board connection
// Each relay is turned on for 2 seconds and then off.
// You can here them click as there state changes from off to on and on to
// off.
// You will also see the corresponding Red LED on the 4 Relay board
// light up when the relay is on.
 
//  define names for the 4 Digital pins On the Arduino   
// These data pins link to 4 Relay board pins IN1, IN2, IN3,IN4

byte RED = 4;                        
byte YELLOW = 3;
byte GREEN = 2;


byte opt1pin = 7;
byte opt2pin = 8;
byte opt3pin = 9;
byte opt4pin = 10;
byte opt5pin = 11;
byte opt6pin = 12;

byte opt = 0;

uint32_t patternCounter = 0;

byte RandomSelector = 1;

void setup()
{
  Serial.begin(9600);
  // Initialise the Arduino data pins for OUTPUT
  pinMode(RED, OUTPUT);       
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  Serial.println("pinmode"); 

  digitalWrite(RED,LOW);
  digitalWrite(YELLOW,LOW);
  digitalWrite(GREEN,LOW);

  pinMode(opt1pin, INPUT_PULLUP);
  pinMode(opt2pin, INPUT_PULLUP);
  pinMode(opt3pin, INPUT_PULLUP);
  pinMode(opt4pin, INPUT_PULLUP);
  pinMode(opt5pin, INPUT_PULLUP);
  pinMode(opt6pin, INPUT_PULLUP);
}

void loop()
{
  getOpt();
  //Serial.println(opt);



  switch(opt) {
    case 1: //turn everything off, standby mode
      digitalWrite(RED,LOW);
      digitalWrite(YELLOW,LOW);
      digitalWrite(GREEN,LOW);
      patternCounter = 0;
      break;
      
    case 2: //Normal Light sequence, slow
      if(patternCounter > 2200) { patternCounter = 0; }
      if(patternCounter <= 1000) {
        digitalWrite(RED,HIGH);
        digitalWrite(YELLOW,LOW);
        digitalWrite(GREEN,LOW);
      } else if(patternCounter > 1000 && patternCounter <= 2000) {
        digitalWrite(RED,LOW);
        digitalWrite(YELLOW,LOW);
        digitalWrite(GREEN,HIGH);
      } else if(patternCounter > 2000 && patternCounter <= 2200) {
        digitalWrite(RED,LOW);
        digitalWrite(YELLOW,HIGH);
        digitalWrite(GREEN,LOW);
      }
      patternCounter++;
      delay(15); 
      break;
      
    case 3: //Normal Light Sequence, Fast
      if(patternCounter > 230) { patternCounter = 0; }
      if(patternCounter <= 100) {
        digitalWrite(RED,HIGH);
        digitalWrite(YELLOW,LOW);
        digitalWrite(GREEN,LOW);
      } else if(patternCounter > 100 && patternCounter <= 200) {
        digitalWrite(RED,LOW);
        digitalWrite(YELLOW,LOW);
        digitalWrite(GREEN,HIGH);
      } else if(patternCounter > 200 && patternCounter <= 230) {
        digitalWrite(RED,LOW);
        digitalWrite(YELLOW,HIGH);
        digitalWrite(GREEN,LOW);
      }
      patternCounter++;
      delay(15); 
      break;
      
    case 4: //random, one at a time
      if(patternCounter < 2000)
      { 
        if      (RandomSelector == 1) { isolateRed(); }
        else if (RandomSelector == 2) { isolateYellow(); }
        else if (RandomSelector == 3) { isolateGreen(); }
      } else {
        patternCounter = 0;
        RandomSelector = random(1, 4);
      }
      
      patternCounter++;
      break;

    case 5: //random, any at a time
      if(patternCounter < 2000)
      { 
        if      (RandomSelector == 1) { digitalWrite(RED,HIGH); }
        else if (RandomSelector == 2) { digitalWrite(YELLOW,HIGH); }
        else if (RandomSelector == 3) { digitalWrite(GREEN,HIGH); }
        else if (RandomSelector == 4) { digitalWrite(RED,LOW); }
        else if (RandomSelector == 5) { digitalWrite(YELLOW,LOW); }
        else if (RandomSelector == 6) { digitalWrite(GREEN,LOW); }
      } else {
        patternCounter = 0;
        RandomSelector = random(1, 7);
      }
      
      patternCounter++;
      break;

    case 6: //random, any at a time
      if(patternCounter < 2000)
      { 
        if      (RandomSelector == 1) { isolateRed(); }
        else if (RandomSelector == 2) { isolateYellow(); }
        else if (RandomSelector == 3) { isolateGreen(); }
      } else {
        patternCounter = 0;
        RandomSelector++;
        if(RandomSelector > 3) { RandomSelector = 1; }
      }
      
      patternCounter++;
      break;
  }





  /* 
  //Serial.println("red"); 
  digitalWrite(RED,HIGH);           // Turns ON Relays 1  
  delay(2000);                                      // Wait 2 seconds
  digitalWrite(RED,LOW);          // Turns Relay Off 
  
  //Serial.println("yellow"); 
  digitalWrite(YELLOW,HIGH);           // Turns ON Relays 2
  delay(2000);                                      // Wait 2 seconds
  digitalWrite(YELLOW,LOW);          // Turns Relay Off

  //Serial.println("green"); 
  digitalWrite(GREEN,HIGH);           // Turns ON Relays 3
  delay(2000);                                      // Wait 2 seconds
  digitalWrite(GREEN,LOW);          // Turns Relay Off   
  */     
}

void getOpt()
{
    if(digitalRead(opt1pin) == 0) {opt = 1; return;}
    if(digitalRead(opt2pin) == 0) {opt = 2; return;}
    if(digitalRead(opt3pin) == 0) {opt = 3; return;}
    if(digitalRead(opt4pin) == 0) {opt = 4; return;}
    if(digitalRead(opt5pin) == 0) {opt = 5; return;}
    if(digitalRead(opt6pin) == 0) {opt = 6; return;}

    //failure!
    opt = 1; return;
}

void isolateRed() //turn on red and turn off everything else
{
  digitalWrite(RED,HIGH);
  digitalWrite(YELLOW,LOW);
  digitalWrite(GREEN,LOW);
}

void isolateYellow() //turn on red and turn off everything else
{
  digitalWrite(RED,LOW);
  digitalWrite(YELLOW,HIGH);
  digitalWrite(GREEN,LOW);
}

void isolateGreen() //turn on red and turn off everything else
{
  digitalWrite(RED,LOW);
  digitalWrite(YELLOW,LOW);
  digitalWrite(GREEN,HIGH);
}
