/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v 
GND to arduino GND
Echo to Arduino pin 13 
Trig to Arduino pin 12
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define trigPin 13
#define echoPin 12

#define PIN 6
int lastLightLocation = 0;
int rainbowColour = 0;
int rainbowLocation = 0;
bool rainbowDirection = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  //Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  double duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  int lightLocation = (500-distance)/1.66;
  lightLocation = 300 - lightLocation;
  if (distance >= 500 || distance <= 0)
  {
    //Serial.println("Out of range");
    lastLightLocation++;
    if(lastLightLocation > 300)
    {
      lastLightLocation = 1;
    }
    displayLights(lastLightLocation);
  }
  else 
  {
    //Serial.print(distance);
    //Serial.println(" cm");
    //Serial.println(lightLocation);
    //lastLightLocation = lightLocation;
    if(lightLocation < lastLightLocation)
    {
      displayLights(lastLightLocation - 1);  
      lastLightLocation--;
    }
    else if(lightLocation > lastLightLocation)
    {
      displayLights(lastLightLocation + 1); 
      lastLightLocation++;
    }
    else
    {
      displayLights(lightLocation);
      lastLightLocation = lightLocation;
    }
  }
}

void displayLights(int light) {
    strip.clear();
    strip.setPixelColor(light - 13, strip.Color(10, 10, 10));
    strip.setPixelColor(light - 12, strip.Color(10, 10, 10));
    strip.setPixelColor(light - 11, strip.Color(50, 50, 50));
    strip.setPixelColor(light - 10, strip.Color(50, 50, 50));
    strip.setPixelColor(light - 9, strip.Color(100, 100, 100));
    strip.setPixelColor(light - 8, strip.Color(100, 100, 100));
    strip.setPixelColor(light - 7, strip.Color(150, 150, 150));
    strip.setPixelColor(light - 6, strip.Color(150, 150, 150));
    strip.setPixelColor(light - 5, strip.Color(200, 200, 200));
    strip.setPixelColor(light - 4, strip.Color(200, 200, 200));
    strip.setPixelColor(light - 3, strip.Color(255, 255, 255));
    strip.setPixelColor(light - 2, strip.Color(255, 255, 255));
    strip.setPixelColor(light - 1, strip.Color(255, 255, 255));
    strip.setPixelColor(light, strip.Color(255, 255, 255));
    strip.setPixelColor(light + 1, strip.Color(255, 255, 255));
    strip.setPixelColor(light + 2, strip.Color(255, 255, 255));
    strip.setPixelColor(light + 3, strip.Color(255, 255, 255));
    strip.setPixelColor(light + 4, strip.Color(200, 200, 200));
    strip.setPixelColor(light + 5, strip.Color(200, 200, 200));
    strip.setPixelColor(light + 6, strip.Color(150, 150, 150));
    strip.setPixelColor(light + 7, strip.Color(150, 150, 150));
    strip.setPixelColor(light + 8, strip.Color(100, 100, 100));
    strip.setPixelColor(light + 9, strip.Color(100, 100, 100));
    strip.setPixelColor(light + 10, strip.Color(50, 50, 50));
    strip.setPixelColor(light + 11, strip.Color(50, 50, 50));
    strip.setPixelColor(light + 12, strip.Color(10, 10, 10));
    strip.setPixelColor(light + 13, strip.Color(10, 10, 10));

    if(rainbowColour == 256) {rainbowColour = 1;}
    if(rainbowLocation == 300) {rainbowDirection = 1;}
    if(rainbowLocation == 0) {rainbowDirection = 0;}
    strip.setPixelColor(rainbowLocation, Wheel(rainbowColour));
    if(rainbowDirection) {rainbowLocation--;} else {rainbowLocation++;}
    rainbowColour++;
    strip.show();
    delay(1);
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
