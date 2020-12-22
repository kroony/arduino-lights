//all the lights will defaultly be on, doing a rainbow fade, all number definitions are for the off state. 

/*
 * 7 segment display values for 0 - 9
 * +-A-+
 * F   B
 * +-G-+
 * E   C
 * +-D-+
 */
//                    {A, B, C, D, E, F, G}
const bool zero[7] =  {1, 1, 1, 1, 1, 1, 0};
const bool one[7] =   {0, 1, 1, 0, 0, 0, 0};
const bool two[7] =   {1, 1, 0, 1, 1, 0, 1};
const bool three[7] = {1, 1, 1, 1, 0, 0, 1};
const bool four[7] =  {0, 1, 1, 0, 0, 1, 1};
const bool five[7] =  {1, 0, 1, 1, 0, 1, 1};
const bool six[7] =   {1, 0, 1, 1, 1, 1, 1};
const bool seven[7] = {1, 1, 1, 0, 0, 0, 0};
const bool eight[7] = {1, 1, 1, 1, 1, 1, 1};
const bool nine[7] =  {1, 1, 1, 1, 0, 1, 1};

//address to turn off to show the digit
const byte SecondOnes0[3]  = { 0,  1,  2};
const byte SecondOnes1[15] = { 6,  7,  8, 16, 17, 18, 19, 20, 21, 3,  4,  5,  0,  1,  2};
const byte SecondOnes2[6]  = {13, 14, 15, 3,  4,  5};
const byte SecondOnes3[6]  = {19, 20, 21, 3,  4,  5};
const byte SecondOnes4[9]  = { 6,  7,  8, 16, 17, 18, 19, 20, 21};
const byte SecondOnes5[6]  = { 9, 10, 11, 19, 20, 21};
const byte SecondOnes6[3]  = { 9, 10, 11};
const byte SecondOnes7[12] = {16, 17, 18, 19, 20, 21,  3,  4,  5,  0,  1,  2};
const byte SecondOnes8[0]  = {};
const byte SecondOnes9[3]  = {19, 20, 21};

const byte SecondTens0[3]  = {44, 45, 46};
const byte SecondTens1[15] = {44, 45, 46, 35, 36, 37, 38, 39, 40, 28, 29, 30, 31, 32, 33};
const byte SecondTens2[6]  = {25, 26, 27, 35, 36, 37};
const byte SecondTens3[6]  = {31, 32, 33, 35, 36, 37};
const byte SecondTens4[9]  = {38, 39, 40, 28, 29, 30, 31, 32, 33};
const byte SecondTens5[6]  = {41, 42, 43, 31, 32, 33};
const byte SecondTens6[3]  = {41, 42, 43};
const byte SecondTens7[12] = {44, 45, 46, 35, 36, 37, 28, 29, 30, 31, 32, 33};
const byte SecondTens8[0]  = {};
const byte SecondTens9[3]  = {31, 32, 33};

const byte MinuteOnes0[3]  = {52, 53, 54};
const byte MinuteOnes1[15] = {58, 59, 60, 68, 69, 70, 71, 72, 73, 55, 56, 57, 52, 53, 54};
const byte MinuteOnes2[6]  = {65, 66, 67, 55, 56, 57};
const byte MinuteOnes3[6]  = {71, 72, 73, 55, 56, 57};
const byte MinuteOnes4[9]  = {58, 59, 60, 68, 69, 70, 71, 72, 73};
const byte MinuteOnes5[6]  = {61, 62, 63, 71, 72, 73};
const byte MinuteOnes6[3]  = {61, 62, 63};
const byte MinuteOnes7[12] = {68, 69, 70, 71, 72, 73, 55, 56, 57, 52, 53, 54};
const byte MinuteOnes8[0]  = {};
const byte MinuteOnes9[3]  = {71, 72, 73};

const byte MinuteTens0[3]  = {96, 97, 98};
const byte MinuteTens1[15] = {96, 97, 98, 87, 88, 89, 90, 91, 92, 80, 81, 82, 83, 84, 85};
const byte MinuteTens2[6]  = {77, 78, 79, 87, 88, 89};
const byte MinuteTens3[6]  = {83, 84, 85, 87, 88, 89};
const byte MinuteTens4[9]  = {90, 91, 92, 80, 81, 82, 83, 84, 85};
const byte MinuteTens5[6]  = {93, 94, 95, 83, 84, 85};
const byte MinuteTens6[3]  = {93, 94, 95};
const byte MinuteTens7[12] = {96, 97, 98, 87, 88, 89, 80, 81, 82, 83, 84, 85};
const byte MinuteTens8[0]  = {};
const byte MinuteTens9[3]  = {83, 84, 85};

int timerCount = 0;

int digitDisplay = 10000;


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 99

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  rainbow(10);             // Flowing rainbow cycle along the whole strip
}

// Some functions of our own for creating animated effects -----------------


// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));

      //dead pixels on seconds digit
      strip.setPixelColor(12, strip.Color(0,0,0));
      strip.setPixelColor(22, strip.Color(0,0,0));
      strip.setPixelColor(23, strip.Color(0,0,0));
      strip.setPixelColor(24, strip.Color(0,0,0));
      strip.setPixelColor(34, strip.Color(0,0,0));

      //dead pixels on 1st dots
      strip.setPixelColor(49, strip.Color(0,0,0));

      //dead pixels on minutes digit
      strip.setPixelColor(64, strip.Color(0,0,0));
      strip.setPixelColor(74, strip.Color(0,0,0));
      strip.setPixelColor(75, strip.Color(0,0,0));
      strip.setPixelColor(76, strip.Color(0,0,0));
      strip.setPixelColor(86, strip.Color(0,0,0));

      

      
      timerCount++;
      if(timerCount > 20) {
        timerCount = 0;
        digitDisplay--;
        if(digitDisplay == 0) {
          digitDisplay = 10000;
        }
      }
      //Serial.println(digitDisplay);
      byte minuteTens = floor(digitDisplay/1000);
      //Serial.print(minuteTens);
      //Serial.print(" ");
      byte minuteOnes = floor((digitDisplay - (minuteTens * 1000)) /100);
      //Serial.print(minuteOnes);
      //Serial.print(" ");
      byte secondTens = floor((digitDisplay - (minuteTens * 1000) - (minuteOnes * 100)) / 10);
      //Serial.print(secondTens);
      //Serial.print(" ");
      byte secondOnes = digitDisplay - (minuteTens * 1000) - (minuteOnes * 100) - (secondTens * 10);
      //Serial.print(secondOnes);
      //Serial.println(" ");

      
      
      switch (minuteTens) {
        case 0:
          DisplayDigit(MinuteTens0, sizeof(MinuteTens0));
          break;
        case 1:
          DisplayDigit(MinuteTens1, sizeof(MinuteTens1));
          break;
        case 2:
          DisplayDigit(MinuteTens2, sizeof(MinuteTens2));
          break;
        case 3:
          DisplayDigit(MinuteTens3, sizeof(MinuteTens3));
          break;
        case 4:
          DisplayDigit(MinuteTens4, sizeof(MinuteTens4));
          break;
        case 5:
          DisplayDigit(MinuteTens5, sizeof(MinuteTens5));
          break;
        case 6:
          DisplayDigit(MinuteTens6, sizeof(MinuteTens6));
          break;
        case 7:
          DisplayDigit(MinuteTens7, sizeof(MinuteTens7));
          break;
        case 8:
          DisplayDigit(MinuteTens8, sizeof(MinuteTens8));
          break;
        case 9:
          DisplayDigit(MinuteTens9, sizeof(MinuteTens9));
          break;
        default:
          DisplayDigit(MinuteTens0, sizeof(MinuteTens0));
          break;
      }
      
      switch (minuteOnes) {
        case 0:
          DisplayDigit(MinuteOnes0, sizeof(MinuteOnes0));
          break;
        case 1:
          DisplayDigit(MinuteOnes1, sizeof(MinuteOnes1));
          break;
        case 2:
          DisplayDigit(MinuteOnes2, sizeof(MinuteOnes2));
          break;
        case 3:
          DisplayDigit(MinuteOnes3, sizeof(MinuteOnes3));
          break;
        case 4:
          DisplayDigit(MinuteOnes4, sizeof(MinuteOnes4));
          break;
        case 5:
          DisplayDigit(MinuteOnes5, sizeof(MinuteOnes5));
          break;
        case 6:
          DisplayDigit(MinuteOnes6, sizeof(MinuteOnes6));
          break;
        case 7:
          DisplayDigit(MinuteOnes7, sizeof(MinuteOnes7));
          break;
        case 8:
          DisplayDigit(MinuteOnes8, sizeof(MinuteOnes8));
          break;
        case 9:
          DisplayDigit(MinuteOnes9, sizeof(MinuteOnes9));
          break;
        default:
          DisplayDigit(MinuteOnes0, sizeof(MinuteOnes0));
          break;
      }
      
      switch (secondTens) {
        case 0:
          DisplayDigit(SecondTens0, sizeof(SecondTens0));
          break;
        case 1:
          DisplayDigit(SecondTens1, sizeof(SecondTens1));
          break;
        case 2:
          DisplayDigit(SecondTens2, sizeof(SecondTens2));
          break;
        case 3:
          DisplayDigit(SecondTens3, sizeof(SecondTens3));
          break;
        case 4:
          DisplayDigit(SecondTens4, sizeof(SecondTens4));
          break;
        case 5:
          DisplayDigit(SecondTens5, sizeof(SecondTens5));
          break;
        case 6:
          DisplayDigit(SecondTens6, sizeof(SecondTens6));
          break;
        case 7:
          DisplayDigit(SecondTens7, sizeof(SecondTens7));
          break;
        case 8:
          DisplayDigit(SecondTens8, sizeof(SecondTens8));
          break;
        case 9:
          DisplayDigit(SecondTens9, sizeof(SecondTens9));
          break;
        default:
          DisplayDigit(SecondTens0, sizeof(SecondTens0));
          break;
      }
      
      switch (secondOnes) {
        case 0:
          DisplayDigit(SecondOnes0, sizeof(SecondOnes0));
          break;
        case 1:
          DisplayDigit(SecondOnes1, sizeof(SecondOnes1));
          break;
        case 2:
          DisplayDigit(SecondOnes2, sizeof(SecondOnes2));
          break;
        case 3:
          DisplayDigit(SecondOnes3, sizeof(SecondOnes3));
          break;
        case 4:
          DisplayDigit(SecondOnes4, sizeof(SecondOnes4));
          break;
        case 5:
          DisplayDigit(SecondOnes5, sizeof(SecondOnes5));
          break;
        case 6:
          DisplayDigit(SecondOnes6, sizeof(SecondOnes6));
          break;
        case 7:
          DisplayDigit(SecondOnes7, sizeof(SecondOnes7));
          break;
        case 8:
          DisplayDigit(SecondOnes8, sizeof(SecondOnes8));
          break;
        case 9:
          DisplayDigit(SecondOnes9, sizeof(SecondOnes9));
          break;
        default:
          // there was some sort of problem
          break;
      }
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void DisplayDigit(byte LedsToTurnOff[], byte arraySize)
{
  for(byte i=0; i < arraySize; i++)
  {
    strip.setPixelColor(LedsToTurnOff[i], strip.Color(0,0,0));
  }
}
