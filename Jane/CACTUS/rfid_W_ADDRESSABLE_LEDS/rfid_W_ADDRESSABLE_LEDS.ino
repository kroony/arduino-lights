#include "FastLED.h"
#include <Adafruit_NeoPixel.h>
#include "SPI.h"
#include "MFRC522.h"
#define DATA_PIN 6

#define SS_PIN 53
#define RST_PIN 5
#define PIN 6
#define NUM_LEDS 50
#define BRIGHTNESS 255

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);
byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // put your setup code here, to run once:
  //strip.setBrightness(BRIGHTNESS);
  strip.begin();
  setColor(0,0,0);
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("I am waiting for card...");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));
  Serial.println(piccType);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_UL)
      {
       
    
    delay(5000);
  }
  else{
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }

  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
  delay(1000);

  if (strID.indexOf("5B:52:4C:79") >= 0) {  //put your own tap card key;
    Serial.println("********************");
    Serial.println("**Authorised access**");
    Serial.println("********************");
    //digitalWrite(6, HIGH);
    setColor(0, 255,0);
    delay (5000);
    //digitalWrite(6, LOW);
  
    return;
  }
  else {
    Serial.println("****************");
    Serial.println("**Access denied**");
    Serial.println("****************");
    setColor(255, 0, 0);
    //delay (5000);
    //digitalWrite(6, LOW);
  }
  
if
(strID.indexOf("EA:D9:A9:2E") >= 0) {  //put your own tap card key;
    Serial.println("********************");
    Serial.println("**Authorised access**");
    Serial.println("********************");
    //digitalWrite(6, HIGH);
    setColor(11, 22,120);
    delay (100);
    setColor(26, 55, 200);
    delay (100);
    setColor(45, 120, 100);
    delay (100);
     }
  else {
    Serial.println("****************");
    Serial.println("**Access denied**");
    Serial.println("****************");
    setColor(255, 0, 0);
    //delay (5000);
    //digitalWrite(6, LOW);
  
 }
 if
(strID.indexOf("04:7E:25:FA") >= 0) {  //put your own tap card key;
    Serial.println("********************");
    Serial.println("**Authorised access**");
    Serial.println("********************");
}
    {
   // Move a single white led 
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::White;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
     delay(5);
     
   
    for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::White;


      // Turn our current led back to black for the next loop around
     
   }
}
  }
  } 
}
// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}


void setColor(byte red, byte green, byte blue)
{
  for(int i=0; i<NUM_LEDS; i++)
  {
    strip.setPixelColor(i, Color(red, green, blue));
  }
  strip.show();
}




