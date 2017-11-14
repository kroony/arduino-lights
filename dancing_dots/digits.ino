#include <avr/pgmspace.h>
#include <LedControl.h>

LedControl lc=LedControl(12,11,10,1);

void digitDisplaySetup(){
  lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0,8);       // Set the brightness to default value
  lc.clearDisplay(0);         // and clear the display
}

const byte fills [] PROGMEM = {
    B01110000,  //0
    B10001000,
    B10011000,
    B10101000,
    B11001000,
    B10001000,
    B01110000,
    B00000000,
    B00000000,

    B00100000,  //1
    B01100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B01110000,
    B00000000,
    B00000000,

    B01110000,  //2
    B10001000,
    B00001000,
    B00010000,
    B00100000,
    B01000000,
    B11111000,
    B00000000,
    B00000000,

    B11111000,  //3
    B00010000,
    B00100000,
    B00010000,
    B00001000,
    B10001000,
    B01110000,
    B00000000,
    B00000000,

    B00010000,  //4
    B00110000,
    B01010000,
    B10010000,
    B11111000,
    B00010000,
    B00010000,
    B00000000,
    B00000000,

    B11111000,  //5
    B10000000,
    B11110000,
    B00001000,
    B00001000,
    B10001000,
    B01110000,
    B00000000,
    B00000000,

    B00110000,  //6
    B01000000,
    B10000000,
    B11110000,
    B10001000,
    B10001000,
    B01110000,
    B00000000,
    B00000000,

    B11111000,  //7
    B10001000,
    B00001000,
    B00010000,
    B00100000,
    B00100000,
    B00100000,
    B00000000,
    B00000000,

    B01110000,  //8
    B10001000,
    B10001000,
    B01110000,
    B10001000,
    B10001000,
    B01110000,
    B00000000,
    B00000000,

    B01110000,  //9
    B10001000,
    B10001000,
    B01111000,
    B00001000,
    B00010000,
    B01100000,
    B00000000,
    B00000000
};
const int MAX_FILLS = 9 * 10;
const int FILL_ROWS_PER_NUMBER = 9;

int digitDisplay = 0;
int digitDisplayGoal = 100;


void setNumberDisplay(int i)
{
  digitDisplayGoal = FILL_ROWS_PER_NUMBER * i;
}


void updateDigitDisplay()
{
  if (digitDisplay > digitDisplayGoal) {
    digitDisplay--;
  } else if (digitDisplay < digitDisplayGoal) {
    digitDisplay++;
  }

  for (int i = 0; i != 8; ++i) {
    byte row = pgm_read_byte_near(fills + (digitDisplay + i) % MAX_FILLS);
    // add padding on left
    row = row >> 2;
    lc.setRow(0,i, row);
  }
}

void displayDigit(int number)
{
  setNumberDisplay(number);
  updateDigitDisplay();
}

