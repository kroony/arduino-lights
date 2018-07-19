 /*
### teldredge
### www.funkboxing.com
### teldredge1979@gmail.com

### FAST_SPI2 LED FX EXAMPLES
### MOSTLY DESIGNED FOR A LOOP/RING OF LEDS (ORIGINALLY FOR A SIGN)
### BUT PLENTY USEFUL FOR A LINEAR STRIP TOO

### NOTES:
### - THIS IS REALLY AN EXAMPLE LIBRARY MORE THAN A MODULE OR ANYTHING, MEANING YOU'LL PROBABLY HAVE TO CUT AND PASTE
### - SO FAR TESTED WITH WS2801 AND WS2811 USING ARDUINO NANO 328
### - USES THE 'SERIALCOMMAND' LIBRARY TO CHANGE 'MODES', BUT THESE FXNS COULD BE LOOPED ANYWAY YOU WANT

### LICENSE:::USE FOR WHATEVER YOU WANT, WHENEVER YOU WANT, WHYEVER YOU WANT
### BUT YOU MUST YODEL ONCE FOR FREEDOM AND MAYBE DONATE TO SOMETHING WORTHWHILE

### 20140210 Adapted to OctoWS2811 library (stefan@clumsy.ch)

--------------------------------------------------------------------------------------------------
*/

/*----------------------------------------------------|
|                                                     |
|           FROM THE FAST_SPI2 EXAMPLE FILE           |
|                                                     |
|----------------------------------------------------*/
// Uncomment this line if you have any interrupts that are changing pins - this causes the library to be a little bit more cautious
// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1

// Uncomment this line to force always using software, instead of hardware, SPI (why?)
// #define FORCE_SOFTWARE_SPI 1

// Uncomment this line if you want to talk to DMX controllers
// #define FASTSPI_USE_DMX_SIMPLE 1

#include <OctoWS2811.h>

// #include <SerialCommand.h>

#define NUM_LEDS 50
#define ledsPerStrip 50

// Data pin that led data will be written out over
#define DATA_PIN 3
int BOTTOM_INDEX = 0;
int TOP_INDEX = int(NUM_LEDS/2);
int EVENODD = NUM_LEDS%2;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];
int led = 13;
const int config = WS2811_RGB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

int ledsX[NUM_LEDS]; //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, ETC)

// SerialCommand sCmd;     //-SETUP SerialCommand OBJECT

//int ledMode = 888;      //-START IN DEMO MODE (THIS MODE BLOCKS OUT SERIAL COMMANDS FOR AWHILE)
//int ledMode = 8;      //-START IN POLICE MODE
//int ledMode = 2;      //-START IN CYLON MODE
int ledMode = 888;

//-PERISTENT VARS
int idex = 0;        //-LED INDEX (0 to NUM_LEDS-1
int idx_offset = 0;  //-OFFSET INDEX (BOTTOM LED TO ZERO WHEN LOOP IS TURNED/DOESN'T REALLY WORK)
int ihue = 0;        //-HUE (0-360)
int ibright = 0;     //-BRIGHTNESS (0-255)
int isat = 0;        //-SATURATION (0-255)
int bouncedirection = 0;  //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;      //-INC VAR FOR SIN LOOPS
int lcount = 0;      //-ANOTHER COUNTING VAR

//------------------------------------- UTILITY FXNS --------------------------------------

void colorWipe(int color, int wait) {
//  for (int i=0; i < leds.numPixels(); i++) {
    for (int i=0; i < NUM_LEDS; i++) {
      leds.setPixel(i, color);
      leds.show();
      delayMicroseconds(wait);
   
  }
}


//-FIND INDEX OF HORIZONAL OPPOSITE LED
int horizontal_index(int i) {
  //-ONLY WORKS WITH INDEX < TOPINDEX
  if (i == BOTTOM_INDEX) {return BOTTOM_INDEX;}
  if (i == TOP_INDEX && EVENODD == 1) {return TOP_INDEX + 1;}
  if (i == TOP_INDEX && EVENODD == 0) {return TOP_INDEX;}
  return NUM_LEDS - i;  
}

//-FIND INDEX OF ANTIPODAL OPPOSITE LED
int antipodal_index(int i) {
  //int N2 = int(NUM_LEDS/2);
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {iN = ( i + TOP_INDEX ) % NUM_LEDS; }
  return iN;
}


//-FIND ADJACENT INDEX CLOCKWISE
int adjacent_cw(int i) {
  int r;
  if (i < NUM_LEDS - 1) {r = i + 1;}
  else {r = 0;}
  return r;
}


//-FIND ADJACENT INDEX COUNTER-CLOCKWISE
int adjacent_ccw(int i) {
  int r;
  if (i > 0) {r = i - 1;}
  else {r = NUM_LEDS - 1;}
  return r;
}


//-CONVERT HSV VALUE TO RGB
void HSVtoRGB(int hue, int sat, int val, int colors[3]) {
  // hue: 0-359, sat: 0-255, val (lightness): 0-255
  int r, g, b, base;

  if (sat == 0) { // Achromatic color (gray).
    colors[0]=val;
    colors[1]=val;
    colors[2]=val;
  } else  {
    base = ((255 - sat) * val)>>8;
    switch(hue/60) {
      case 0:
        r = val;
        g = (((val-base)*hue)/60)+base;
        b = base;
        break;
      case 1:
        r = (((val-base)*(60-(hue%60)))/60)+base;
        g = val;
        b = base;
        break;
      case 2:
        r = base;
        g = val;
        b = (((val-base)*(hue%60))/60)+base;
        break;
      case 3:
        r = base;
        g = (((val-base)*(60-(hue%60)))/60)+base;
        b = val;
        break;
      case 4:
        r = (((val-base)*(hue%60))/60)+base;
        g = base;
        b = val;
        break;
      case 5:
        r = val;
        g = base;
        b = (((val-base)*(60-(hue%60)))/60)+base;
        break;
    }
    colors[0]=r;
    colors[1]=g;
    colors[2]=b;
  }
}


void copy_led_array(){
  for(int i = 0; i < NUM_LEDS; i++ ) {
    ledsX[i] = leds.getPixel(i);
  }  
}

/*
void print_led_arrays(int ilen){
  copy_led_array();
  Serial.println("~~~***ARRAYS|idx|led-r-g-b|ledX-0-1-2");
  for(int i = 0; i < ilen; i++ ) {
      Serial.print("~~~");
      Serial.print(i);
      Serial.print("|");      
      Serial.print(leds[i].r);
      Serial.print("-");
      Serial.print(leds[i].g);
      Serial.print("-");
      Serial.print(leds[i].b);
      Serial.print("|");      
      Serial.print(ledsX[i][0]);
      Serial.print("-");
      Serial.print(ledsX[i][1]);
      Serial.print("-");
      Serial.println(ledsX[i][2]);
    }  
}
*/

//------------------------LED EFFECT FUNCTIONS------------------------

void one_color_all(int cred, int cgrn, int cblu) { //-SET ALL LEDS TO ONE COLOR
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      leds.setPixel(i, cred, cgrn, cblu);
      leds.show();       
      delay(1);
    }  
}

void one_color_allNOSHOW(int cred, int cgrn, int cblu) { //-SET ALL LEDS TO ONE COLOR
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      leds.setPixel(i, cred, cgrn, cblu);
    }  
}


void rainbow_fade(int idelay) { //-FADE ALL LEDS THROUGH HSV RAINBOW
    ihue++;
    if (ihue >= 359) {ihue = 0;}
    int thisColor[3];
    HSVtoRGB(ihue, 255, 255, thisColor);
    for(int idex = 0 ; idex < NUM_LEDS; idex++ ) {
      leds.setPixel(idex,thisColor[0],thisColor[1],thisColor[2]); 
    }
    leds.show();    
    delay(idelay);
}


void rainbow_loop(int istep, int idelay) { //-LOOP HSV RAINBOW
  idex++;
  ihue = ihue + istep;
  int icolor[3];  

  if (idex >= NUM_LEDS) {idex = 0;}
  if (ihue >= 359) {ihue = 0;}

  HSVtoRGB(ihue, 255, 255, icolor);
  leds.setPixel(idex, icolor[0], icolor[1], icolor[2]);
  leds.show();
  delay(idelay);
}


void random_burst(int idelay) { //-RANDOM INDEX/COLOR
  int icolor[3];  
  
  idex = random(0,NUM_LEDS);
  ihue = random(0,359);

  HSVtoRGB(ihue, 255, 255, icolor);
  leds.setPixel(idex, icolor[0], icolor[1], icolor[2]);
  leds.show();  
  delay(idelay);
}


void color_bounce(int idelay) { //-BOUNCE COLOR (SINGLE LED)
  if (bouncedirection == 0) {
    idex = idex + 1;
    if (idex == NUM_LEDS) {
      bouncedirection = 1;
      idex = idex - 1;
    }
  }
  if (bouncedirection == 1) {
    idex = idex - 1;
    if (idex == 0) {
      bouncedirection = 0;
    }
  }  
  for(int i = 0; i < NUM_LEDS; i++ ) {
    if (i == idex) {leds.setPixel(i, 255, 0, 0);}
    else {leds.setPixel(i, 0, 0, 0);}
  }
  leds.show();
  delay(idelay);
}


void police_lightsONE(int idelay) { //-POLICE LIGHTS (TWO COLOR SINGLE LED)
  idex++;
  if (idex >= NUM_LEDS) {idex = 0;}
  int idexR = idex;
  int idexB = antipodal_index(idexR);  
  for(int i = 0; i < NUM_LEDS; i++ ) {
    if (i == idexR) {leds.setPixel(i, 255, 0, 0);}
    else if (i == idexB) {leds.setPixel(i, 0, 0, 255);}    
    else {leds.setPixel(i, 0, 0, 0);}
  }
  leds.show();  
  delay(idelay);
}


void police_lightsALL(int idelay) { //-POLICE LIGHTS (TWO COLOR SOLID)
  idex++;
  if (idex >= NUM_LEDS) {idex = 0;}
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  leds.setPixel(idexR, 255, 0, 0);
  leds.setPixel(idexB, 0, 0, 255);
  leds.show();  
  delay(idelay);
}


void color_bounceFADE(int idelay) { //-BOUNCE COLOR (SIMPLE MULTI-LED FADE)
  if (bouncedirection == 0) {
    idex = idex + 1;
    if (idex == NUM_LEDS) {
      bouncedirection = 1;
      idex = idex - 1;
    }
  }
  if (bouncedirection == 1) {
    idex = idex - 1;
    if (idex == 0) {
      bouncedirection = 0;
    }
  }
  int iL1 = adjacent_cw(idex);
  int iL2 = adjacent_cw(iL1);
  int iL3 = adjacent_cw(iL2);  
  int iR1 = adjacent_ccw(idex);
  int iR2 = adjacent_ccw(iR1);
  int iR3 = adjacent_ccw(iR2); 
  
  for(int i = 0; i < NUM_LEDS; i++ ) {
    if (i == idex) {leds.setPixel(i, 255, 0, 0);}
    else if (i == iL1) {leds.setPixel(i, 100, 0, 0);}
    else if (i == iL2) {leds.setPixel(i, 50, 0, 0);}
    else if (i == iL3) {leds.setPixel(i, 10, 0, 0);}        
    else if (i == iR1) {leds.setPixel(i, 100, 0, 0);}
    else if (i == iR2) {leds.setPixel(i, 50, 0, 0);}
    else if (i == iR3) {leds.setPixel(i, 10, 0, 0);}    
    else {leds.setPixel(i, 0, 0, 0);}
  }
 
  leds.show();
  delay(idelay);
}


void flicker(int thishue, int thissat) {
  int random_bright = random(0,255);
  int random_delay = random(10,100);
  int random_bool = random(0,random_bright);
  int thisColor[3];
  
  if (random_bool < 10) {
    HSVtoRGB(thishue, thissat, random_bright, thisColor);

    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      leds.setPixel(i, thisColor[0], thisColor[1], thisColor[2]);
    }
    
    leds.show();    
    delay(random_delay);
  }
}


void pulse_one_color_all(int ahue, int idelay) { //-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR 

  if (bouncedirection == 0) {
    ibright++;
    if (ibright >= 255) {bouncedirection = 1;}
  }
  if (bouncedirection == 1) {
    ibright = ibright - 1;
    if (ibright <= 1) {bouncedirection = 0;}         
  }  
    
  int acolor[3];
  HSVtoRGB(ahue, 255, ibright, acolor);
  
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      leds.setPixel(i, acolor[0], acolor[1], acolor[2]);
    }
    leds.show();    
    delay(idelay);
}


void pulse_one_color_all_rev(int ahue, int idelay) { //-PULSE SATURATION ON ALL LEDS TO ONE COLOR 

  if (bouncedirection == 0) {
    isat++;
    if (isat >= 255) {bouncedirection = 1;}
  }
  if (bouncedirection == 1) {
    isat = isat - 1;
    if (isat <= 1) {bouncedirection = 0;}         
  }  
    
  int acolor[3];
  HSVtoRGB(ahue, isat, 255, acolor);
  
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      leds.setPixel(i, acolor[0], acolor[1], acolor[2]);
    }
    leds.show();
    delay(idelay);
}


void random_red() { //QUICK 'N DIRTY RANDOMIZE TO GET CELL AUTOMATA STARTED  
  int temprand;
  for(int i = 0; i < NUM_LEDS; i++ ) {
    temprand = random(0,100);
    if (temprand > 50) {leds.setPixel(i, 255, 0, 0);}
    if (temprand <= 50) {leds.setPixel(i, 0, 0, 0);}
  }
  leds.show();  
}

/*
void rule30(int idelay) { //1D CELLULAR AUTOMATA - RULE 30 (RED FOR NOW)
  copy_led_array();
  int iCW;
  int iCCW;
  int y = 100;  
  for(int i = 0; i < NUM_LEDS; i++ ) {
    iCW = adjacent_cw(i);
    iCCW = adjacent_ccw(i);
    if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] > y) {leds[i].r = 0;}
    if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] <= y) {leds[i].r = 0;}
    if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] > y) {leds[i].r = 0;}
    if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] <= y) {leds[i].r = 255;}
    if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] > y) {leds[i].r = 255;}
    if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] <= y) {leds[i].r = 255;}
    if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] > y) {leds[i].r = 255;}
    if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] <= y) {leds[i].r = 0;}
  }
  
  leds.show();  
  delay(idelay);
}
*/


void random_march(int idelay) { //RANDOM MARCH CCW
  copy_led_array();
  int iCCW;
  
  int acolor[3];
  HSVtoRGB(random(0,360), 255, 255, acolor);
  
//  leds[0].r = acolor[0];
//  leds[0].g = acolor[1];
//  leds[0].b = acolor[2];
  leds.setPixel(0, acolor[0], acolor[1], acolor[2]);
  
  for(int i = 1; i < NUM_LEDS ; i++ ) {  //-GET/SET EACH LED COLOR FROM CCW LED
    iCCW = adjacent_ccw(i);
//    leds[i].r = ledsX[iCCW][0];
//    leds[i].g = ledsX[iCCW][1];
//    leds[i].b = ledsX[iCCW][2];
    leds.setPixel(i, ledsX[iCCW]);
  }
  
  leds.show();  
  delay(idelay);
}


void rwb_march(int idelay) { //R,W,B MARCH CCW
  copy_led_array();
  int iCCW;
  
  idex++;
  if (idex > 2) {idex = 0;}
  
  switch (idex) {
    case 0:
//      leds[0].r = 255;
//      leds[0].g = 0;
//      leds[0].b = 0;
      leds.setPixel(0, 255, 0, 0);
    break;
    case 1:
//      leds[0].r = 255;
//      leds[0].g = 255;
//      leds[0].b = 255;
      leds.setPixel(0, 255, 255, 255);
    break;
    case 2:
//      leds[0].r = 0;
//      leds[0].g = 0;
//      leds[0].b = 255;
      leds.setPixel(0, 0, 0, 255);
    break;
  }
      
    for(int i = 1; i < NUM_LEDS; i++ ) {  //-GET/SET EACH LED COLOR FROM CCW LED
      iCCW = adjacent_ccw(i);
      leds.setPixel(i, ledsX[iCCW]);
    }
  
  leds.show();  
  delay(idelay);
}


void white_temps() {
  int N9 = int(NUM_LEDS/9);
  for (int i = 0; i < NUM_LEDS; i++ ) {
    if (i >= 0 && i < N9) {leds.setPixel(i, 255, 147, 41);} //-CANDLE - 1900
    if (i >= N9 && i < N9*2) {leds.setPixel(i, 255, 197, 143);} //-40W TUNG - 2600
    if (i >= N9*2 && i < N9*3) {leds.setPixel(i, 255, 214, 170);} //-100W TUNG - 2850
    if (i >= N9*3 && i < N9*4) {leds.setPixel(i, 255, 241, 224);} //-HALOGEN - 3200
    if (i >= N9*4 && i < N9*5) {leds.setPixel(i, 255, 250, 244);} //-CARBON ARC - 5200
    if (i >= N9*5 && i < N9*6) {leds.setPixel(i, 255, 255, 251);} //-HIGH NOON SUN - 5400
    if (i >= N9*6 && i < N9*7) {leds.setPixel(i, 255, 255, 255);} //-DIRECT SUN - 6000
    if (i >= N9*7 && i < N9*8) {leds.setPixel(i, 201, 226, 255);} //-OVERCAST SKY - 7000
    if (i >= N9*8 && i < NUM_LEDS) {leds.setPixel(i, 64, 156, 255);} //-CLEAR BLUE SKY - 20000  
  }
  leds.show();  
  delay(100);
}


void color_loop_vardelay() { //-COLOR LOOP (SINGLE LED) w/ VARIABLE DELAY
  idex++;
  if (idex > NUM_LEDS) {idex = 0;}

  int acolor[3];
  HSVtoRGB(0, 255, 255, acolor);
  
  int di = abs(TOP_INDEX - idex); //-DISTANCE TO CENTER    
  int t = constrain((10/di)*10, 10, 500); //-DELAY INCREASE AS INDEX APPROACHES CENTER (WITHIN LIMITS)
  
  for(int i = 0; i < NUM_LEDS; i++ ) {
    if (i == idex) {
      leds.setPixel(i, acolor[0], acolor[1], acolor[2]);
    }
    else {
      leds.setPixel(i, 0, 0, 0);
    }
  }
  
  leds.show();  
  delay(t);
}

void color_loop_randdelay() { //-COLOR LOOP (SINGLE LED) w/ VARIABLE DELAY
  int dir=random(0,2);
  int steps=random(1,NUM_LEDS);
  int hue=random(0,256);
  int sat=random(10,256);
  int val=random(128,256);
  for (int j=0; j<steps; j++) { 
  if (dir<1) {idex++;} else {idex--;}
  if (idex > NUM_LEDS) {idex = 0;}
  if (idex < 0) {idex = NUM_LEDS-1;}

  int acolor[3];
  HSVtoRGB(hue, sat, val, acolor);
//  HSVtoRGB(360/NUM_LEDS*idex, 255, 255, acolor);
  
  int di = abs(TOP_INDEX - idex); //-DISTANCE TO CENTER    
  int t = random(1,15); //-DELAY INCREASE AS INDEX APPROACHES CENTER (WITHIN LIMITS)
  t = t*t;
  
  for(int i = 0; i < NUM_LEDS; i++ ) {
    if (i == idex) {
      leds.setPixel(i, acolor[0], acolor[1], acolor[2]);
    }
    else {
//      int bcolor[3];
//      HSVtoRGB(360/NUM_LEDS*i, 255, 10, bcolor);
//      leds.setPixel(i, bcolor[0], bcolor[1], bcolor[2]);
      leds.setPixel(i, 0, 0, 0);
    }
  }
  
  leds.show();  
  delay(t);
  }
}
  
void strip_march_cw(int idelay) { //-MARCH STRIP C-W
  copy_led_array();
  int iCCW;  
  for(int i = 0; i < NUM_LEDS; i++ ) {  //-GET/SET EACH LED COLOR FROM CCW LED
    iCCW = adjacent_ccw(i);
//    leds[i].r = ledsX[iCCW][0];
//    leds[i].g = ledsX[iCCW][1];
//    leds[i].b = ledsX[iCCW][2];    
    leds.setPixel(i, ledsX[iCCW]);
  }
  leds.show();  
  delay(idelay);
}


void strip_march_ccw(int idelay) { //-MARCH STRIP C-W 
  copy_led_array();
  int iCW;  
  for(int i = 0; i < NUM_LEDS; i++ ) {  //-GET/SET EACH LED COLOR FROM CCW LED
    iCW = adjacent_cw(i);
//    leds[i].r = ledsX[iCW][0];
//    leds[i].g = ledsX[iCW][1];
//    leds[i].b = ledsX[iCW][2];   
    leds.setPixel(i, ledsX[iCW]);
 
  }
  leds.show();  
  delay(idelay);
}


void pop_horizontal(int ahue, int idelay) {  //-POP FROM LEFT TO RIGHT UP THE RING
  int acolor[3];
  HSVtoRGB(ahue, 255, 255, acolor);
  
  int ix;
  
  if (bouncedirection == 0) {
    bouncedirection = 1;
    ix = idex;  
  }
  else if (bouncedirection == 1) {
    bouncedirection = 0;
    ix = horizontal_index(idex);
    idex++;
    if (idex > TOP_INDEX) {idex = 0;}      
  }
  
  for(int i = 0; i < NUM_LEDS; i++ ) {
    if (i == ix) {leds.setPixel(i, acolor[0], acolor[1], acolor[2]);}
    else {leds.setPixel(i, 0, 0, 0);}
  }
  
  leds.show();  
  delay(idelay);    
}


void quad_bright_curve(int ahue, int idelay) {  //-QUADRATIC BRIGHTNESS CURVER
  int acolor[3];
  int ax;
    
  for(int x = 0; x < NUM_LEDS; x++ ) {
    if (x <= TOP_INDEX) {ax = x;}
    else if (x > TOP_INDEX) {ax = NUM_LEDS-x;}

    int a = 1; int b = 1; int c = 0;
        
    int iquad = -(ax*ax*a)+(ax*b)+c; //-ax2+bx+c
    int hquad = -(TOP_INDEX*TOP_INDEX*a)+(TOP_INDEX*b)+c; //HIGHEST BRIGHTNESS
    
    ibright = int((float(iquad)/float(hquad))*255);
      
    HSVtoRGB(ahue, 255, ibright, acolor);
  
//    leds[x].r = acolor[0]; leds[x].g = acolor[1]; leds[x].b = acolor[2];
    leds.setPixel(x, acolor[0], acolor[1], acolor[2]);

  
  }
  leds.show();  
  delay(idelay);
}


void flame() {
  int acolor[3];
  int idelay = random(0,10);
  
  float hmin = 0.1; float hmax = 45.0;
  float hdif = hmax-hmin;
  int randtemp = random(0,3);
  float hinc = (hdif/float(TOP_INDEX))+randtemp;

  int ahue = hmin;
  for(int i = 0; i < TOP_INDEX; i++ ) {
    
    ahue = ahue + hinc;

    HSVtoRGB(ahue, 255, 255, acolor);
    
//    leds[i].r = acolor[0]; leds[i].g = acolor[1]; leds[i].b = acolor[2]; 
    leds.setPixel(i, acolor[0], acolor[1], acolor[2]);

    int ih = horizontal_index(i);
//    leds[ih].r = acolor[0]; leds[ih].g = acolor[1]; leds[ih].b = acolor[2];
    leds.setPixel(ih, acolor[0], acolor[1], acolor[2]);

    
//    leds[TOP_INDEX].r = 255; leds[TOP_INDEX].g = 255; leds[TOP_INDEX].b = 255;
        leds.setPixel(TOP_INDEX, 255, 255, 255);

  
    leds.show();    
    delay(idelay);
  }
}


void radiation(int ahue, int idelay) { //-SORT OF RADIATION SYMBOLISH- 
  //int N2 = int(NUM_LEDS/2);
  int N3 = int(NUM_LEDS/3);
  int N6 = int(NUM_LEDS/6);  
  int N12 = int(NUM_LEDS/12);  
  int acolor[3];

  for(int i = 0; i < N6; i++ ) { //-HACKY, I KNOW...
    tcount = tcount + .02;
    if (tcount > 3.14) {tcount = 0.0;}
    ibright = int(sin(tcount)*255);
    
    int j0 = (i + NUM_LEDS - N12) % NUM_LEDS;
    int j1 = (j0+N3) % NUM_LEDS;
    int j2 = (j1+N3) % NUM_LEDS;    
    HSVtoRGB(ahue, 255, ibright, acolor);  
//    leds[j0].r = acolor[0]; leds[j0].g = acolor[1]; leds[j0].b = acolor[2];
    leds.setPixel(j0, acolor[0], acolor[1], acolor[2]);

//    leds[j1].r = acolor[0]; leds[j1].g = acolor[1]; leds[j1].b = acolor[2];
    leds.setPixel(j1, acolor[0], acolor[1], acolor[2]);
//    leds[j2].r = acolor[0]; leds[j2].g = acolor[1]; leds[j2].b = acolor[2];    
    leds.setPixel(j2, acolor[0], acolor[1], acolor[2]);

  }    
    leds.show();    
    delay(idelay);    
}

void swiss_cross(int idelay) { //-swiss cross
  //int N2 = int(NUM_LEDS/2);
  int N4 = int(NUM_LEDS/4);
  int count = NUM_LEDS * 0.1;
    
  one_color_allNOSHOW(255,0,0);
  for(int i = 0; i < 4; i++) {
    for(int j = -(count/2); j < (count/2); j++) {
      int pixel = (N4*i+j);
      if (pixel < 0) { pixel += NUM_LEDS;Â }
      leds.setPixel(pixel, 255, 255, 255);
    }
  }    
    leds.show();    
    delay(idelay);    
}

void germany(int idelay) { //-german flag
  //int N2 = int(NUM_LEDS/2);
  one_color_allNOSHOW(0,0,0);
  int split = 12;
  for(int i = 0; i < NUM_LEDS; i++) {
    if ( i < (NUM_LEDS/split)) { leds.setPixel(i, 255, 0, 0);}
    else if ( i < (NUM_LEDS/split*(split/2-1))) { leds.setPixel(i, 0, 0, 0);}
    else if ( i < (NUM_LEDS/split*(split/2+1))) { leds.setPixel(i, 255, 0, 0);}
    else if ( i < (NUM_LEDS/split*(split-1))) { leds.setPixel(i, 255, 204, 0);}
    else {leds.setPixel(i, 255, 0, 0);}
  }    
    leds.show();    
    delay(idelay);    
}

void sin_bright_wave(int ahue, int idelay) {  
  int acolor[3];

  for(int i = 0; i < NUM_LEDS; i++ ) {
    tcount = tcount + .1;
    if (tcount > 3.14) {tcount = 0.0;}
    ibright = int(sin(tcount)*255);

    HSVtoRGB(ahue, 255, ibright, acolor);
  
//    leds[i].r = acolor[0]; leds[i].g = acolor[1]; leds[i].b = acolor[2]; 
    leds.setPixel(i, acolor[0], acolor[1], acolor[2]);

    leds.show();    
    delay(idelay);
  }
}


void fade_vertical(int ahue, int idelay) { //-FADE 'UP' THE LOOP
  idex++;
  if (idex > TOP_INDEX) {idex = 0;}  
  int idexA = idex;
  int idexB = horizontal_index(idexA);
  
  ibright = ibright + 10;
  if (ibright > 255) {ibright = 0;}
  int acolor[3];
  HSVtoRGB(ahue, 255, ibright, acolor);
  
  leds.setPixel(idexA, acolor[0], acolor[1], acolor[2]);  
  leds.setPixel(idexB, acolor[0], acolor[1], acolor[2]);  
 
  leds.show();  
  delay(idelay);
}


void rainbow_vertical(int istep, int idelay) { //-RAINBOW 'UP' THE LOOP
  idex++;
  if (idex > TOP_INDEX) {idex = 0;}  
  ihue = ihue + istep;
  if (ihue > 359) {ihue = 0;}
  Serial.println(ihue);  
  int idexA = idex;
  int idexB = horizontal_index(idexA);

  int acolor[3];
  HSVtoRGB(ihue, 255, 255, acolor);

  leds.setPixel(idexA, acolor[0], acolor[1], acolor[2]);  
  leds.setPixel(idexB, acolor[0], acolor[1], acolor[2]);  
 
  leds.show();  
  delay(idelay);
}


void pacman(int idelay) { //-MARCH STRIP C-W
  int s = int(NUM_LEDS/4);
  lcount++;
  if (lcount > 5) {lcount = 0;}
  if (lcount == 0) {
    one_color_allNOSHOW(255,255,0);}
  if (lcount == 1 || lcount == 5) {
      one_color_allNOSHOW(255,255,0);
//      leds[s].r = 0; leds[s].g = 0; leds[s].b = 0;}
      leds.setPixel(s, 0, 0, 0);}

  if (lcount == 2 || lcount == 4) {
      one_color_allNOSHOW(255,255,0);
//      leds[s-1].r = 0; leds[s-1].g = 0; leds[s-1].b = 0; 
      leds.setPixel(s-1, 0, 0, 0);
 
//      leds[s].r = 0; leds[s].g = 0; leds[s].b = 0;
      leds.setPixel(s, 0, 0, 0);

//      leds[s+1].r = 0; leds[s+1].g = 0; leds[s+1].b = 0;}        
      leds.setPixel(s+1, 0, 0, 0);}

  if (lcount == 3) {
      one_color_allNOSHOW(255,255,0);
//      leds[s-2].r = 0; leds[s-2].g = 0; leds[s-2].b = 0;
      leds.setPixel(s-2, 0, 0, 0);  
//      leds[s-1].r = 0; leds[s-1].g = 0; leds[s-1].b = 0;  
      leds.setPixel(s-1, 0, 0, 0);
//      leds[s].r = 0; leds[s].g = 0; leds[s].b = 0;
      leds.setPixel(s, 0, 0, 0);
//      leds[s+1].r = 0; leds[s+1].g = 0; leds[s+1].b = 0;
      leds.setPixel(s+1, 0, 0, 0);
//      leds[s+2].r = 0; leds[s+2].g = 0; leds[s+2].b = 0;}
      leds.setPixel(s+2, 0, 0, 0);}
  
  leds.show();  
  delay(idelay);
}

void scannerUp(int color, int wait)
{
//  for (int i=0; i < leds.numPixels(); i++) {

    int current = leds.getPixel(0);
    for (int i=0; i < NUM_LEDS; i++) {
    leds.setPixel(i, color);
    leds.show();
    leds.setPixel(i+1, color);
    leds.show();
    leds.setPixel(i+2, color);
    leds.show();
    leds.setPixel(i, current);
    leds.show();
    delay(wait);
  }
}
void scannerDown(int color, int wait)
{
    int current = leds.getPixel(NUM_LEDS-1);
    for (int i=NUM_LEDS-1; i >= 0; i--) {
    leds.setPixel(i, color);
    leds.show();
    leds.setPixel(i-1, color);
    leds.show();
    leds.setPixel(i-2, color);
    leds.show();
    leds.setPixel(i, current);
    leds.show();
    delay(wait);
  }
}

void scannerUp2(int start, int color, int wait)
{
//  for (int i=0; i < leds.numPixels(); i++) {

    int current = leds.getPixel(start);
    for (int i=start; i < NUM_LEDS; i=i+2) {
    leds.setPixel(i, color);
    leds.show();
    leds.setPixel(i+2, color);
    leds.show();
    leds.setPixel(i+4, color);
    leds.show();
    leds.setPixel(i, current);
    leds.show();
    delay(wait);
  }
}
void scannerDown2(int start, int color, int wait)
{
    int current = leds.getPixel(start);
    for (int i=start; i >= 0; i=i-2) {
    leds.setPixel(i, color);
    leds.show();
    leds.setPixel(i-2, color);
    leds.show();
    leds.setPixel(i-4, color);
    leds.show();
    leds.setPixel(i, current);
    leds.show();
    delay(wait);
  }
}



void scannerMirror(int color, int wait)
{
//  for (int i=0; i < leds.numPixels(); i++) {
    for (int i=0; i < NUM_LEDS; i++) {
    leds.setPixel(i, color);
    leds.setPixel(i+1, color);
    leds.setPixel(i+2, color);
    leds.setPixel(NUM_LEDS-i, color);
    leds.setPixel(NUM_LEDS-1-i, color);
    leds.setPixel(NUM_LEDS-2-i, color);    
    leds.show();
    leds.setPixel(i, 0x000000);
    leds.setPixel(NUM_LEDS-i, 0x000000);
    leds.show();
    delay(wait);
  }
}

void scannerMirror2(int color, int wait)
{
//  for (int i=0; i < leds.numPixels(); i++) {
    int current = leds.getPixel(0);

    for (int i=0; i < NUM_LEDS; i=i+2) {
    leds.setPixel(i, color);
    leds.setPixel(i+2, color);
    leds.setPixel(i+4, color);
    leds.setPixel(NUM_LEDS-1-i, color);
    leds.setPixel(NUM_LEDS-3-i, color);
    leds.setPixel(NUM_LEDS-5-i, color);    
    leds.show();
    leds.setPixel(i, current);
    leds.setPixel(NUM_LEDS-1-i, current);
    leds.show();
    delay(wait);
  }
}

void bar(int color, int wait)
{
//  for (int i=0; i < leds.numPixels(); i++) {
    for (int i=0; i < NUM_LEDS; i++) {
    leds.setPixel(i, color);
    leds.show();
    delayMicroseconds(wait);
  }
    for (int i=NUM_LEDS-1; i > 0; i--) {
    leds.setPixel(i, 0x000000);
    leds.show();
    delay(wait);
  }
}

void randomPixelColor(int wait)
{
  int rr = random(0,256);
  int rg = random(0,256);
  int rb = random(0,256);
  int pixel = random(0,NUM_LEDS);
  int current = leds.getPixel(pixel);
  leds.setPixel(pixel, rr, rg, rb);
  leds.show();
  delay(wait);
  leds.setPixel(pixel, current);
  leds.show();
  delay(wait);
}

//------------------SETUP------------------
void setup()  
{


//  Serial.begin(9600);      // SETUP HARDWARE SERIAL (USB)
  //Serial.begin(115200);      // SETUP HARDWARE SERIAL (USB)

//  LEDS.setBrightness(128); // SET BRIGHTNESS TO 1/2 POWER

  //LEDS.addLeds<WS2811, 13>(leds, NUM_LEDS);
  //LEDS.addLeds<TM1809, 13>(leds, NUM_LEDS);
  //LEDS.addLeds<UCS1903, 13>(leds, NUM_LEDS);
  //LEDS.addLeds<TM1803, 13>(leds, NUM_LEDS);
  //LEDS.addLeds<LPD8806>(leds, NUM_LEDS)->clearLeds(300);
  
  //LEDS.addLeds<WS2801>(leds, NUM_LEDS);
  //---Put ws2801 strip on the hardware SPI pins with a BGR ordering of rgb and limited to a 1Mhz data rate
  //LEDS.addLeds<WS2801, 11, 13, BGR, DATA_RATE_MHZ(1)>(leds, NUM_LEDS);
//  LEDS.addLeds<WS2811, 22>(leds, NUM_LEDS);
  
  //LEDS.addLeds<SM16716>(leds, NUM_LEDS);
  //LEDS.addLeds<WS2811, 11>(leds, NUM_LEDS);
  //LEDS.addLeds<LPD8806, 10, 11>(leds, NUM_LEDS);
  //LEDS.addLeds<WS2811, 13, BRG>(leds, NUM_LEDS);
  //LEDS.addLeds<LPD8806, BGR>(leds, NUM_LEDS);
  
//  sCmd.addCommand("m",   set_mode_strip);
//  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  
//  one_color_all(0,0,0); //-BLANK STRIP
  
  pinMode(led, OUTPUT);    
  randomSeed(analogRead(0));
  leds.begin();
  colorWipe(0x000000, 0);
  leds.show();
  
//  Serial.println("---SETUP COMPLETE---");
}



void set_mode_strip() {    //-SETS THE MODE (SOME MODES REQUIRE RANDOM STARTS TO WORK RIGHT
  char *arg;
//  arg = sCmd.next();
  
  if (arg != NULL) {
    ledMode = atoi(arg);
  }
  if (ledMode == 13) {  //-FOR CELL AUTO
    random_red();
  }
  
  Serial.print("~~~***NEW MODE-");
  Serial.println(ledMode);
}

void demo_mode(){
  int r = 10;
  for(int i=0; i<r*3; i++) {one_color_all(255,255,255);}
  for(int i=0; i<r*25; i++) {rainbow_fade(20);}
  for(int i=0; i<r*20; i++) {rainbow_loop(10, 20);}
  for(int i=0; i<r*20; i++) {random_burst(20);}
  for(int i=0; i<r*12; i++) {color_bounce(20);}
  for(int i=0; i<r*12; i++) {color_bounceFADE(40);}
  for(int i=0; i<r*5; i++) {police_lightsONE(40);}
  for(int i=0; i<r*5; i++) {police_lightsALL(40);}
  for(int i=0; i<r*35; i++) {flicker(200, 255);}
  for(int i=0; i<r*50; i++) {pulse_one_color_all(0, 10);}
  for(int i=0; i<r*35; i++) {pulse_one_color_all_rev(0, 10);}
  for(int i=0; i<r*5; i++) {fade_vertical(240, 60);}
  for(int i=0; i<r; i++) random_red();
//  for(int i=0; i<r*5; i++) {rule30(100);}
  for(int i=0; i<r*8; i++) {random_march(30);}
  for(int i=0; i<r*5; i++) {rwb_march(80);}
  one_color_all(0,0,0);
  for(int i=0; i<r*15; i++) {radiation(120, 60);}
  for(int i=0; i<r*15; i++) {color_loop_vardelay();}
  for(int i=0; i<r; i++) {color_loop_randdelay();}

  for(int i=0; i<r*5; i++) {white_temps();}
  for(int i=0; i<r; i++) {sin_bright_wave(240, 35);}
  for(int i=0; i<r*5; i++) {pop_horizontal(300, 100);}
  for(int i=0; i<r*5; i++) {quad_bright_curve(240, 100);}
  for(int i=0; i<r*3; i++) {flame();}
  for(int i=0; i<r*10; i++) {pacman(50);}
  for(int i=0; i<r*15; i++) {rainbow_vertical(15, 50);}

  for(int i=0; i<r; i++) {scannerMirror2(0x0000FF, 30);}
  for(int i=0; i<r; i++) {scannerUp(0xA0A0A0, 30);}
  for(int i=0; i<r; i++) {scannerDown(0x4080B0, 30);}
  for(int i=0; i<r*50; i++) {randomPixelColor(50);}
  
  for(int i=0; i<r; i++) {swiss_cross(500);}
  for(int i=0; i<r; i++) {germany(500);}    
  
  for(int i=0; i<r*3; i++) {strip_march_ccw(100);}
  for(int i=0; i<r*3; i++) {strip_march_cw(100);}
  for(int i=0; i<r*2; i++) {one_color_all(255,0,0);}
  for(int i=0; i<r*2; i++) {one_color_all(0,255,0);}
  for(int i=0; i<r*2; i++) {one_color_all(0,0,255);}
  for(int i=0; i<r*2; i++) {one_color_all(255,255,0);}
  for(int i=0; i<r*2; i++) {one_color_all(0,255,255);}
  for(int i=0; i<r*2; i++) {one_color_all(255,0,255);}
}




//------------------MAIN LOOP------------------
void loop() {
  
//  sCmd.readSerial();     //-PROCESS SERIAL COMMANDS
    digitalWrite(led, HIGH); 


  if (ledMode == 0) {one_color_all(0,0,0);}                 //---STRIP OFF - "0"
  else if (ledMode == 1) {one_color_all(255,255,255);}      //---STRIP SOLID WHITE
  else if (ledMode == 2) {rainbow_fade(20);}                //---STRIP RAINBOW FADE
  else if (ledMode == 3) {rainbow_loop(10, 20);}            //---RAINBOW LOOP
  else if (ledMode == 4) {random_burst(20);}                //---RANDOM
  else if (ledMode == 5) {color_bounce(20);}                //---CYLON v1
  else if (ledMode == 6) {color_bounceFADE(20);}            //---CYLON v2
  else if (ledMode == 7) {police_lightsONE(40);}            //---POLICE SINGLE
  else if (ledMode == 8) {police_lightsALL(40);}            //---POLICE SOLID
  else if (ledMode == 9) {flicker(200,255);}                //---STRIP FLICKER 
  else if (ledMode == 10) {pulse_one_color_all(0, 10);}     //--- PULSE COLOR BRIGHTNESS
  else if (ledMode == 11) {pulse_one_color_all_rev(0, 10);} //--- PULSE COLOR SATURATION   
  else if (ledMode == 12) {fade_vertical(240, 60);}         //--- VERTICAL SOMETHING
//  if (ledMode == 13) {rule30(100);}                       //--- CELL AUTO - RULE 30 (RED)
  else if (ledMode == 14) {random_march(100);}              //--- MARCH RANDOM COLORS
  else if (ledMode == 15) {rwb_march(100);}                 //--- MARCH RWB COLORS
  else if (ledMode == 16) {radiation(120, 60);}             //--- RADIATION SYMBOL (OR SOME APPROXIMATION)
  else if (ledMode == 17) {color_loop_vardelay();}          //--- VARIABLE DELAY LOOP
  else if (ledMode == 18) {white_temps();}                  //--- WHITE TEMPERATURES
  else if (ledMode == 19) {sin_bright_wave(240, 35);}       //--- SIN WAVE BRIGHTNESS
  else if (ledMode == 20) {pop_horizontal(300, 100);}       //--- POP LEFT/RIGHT
  else if (ledMode == 21) {quad_bright_curve(240, 100);}    //--- QUADRATIC BRIGHTNESS CURVE  
  else if (ledMode == 22) {flame();}                        //--- FLAME-ISH EFFECT
  else if (ledMode == 23) {rainbow_vertical(10, 20);}       //--- VERITCAL RAINBOW
  else if (ledMode == 24) {pacman(100);}                    //--- PACMAN
  else if (ledMode == 25) {color_loop_randdelay();}         //--- VARIABLE DELAY LOOP
  else if (ledMode == 26) {swiss_cross(100);}               //--- SWISS CROSS
  else if (ledMode == 27) {germany(100);}                   //--- GERMANY FLAG

  else if (ledMode == 98) {strip_march_ccw(100);}           //--- MARCH WHATEVERS ON THE STRIP NOW CC-W
  else if (ledMode == 99) {strip_march_cw(100);}            //--- MARCH WHATEVERS ON THE STRIP NOW C-W

  else if (ledMode == 101) {one_color_all(255,0,0);}        //---101- STRIP SOLID RED
  else if (ledMode == 102) {one_color_all(0,255,0);}        //---102- STRIP SOLID GREEN
  else if (ledMode == 103) {one_color_all(0,0,255);}        //---103- STRIP SOLID BLUE
  else if (ledMode == 104) {one_color_all(255,255,0);}      //---104- STRIP SOLID YELLOW
  else if (ledMode == 105) {one_color_all(0,255,255);}      //---105- STRIP SOLID TEAL?
  else if (ledMode == 106) {one_color_all(255,0,255);}      //---106- STRIP SOLID VIOLET?
  
  else {demo_mode();}               

}


// GETS CALLED BY SERIALCOMMAND WHEN NO MATCHING COMMAND
void unrecognized(const char *command) {
  Serial.println("nothin fo ya...");
}

