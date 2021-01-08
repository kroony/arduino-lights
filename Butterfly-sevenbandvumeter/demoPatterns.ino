void Flap(byte wait)
{
  byte RandomWheel = random(0, 10);
  uint32_t WheelColour = 0;
  byte j = 0;
  
  ClearStrip();
  for(byte i=0; i<255; i++){

    switch (RandomWheel) {
      case 0: WheelColour = grWheel(i); break;
      case 1: WheelColour = rbWheel(i); break;
      case 2: WheelColour = bgWheel(i); break;
      case 3: WheelColour = gbWheel(i); break;
      case 4: WheelColour = brWheel(i); break;
      case 5: WheelColour = rgWheel(i); break;
      case 6: WheelColour = gpWheel(i); break;
      case 7: WheelColour = byWheel(i); break;
      case 8: WheelColour = rcWheel(i); break;
      case 9: WheelColour = Wheel(i); break;
    }  
    if(j == 5) { j = 0;} //reset j
    if(j==0) {
      BlockColour(1,  28, WheelColour); //right outer
      BlockColour(39, 64, WheelColour); //left outer
  
      BlockColour(65, 74, Color(0,0,0)); //left middle
      BlockColour(29, 38, Color(0,0,0)); //right middle
  
      BlockColour(75, 82, Color(0,0,0)); //right inner
      BlockColour(83, 89, Color(0,0,0)); //left inner
    } else if(j==1 || j==4) {
      BlockColour(1,  28, Color(0,0,0)); //right outer
      BlockColour(39, 64, Color(0,0,0)); //left outer
  
      BlockColour(65, 74, WheelColour); //left middle
      BlockColour(29, 38, WheelColour); //right middle
  
      BlockColour(75, 82, Color(0,0,0)); //right inner
      BlockColour(83, 89, Color(0,0,0)); //left inner
    } else if(j==2 || j==3) {
      BlockColour(1,  28, Color(0,0,0)); //right outer
      BlockColour(39, 64, Color(0,0,0)); //left outer
  
      BlockColour(65, 74, Color(0,0,0)); //left middle
      BlockColour(29, 38, Color(0,0,0)); //right middle
  
      BlockColour(75, 82, WheelColour); //right inner
      BlockColour(83, 89, WheelColour); //left inner
    }
    j++;
    strip.show();
    delay(wait);
    ClearStrip();
  }
}

void theaterChase(int wait) {
  byte RandomWheel = random(0, 10);
  uint32_t WheelColour = 0;
  
  for(int a=0; a<255; a++) {  // Repeat 255 times...

    switch (RandomWheel) {
      case 0: WheelColour = grWheel(a); break;
      case 1: WheelColour = rbWheel(a); break;
      case 2: WheelColour = bgWheel(a); break;
      case 3: WheelColour = gbWheel(a); break;
      case 4: WheelColour = brWheel(a); break;
      case 5: WheelColour = rgWheel(a); break;
      case 6: WheelColour = gpWheel(a); break;
      case 7: WheelColour = byWheel(a); break;
      case 8: WheelColour = rcWheel(a); break;
      case 9: WheelColour = Wheel(a); break;
    }
    
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, WheelColour); // Set pixel 'c' to value 'color'
      }
      if(b == 0) {
        BlockColour(75, 82, WheelColour); //right inner
        BlockColour(83, 89, WheelColour); //left inner
      }else{
        BlockColour(75, 82, WheelColour/2); //right inner
        BlockColour(83, 89, WheelColour/2); //left inner
      }
      
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void demoPaint(byte wait) {
  //void TwinklePaint(int StartPos, int EndPos, byte Freq, byte ColourSelector)
  byte randWheel = random(0,10);
  byte randFreq = 0;
  for(byte i=0; i<255; i++){
    randFreq = random(0, 255);
    Twinkle(1 , 28, randFreq, randWheel); //right outer
    Twinkle(39, 64, randFreq, randWheel); //left outer

    Twinkle(65, 74, randFreq, randWheel); //left middle
    Twinkle(29, 38, randFreq, randWheel); //right middle

    TwinklePaint(75, 82, randFreq, randWheel); //right inner
    TwinklePaint(83, 89, randFreq, randWheel); //left inner
    strip.show();
    delay(wait);
    ClearStrip();
  }
}

void demoWaterfallDot(byte wait) {
  byte wheel1 = 0;
  byte wheel2 = 0;
  byte wheel3 = 0;

  bool fallDirection = random(0, 2);

  byte randWheelSelector = random(0, 4);
  
  switch (randWheelSelector) {
    case 0: wheel1 = 0; wheel2 = 1; wheel3 = 2; break;
    case 1: wheel1 = 2; wheel2 = 1; wheel3 = 0; break;
    case 2: wheel1 = 3; wheel2 = 4; wheel3 = 5; break;
    case 3: wheel1 = 5; wheel2 = 4; wheel3 = 3; break;
  }
  
  byte randFreq = 0;
  for(byte i=0; i<255; i++){
    randFreq = random(0, 255);
   
    if(randFreq < 255-i) { randFreq = 0;}

    if(fallDirection) {
      WaterfallDown(1, 28, randFreq, wheel1); //right outer
      WaterfallDown(39, 64, randFreq, wheel1); //left outer
  
      WaterfallUp(65, 74, randFreq, wheel2); //left middle
      WaterfallUp(29, 38, randFreq, wheel2); //right middle
  
      WaterfallDown(75, 82, randFreq, wheel3); //right inner
      WaterfallUp(83, 89, randFreq, wheel3); //left inner
    } else {
      WaterfallUp(1, 28, randFreq, wheel1); //right outer
      WaterfallUp(39, 64, randFreq, wheel1); //left outer
  
      WaterfallDown(65, 74, randFreq, wheel2); //left middle
      WaterfallDown(29, 38, randFreq, wheel2); //right middle
  
      WaterfallUp(75, 82, randFreq, wheel3); //right inner
      WaterfallDown(83, 89, randFreq, wheel3); //left inner
    }
    strip.show();
    delay(wait);
    ClearStrip();
  }

//clear the strip
  for(byte i=0; i<30; i++){
    if(fallDirection) {
      WaterfallDown(1, 28, 0, wheel1); //right outer
      WaterfallDown(39, 64, 0, wheel1); //left outer
  
      WaterfallUp(65, 74, 0, wheel2); //left middle
      WaterfallUp(29, 38, 0, wheel2); //right middle
  
      WaterfallDown(75, 82, 0, wheel3); //right inner
      WaterfallUp(83, 89, 0, wheel3); //left inner
    } else {
      WaterfallUp(1, 28, 0, wheel1); //right outer
      WaterfallUp(39, 64, 0, wheel1); //left outer
  
      WaterfallDown(65, 74, 0, wheel2); //left middle
      WaterfallDown(29, 38, 0, wheel2); //right middle
  
      WaterfallUp(75, 82, 0, wheel3); //right inner
      WaterfallDown(83, 89, 0, wheel3); //left inner
    }
    strip.show();
    delay(wait);
    ClearStrip();
  }
}
