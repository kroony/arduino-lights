//RGB LED Library
#include <Adafruit_NeoPixel.h>

//Library Setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, 6, NEO_GRB + NEO_KHZ800);

int outputCount = 0;

bool wasHighLast = false;

//initiate dots
DotObject dotArray[20];

void setup()
{
  randomSeed(analogRead(0));
  //Initialize Serial Connection (for debugging)
  Serial.begin(9600);
  Serial.println("Hello");
  delay(250);
  //Initialize Strip
  strip.begin();
  strip.show();

  digitDisplaySetup();
}

void loop()
{
  strip.clear();

  if (digitalRead(10) == HIGH) {//when port 10 pressed, try activate an inactive dot
    if (!wasHighLast) {
      bool activatedSomething = 0;
      for(byte x = 0; x < 20; x++) {
        if(!dotArray[x].active) {
          dotArray[x].activate();
          x = 20;
          activatedSomething = 1;
        }
      }
      if(!activatedSomething) {
        Serial.println("At Max Dots!");
      }
    }
    wasHighLast = true;//check for new press of button not just hold down.
  } else {
    wasHighLast = false;
  }

  //run each dots loop
  int active = 0;
  for(byte x = 0; x < 20; x++) {
    dotArray[x].loop();
    if (dotArray[x].active) active++;
  }
  displayDigit(active);

  //collision detection
  for(byte x = 0; x != 20; ++x) {
    for(byte y = 0; y != x; ++y) {
      if(dotArray[x].active && dotArray[y].active){//dont check unless they are both active
        double loc1_o = dotArray[x].location;
        double loc1_n = loc1_o + dotArray[x].velocity;
        double loc2_o = dotArray[y].location;
        double loc2_n = loc2_o + dotArray[y].velocity;

        if(testOverlap(min(loc1_o, loc1_n), max(loc1_o, loc1_n), min(loc2_o, loc2_n), max(loc2_o, loc2_n))){
          
          if(signbit(dotArray[x].velocity) == signbit(dotArray[y].velocity))
          {//traveling in the same direction
            //Serial.println("same direction collision");
            //swap velocity
            double tmpVelocity = dotArray[x].velocity;
            dotArray[x].velocity = dotArray[y].velocity;
            dotArray[y].velocity = tmpVelocity;

            //change color a little bit
            dotArray[x].color256 = shiftColor(dotArray[x].color256, 2);
            dotArray[y].color256 = shiftColor(dotArray[y].color256, 2);

            //add to collision count
            //dotArray[x].addCollision();
            //dotArray[y].addCollision();
          } 
          else 
          { //traveling towards each other

            //change color a little bit more
            dotArray[x].color256 = shiftColor(dotArray[x].color256, 4);
            dotArray[y].color256 = shiftColor(dotArray[y].color256, 4);

            //add to collision count and turn around if there is no explosion
            if(!dotArray[x].addCollision()) {dotArray[y].velocity = dotArray[y].velocity * -1;} else {dotArray[y].collisions = 0;}
            if(!dotArray[y].addCollision()) {dotArray[x].velocity = dotArray[x].velocity * -1;} else {dotArray[x].collisions = 0;}
            
            //Serial.println("opposing collision");
          }
        }
      }
    }
  }

  //push light array out
  strip.show();
  
  //delay ms
  //delay(35);

  //output debug
  outputCount++;

  //outputDebug(outputCount % 20);
  if(outputCount > 500) {
    outputCount = 0;
    
    for(byte x = 0; x < 20; x++) {
      if(!dotArray[x].active) {
        dotArray[x].activate();
        x = 20;
      }
    }
  }
}

bool overlap(double a, double b, double c) {
 return min(b,c) > a && b < max(b,c);
}

bool testOverlap(int x1, int x2, int y1, int y2) {
  return (x1 >= y1 && x1 <= y2) ||
         (x2 >= y1 && x2 <= y2) ||
         (y1 >= x1 && y1 <= x2) ||
         (y2 >= x1 && y2 <= x2);
}

int shiftColor(byte color, byte shift)
{
  color = color + shift;
  if (color > 255)
  {
    color = color - 255;
  }
  return color;
}

void outputDebug(int x) {
  if (dotArray[x].active) {
    Serial.print("Dot ");
    Serial.print(x);
    Serial.print(" - Active: ");
    Serial.print(dotArray[x].active);
    Serial.print(", Collisions: ");
    Serial.print(dotArray[x].collisions);
    Serial.print(", Location: ");
    Serial.println(dotArray[x].location);
  } else {
    Serial.print("");
  }
}

