#include "FastLED.h"

// matrix size
uint8_t Width  = 6;
uint8_t Height = 23;
uint8_t CentreX =  (Width / 2) - 1;
uint8_t CentreY = (Height / 2) - 1;

// NUM_LEDS = Width * Height
#define NUM_LEDS      138
#define BRIGHTNESS    50
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
CRGB leds[NUM_LEDS];


// parameters and buffer for the noise array
#define NUM_LAYERS 2
uint32_t x[NUM_LAYERS];
uint32_t y[NUM_LAYERS];
uint32_t z[NUM_LAYERS];
uint32_t scale_x[NUM_LAYERS];
uint32_t scale_y[NUM_LAYERS];

uint8_t noise[NUM_LAYERS][6][23];
uint8_t noise2[NUM_LAYERS][6][23];

uint8_t heat[138];

void setup() {

  Serial.begin(115200);
  // Adjust this for you own setup. Use the hardware SPI pins if possible.
  // On Teensy 3.1/3.2 the pins are 11 & 13
  // Details here: https://github.com/FastLED/FastLED/wiki/SPI-Hardware-or-Bit-banging
  // In case you see flickering / glitching leds, reduce the data rate to 12 MHZ or less
  //LEDS.addLeds<APA102, 11, 13, BGR, DATA_RATE_MHZ(12)>(leds, NUM_LEDS);
  FastLED.addLeds<CHIPSET, 6, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setDither(DISABLE_DITHER);
}

void loop() {
  Fire2018_2();
  //show_fps();
}

// check the Serial Monitor for fps rate
void show_fps() {
  EVERY_N_MILLIS(100) {
    Serial.println(LEDS.getFPS());
  }
}

// this finds the right index within a serpentine matrix
uint16_t XY( uint8_t x, uint8_t y) {
  uint16_t i;
  if ( y & 0x01) {
    uint8_t reverseX = (Width - 1) - x;
    i = (y * Width) + reverseX;
  } else {
    i = (y * Width) + x;
  }
  return i;
}

void Fire2018_2() {

  // some changing values
  uint16_t ctrl1 = inoise16(11 * millis(), 0, 0);
  uint16_t ctrl2 = inoise16(13 * millis(), 100000, 100000);
  uint16_t  ctrl = ((ctrl1 + ctrl2) / 2);

  // parameters for the heatmap
  uint16_t speed = 25;
  x[0] = 3 * ctrl * speed;
  y[0] = 20 * millis() * speed;
  z[0] = 5 * millis() * speed ;
  scale_x[0] = ctrl1 / 2;
  scale_y[0] = ctrl2 / 2;

  //calculate the noise data
  uint8_t layer = 0;
  for (uint8_t i = 0; i < Width; i++) {
    uint32_t ioffset = scale_x[layer] * (i - CentreX);
    for (uint8_t j = 0; j < Height; j++) {
      uint32_t joffset = scale_y[layer] * (j - CentreY);
      uint16_t data = ((inoise16(x[layer] + ioffset, y[layer] + joffset, z[layer])) + 1);
      noise[layer][i][j] = data >> 8;
    }
  }

  // parameters for te brightness mask
  speed = 20;
  x[1] = 3 * ctrl * speed;
  y[1] = 20 * millis() * speed;
  z[1] = 5 * millis() * speed ;
  scale_x[1] = ctrl1 / 2;
  scale_y[1] = ctrl2 / 2;

  //calculate the noise data
  layer = 1;
  for (uint8_t i = 0; i < Width; i++) {
    uint32_t ioffset = scale_x[layer] * (i - CentreX);
    for (uint8_t j = 0; j < Height; j++) {
      uint32_t joffset = scale_y[layer] * (j - CentreY);
      uint16_t data = ((inoise16(x[layer] + ioffset, y[layer] + joffset, z[layer])) + 1);
      noise[layer][i][j] = data >> 8;
    }
  }

  // draw lowest line - seed the fire
  for (uint8_t x = 0; x < Width; x++) {
    heat[XY(x, 15)] =  noise[0][15 - x][7];
  }


  //copy everything one line up
  for (uint8_t y = 0; y < Height - 1; y++) {
    for (uint8_t x = 0; x < Width; x++) {
      heat[XY(x, y)] = heat[XY(x, y + 1)];
    }
  }

  //dim
  for (uint8_t y = 0; y < Height - 1; y++) {
    for (uint8_t x = 0; x < Width; x++) {
      uint8_t dim = noise[0][x][y];
      // high value = high flames
      dim = dim / 1.7;
      dim = 255 - dim;
      heat[XY(x, y)] = scale8(heat[XY(x, y)] , dim);
    }
  }

  for (uint8_t y = 0; y < Height; y++) {
    for (uint8_t x = 0; x < Width; x++) {
      // map the colors based on heatmap
      leds[XY(x, y)] = CRGB( heat[XY(x, y)], 1 , 0);
      // dim the result based on 2nd noise layer
      leds[XY(x, y)].nscale8(noise[1][x][y]);
    }
  }

  FastLED.show();
  delay(8);

}
