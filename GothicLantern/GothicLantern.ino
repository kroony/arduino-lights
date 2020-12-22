/*
  FastLED Fire 2018 by Stefan Petrick
  The visual effect highly depends on the framerate.
  In the Youtube video it runs at arround 70 fps.
  https://www.youtube.com/watch?v=SWMu-a9pbyk
  The heatmap movement is independend from the framerate.
  The actual scaling operation is not.
  Check out the code for further comments about the interesting parts
*/

#include "FastLED.h"

uint8_t Width  = 10;
uint8_t Height = 14;
uint8_t CentreX =  (Width / 2) - 1;
uint8_t CentreY = (Height / 2) - 1;

#define NUM_LEDS      140
#define BRIGHTNESS    255
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
CRGB leds[NUM_LEDS];

const byte BrightnessPotPin = 2;
const byte SpeedPotPin = 1;
const byte HuePotPin = 0;
uint8_t hue = 0;


// control parameters for the noise array

uint32_t x;
uint32_t y;
uint32_t z;
uint32_t scale_x;
uint32_t scale_y;

// storage for the noise data
// adjust the size to suit your setup
uint8_t noise[10][14];

// heatmap data with the size matrix width * height
uint8_t heat[140];

// the color palette
CRGBPalette16 Pal;

void setup() {

  Serial.begin(115200);
  // Adjust this for you own setup. Use the hardware SPI pins if possible.
  // On Teensy 3.1/3.2 the pins are 11 & 13
  // Details here: https://github.com/FastLED/FastLED/wiki/SPI-Hardware-or-Bit-banging
  // In case you see flickering / glitching leds, reduce the data rate to 12 MHZ or less
  FastLED.addLeds<CHIPSET, 6, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setDither(DISABLE_DITHER);
  //Pal = HeatColors_p;


  
  for (uint8_t i = 0; i < Width; i++) {
    for (uint8_t j = 0; j < Height; j++) {
      Serial.print("x: ");
      Serial.print(i);
      Serial.print("  -  y: ");
      Serial.print(j);
      Serial.print("  -  add: ");
      Serial.println(XY(i,j));
      
    }
  }
}

void loop() {
  Fire2018();
  //show_fps();
}

// check the Serial Monitor for fps rate
void show_fps() {
  EVERY_N_MILLIS(100) {
    Serial.println(LEDS.getFPS());
  }
}

// this finds the right index within a serpentine matrix
uint8_t XY( uint8_t x, uint8_t y) {  //uint8_t instead of 16 as all your values <256
  uint8_t i;
  /*if ( !(x & 0x01)) {  //column is odd (the OxO1 bit). Index increments as height diminishes (going down the serpentine)
    uint8_t reverseY = (Height - 1) - y;
    i = (x * Height) + reverseY;
  } else {  //column is even. Index increments as height increases. 
    i = (x * Height) + y;
  }*/
  i = (y * 10) + x;
  i = 140 - i;
  
  return i;
/*
  x 0 y 0 
  x 1 y 0 
*/
  
}


// here we go
void Fire2018() {

  // get one noise value out of a moving noise space
  uint16_t ctrl1 = inoise16(11 * millis(), 0, 0);
  // get another one
  uint16_t ctrl2 = inoise16(13 * millis(), 100000, 100000);
  // average of both to get a more unpredictable curve
  uint16_t  ctrl = ((ctrl1 + ctrl2) / 2);

  // this factor defines the general speed of the heatmap movement
  // high value = high speed
  uint8_t speed = map(analogRead(SpeedPotPin), 0, 1023, 1, 50);
  //Serial.println(speed);
  //uint8_t speed = 27; //original 27

  // here we define the impact of the wind
  // high factor = a lot of movement to the sides
  x = 3 * ctrl * speed; //initial 3

  // this is the speed of the upstream itself
  // high factor = fast movement
  y =2 * millis() * speed; //initial 2

  // just for ever changing patterns we move through z as well
  z = 3 * millis() * speed ; //initial 3

  // ...and dynamically scale the complete heatmap for some changes in the
  // size of the heatspots.
  // The speed of change is influenced by the factors in the calculation of ctrl1 & 2 above.
  // The divisor sets the impact of the size-scaling.
  scale_x = ctrl1 / 10;
  scale_y = ctrl2 / 2;

  // Calculate the noise array based on the control parameters.
  uint8_t layer = 0;
  for (uint8_t i = 0; i < Width; i++) {
    uint32_t ioffset = scale_x * (i - CentreX);
    for (uint8_t j = 0; j < Height; j++) {
      uint32_t joffset = scale_y * (j - CentreY);
      uint16_t data = ((inoise16(x + ioffset, y + joffset, z)) + 1);
      noise[i][j] = data >> 8;
    }
  }

  //change the pallet
  hue = map(analogRead(HuePotPin), 0, 1023, 1, 255);
  if(hue <= 5) { Pal = HeatColors_p; } else {
    CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
    CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
    CRGB whiter = CHSV(hue,128,128);
    //gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);
    Pal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);
    //Serial.println(hue);
  }
  


  // Draw the first (lowest) line - seed the fire.
  // It could be random pixels or anything else as well.
  for (uint8_t x = 0; x < Width; x++) {
    // draw
    leds[XY(x, Height-1)] = ColorFromPalette( Pal, noise[x][0]);
    // and fill the lowest line of the heatmap, too
    heat[XY(x, Height-1)] = noise[x][0];
  }

  // Copy the heatmap one line up for the scrolling.
  for (uint8_t y = 0; y < Height - 1; y++) {
    for (uint8_t x = 0; x < Width; x++) {
      heat[XY(x, y)] = heat[XY(x, y + 1)];
    }
  }

  // Scale the heatmap values down based on the independent scrolling noise array.
  for (uint8_t y = 0; y < Height - 1; y++) {
    for (uint8_t x = 0; x < Width; x++) {

      // get data from the calculated noise field
      uint8_t dim = noise[x][y];

      // This number is critical
      // If it´s to low (like 1.1) the fire dosn´t go up far enough.
      // If it´s to high (like 3) the fire goes up too high.
      // It depends on the framerate which number is best.
      // If the number is not right you loose the uplifting fire clouds
      // which seperate themself while rising up.
      //dim = dim / 3; //started 1.4

      float LightBrightness = map(analogRead(BrightnessPotPin), 0, 1023, 0, 255);
      LightBrightness = LightBrightness / 5.0;
      if(LightBrightness == 0) { LightBrightness = 0.01; }
      dim = dim / LightBrightness; //started 1.4
      
      dim = 255 - dim;


      // here happens the scaling of the heatmap
      heat[XY(x, y)] = scale8(heat[XY(x, y)] , dim);
    }
  }

  // Now just map the colors based on the heatmap.
  for (uint8_t y = 0; y < Height - 1; y++) {
    for (uint8_t x = 0; x < Width; x++) {
      leds[XY(x, y)] = ColorFromPalette( Pal, heat[XY(x, y)]);
    }
  }

  // Done. Bring it on!
  FastLED.show();

  // I hate this delay but with 8 bit scaling there is no way arround.
  // If the framerate gets too high the frame by frame scaling doesn´s work anymore.
  // Basically it does but it´s impossible to see then...

  // If you change the framerate here you need to adjust the
  // y speed and the dim divisor, too.
  delay(5); //initial 25

}
