/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/
// defines pins numbers

#include <Adafruit_NeoPixel.h>

//Set Strip Constants
const int length = 60;
const byte brightness = 255;

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(length, 6, NEO_GRB + NEO_KHZ800);


const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;
void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication

//Initialize Strip
  strip.begin();
  strip.setBrightness(brightness);
  strip.show(); 

  
}
void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);

if(distance < 30) {
  setColor(strip.Color(255, 0, 0)); // Red
  //red wipe
  } else if(distance < 60) {
  setColor(strip.Color(0, 0, 255)); // Blue
} else {
  setColor(strip.Color(0, 0, 0)); // Blue
}

}



void setColor(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
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

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t WheelBrightness(byte WheelPos, byte brightness)
{
  if (WheelPos < 85) {
   return Color(brightness*(WheelPos * 3)/255, 
                brightness*(255 - WheelPos * 3)/255, 
                0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(brightness*(255 - WheelPos * 3)/255, 
                0, 
                brightness*(WheelPos * 3)/255);
  } else {
   WheelPos -= 170; 
   return Color(0, 
                brightness*(WheelPos * 3)/255, 
                brightness*(255 - WheelPos * 3)/255);
  }
}
