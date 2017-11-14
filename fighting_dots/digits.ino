#include <avr/pgmspace.h>
#include <LedControl.h>

LedControl lc=LedControl(12,11,10,1);

void digitDisplaySetup(){
  lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0,8);       // Set the brightness to default value
  lc.clearDisplay(0);         // and clear the display
}

const byte fills [] PROGMEM = {
    B00001110,  //0
    B00010001,
    B00010011,
    B00010101,
    B00011001,
    B00010001,
    B00001110,
    B00000000,
    B00000000,

    B00000100,  //1
    B00001100,
    B00000100,
    B00000100,
    B00000100,
    B00000100,
    B00001110,
    B00000000,
    B00000000,

    B00001110,  //2
    B00010001,
    B00000001,
    B00000010,
    B00000100,
    B00001000,
    B00011111,
    B00000000,
    B00000000,

    B00011111,  //3
    B00000010,
    B00000100,
    B00000010,
    B00000001,
    B00010001,
    B00001110,
    B00000000,
    B00000000,

    B00000010,  //4
    B00000110,
    B00001010,
    B00010010,
    B00011111,
    B00000010,
    B00000010,
    B00000000,
    B00000000,

    B00011111,  //5
    B00010000,
    B00011110,
    B00000001,
    B00000001,
    B00010001,
    B00001110,
    B00000000,
    B00000000,

    B00000110,  //6
    B00001000,
    B00010000,
    B00011110,
    B00010001,
    B00010001,
    B00001110,
    B00000000,
    B00000000,

    B00011111,  //7
    B00010001,
    B00000001,
    B00000010,
    B00000100,
    B00000100,
    B00000100,
    B00000000,
    B00000000,

    B00001110,  //8
    B00010001,
    B00010001,
    B00001110,
    B00010001,
    B00010001,
    B00001110,
    B00000000,
    B00000000,

    B00001110,  //9
    B00010001,
    B00010001,
    B00001111,
    B00000001,
    B00000010,
    B00001100,
    B00000000,
    B00000000,

    B10001110,  //10
    B10010001,
    B10010011,
    B10010101,
    B10011001,
    B10010001,
    B10001110,
    B00000000,
    B00000000,

    B10000100,  //11
    B10001100,
    B10000100,
    B10000100,
    B10000100,
    B10000100,
    B10001110,
    B00000000,
    B00000000,

    B10001110,  //12
    B10010001,
    B10000001,
    B10000010,
    B10000100,
    B10001000,
    B10011111,
    B10000000,
    B00000000,

    B10011111,  //13
    B10000010,
    B10000100,
    B10000010,
    B10000001,
    B10010001,
    B10001110,
    B00000000,
    B00000000,

    B10000010,  //14
    B10000110,
    B10001010,
    B10010010,
    B10011111,
    B10000010,
    B10000010,
    B00000000,
    B00000000,

    B10011111,  //15
    B10010000,
    B10011110,
    B10000001,
    B10000001,
    B10010001,
    B10001110,
    B00000000,
    B00000000,

    B10000110,  //16
    B10001000,
    B10010000,
    B10011110,
    B10010001,
    B10010001,
    B10001110,
    B00000000,
    B00000000,

    B10011111,  //17
    B10010001,
    B10000001,
    B10000010,
    B10000100,
    B10000100,
    B10000100,
    B00000000,
    B00000000,

    B10001110,  //18
    B10010001,
    B10010001,
    B10001110,
    B10010001,
    B10010001,
    B10001110,
    B00000000,
    B00000000,

    B10001110,  //19
    B10010001,
    B10010001,
    B10001111,
    B10000001,
    B10000010,
    B10001100,
    B00000000,
    B00000000

    
};
const int MAX_FILLS = 9 * 20;
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
    //row = row >> 3;
    lc.setRow(0,i, row);
  }
}

void displayDigit(int number)
{
  setNumberDisplay(number);
  updateDigitDisplay();
}

