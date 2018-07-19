#include 
 
#define LED_PIN     5
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    60
 
#define BRIGHTNESS  200
 
bool gReverseDirection = false;
 
CRGB leds[NUM_LEDS];
 
int RFIDResetPin = 13;
 
bool lightOn = false;
 
//Register your RFID tag here
char tag1[13] = "7F001B29A9E4";
 
void setup(){
  delay(30); // sanity delay
  FastLED.addLeds(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  offLED();
  Serial.begin(9600);
 
  pinMode(RFIDResetPin, OUTPUT);
  digitalWrite(RFIDResetPin, HIGH);
 
  //ONLY NEEDED IF CONTROLING THESE PINS - EG. LEDs
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}
 
void loop(){
 
  char tagString[13];
  int index = 0;
  boolean reading = false;
 
  while(Serial.available()){
 
    int readByte = Serial.read(); //read next available byte
 
    if(readByte == 2) reading = true; //begining of tag
    if(readByte == 3) reading = false; //end of tag
 
    if(reading && readByte != 2 && readByte != 10 && readByte != 13){
      //store the tag
      tagString[index] = readByte;
      index ++;
    }
  }
 
  checkTag(tagString); //Check if it is a match
  clearTag(tagString); //Clear the char of all value
  resetReader(); // reset the RFID reader
  delay(800);
}
 
void checkTag(char tag[]){
///////////////////////////////////
//Check the read tag against known tags
///////////////////////////////////
  if(strlen(tag) == 0) return; //empty, no need to contunue
  Serial.println(tag); //read out any tag
  if(compareTag(tag, tag1)){
    toggleLED();
  }
}
 
void toggleLED(){
  if (lightOn) {
    offLED();
  } else {
    onLED();
  }
  delay(100);
  FastLED.show();
  lightOn = !lightOn;
}
 
void onLED() {
  for ( int j = 0; j < NUM_LEDS; j++) {
    leds[j] = CRGB::Red;
  }
}
 
void offLED() {
  for ( int j = 0; j < NUM_LEDS; j++) {
    leds[j] = CRGB::Black;
  }
}
 
void resetReader(){
///////////////////////////////////
//Reset the RFID reader to read again.
///////////////////////////////////
  digitalWrite(RFIDResetPin, LOW);
  digitalWrite(RFIDResetPin, HIGH);
  delay(150);
}
 
void clearTag(char one[]){
///////////////////////////////////
//clear the char array by filling with null - ASCII 0
//Will think same tag has been read otherwise
///////////////////////////////////
  for(int i = 0; i < strlen(one); i++){
    one[i] = 0;
  }
}
 
boolean compareTag(char one[], char two[]){
///////////////////////////////////
//compare two value to see if same,
//strcmp not working 100% so we do this
///////////////////////////////////
 
  if(strlen(one) == 0) return false; //empty
 
  for(int i = 0; i < 12; i++){
    if(one[i] != two[i]) return false;
  }
 
  return true; //no mismatches
}
