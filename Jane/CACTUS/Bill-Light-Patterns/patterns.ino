/*byte VUDown(int StartPos, int EndPos, byte Freq, byte MaxFreq, byte ColourSelector)
{
  int totalLength = EndPos - StartPos + 1;

  if(MaxFreq < Freq){
    MaxFreq = Freq;
    peakColour++;
  }
  else MaxFreq = max(MaxFreq - 1, 0);

  int peak = map(Freq, 0, 255, 0, totalLength);
  for(int i=0; i<totalLength; i++)
  {
    if(i < peak)
    {
      switch (ColourSelector) {
        case 0: strip.setPixelColor(EndPos - i, grWheel(map(i, 0, totalLength, 0, 255))); break;
        case 1: strip.setPixelColor(EndPos - i, rbWheel(map(i, 0, totalLength, 0, 255))); break;
        case 2: strip.setPixelColor(EndPos - i, bgWheel(map(i, 0, totalLength, 0, 255))); break;
        case 3: strip.setPixelColor(EndPos - i, gbWheel(map(i, 0, totalLength, 0, 255))); break;
        case 4: strip.setPixelColor(EndPos - i, brWheel(map(i, 0, totalLength, 0, 255))); break;
        case 5: strip.setPixelColor(EndPos - i, rgWheel(map(i, 0, totalLength, 0, 255))); break;
        case 6: strip.setPixelColor(EndPos - i, gpWheel(map(i, 0, totalLength, 0, 255))); break;
        case 7: strip.setPixelColor(EndPos - i, byWheel(map(i, 0, totalLength, 0, 255))); break;
        case 8: strip.setPixelColor(EndPos - i, rcWheel(map(i, 0, totalLength, 0, 255))); break;
        case 9: strip.setPixelColor(EndPos - i, Wheel(map(i, 0, totalLength, 0, 255))); break;
      }
    }
    else
    {
      strip.setPixelColor(EndPos - i, Color(0,0,0));
    }
  }
  //peak tracker
  strip.setPixelColor(EndPos - map(MaxFreq, 0, 255, 0, totalLength), rbWheel(peakColour));

  return MaxFreq;
}

byte VUUp(int StartPos, int EndPos, byte Freq, byte MaxFreq, byte ColourSelector)
{
  int totalLength = EndPos - StartPos + 1;

  if(MaxFreq < Freq){
    MaxFreq = Freq;
    peakColour++;
  }
  else MaxFreq = max(MaxFreq - 1, 0);

  int peak = map(Freq, 0, 255, 0, totalLength);
  for(int i=0; i<totalLength; i++)
  {
    if(i < peak)
    {
      switch (ColourSelector) {
        case 0: strip.setPixelColor(StartPos + i, grWheel(map(i, 0, totalLength, 0, 255))); break;
        case 1: strip.setPixelColor(StartPos + i, rbWheel(map(i, 0, totalLength, 0, 255))); break;
        case 2: strip.setPixelColor(StartPos + i, bgWheel(map(i, 0, totalLength, 0, 255))); break;
        case 3: strip.setPixelColor(StartPos + i, gbWheel(map(i, 0, totalLength, 0, 255))); break;
        case 4: strip.setPixelColor(StartPos + i, brWheel(map(i, 0, totalLength, 0, 255))); break;
        case 5: strip.setPixelColor(StartPos + i, rgWheel(map(i, 0, totalLength, 0, 255))); break;
        case 6: strip.setPixelColor(StartPos + i, gpWheel(map(i, 0, totalLength, 0, 255))); break;
        case 7: strip.setPixelColor(StartPos + i, byWheel(map(i, 0, totalLength, 0, 255))); break;
        case 8: strip.setPixelColor(StartPos + i, rcWheel(map(i, 0, totalLength, 0, 255))); break;
        case 9: strip.setPixelColor(StartPos + i, Wheel(map(i, 0, totalLength, 0, 255))); break;
      }
    }
    else
    {
      strip.setPixelColor(StartPos + i, Color(0,0,0));
    }
  }
  //peak tracker
  strip.setPixelColor(StartPos + map(MaxFreq, 0, 255, 0, totalLength), rbWheel(peakColour));

  return MaxFreq;
}


void BlockPulse(int StartPos, int EndPos, byte Freq, byte ColourSelector)
{
  int totalLength = EndPos - StartPos + 1;

  for(int i=0; i<totalLength; i++)
  {
    switch (ColourSelector) {
      case 0: strip.setPixelColor(StartPos + i, grFadeWheel(Freq)); break;
      case 1: strip.setPixelColor(StartPos + i, rbFadeWheel(Freq)); break;
      case 2: strip.setPixelColor(StartPos + i, bgFadeWheel(Freq)); break;
      case 3: strip.setPixelColor(StartPos + i, gbFadeWheel(Freq)); break;
      case 4: strip.setPixelColor(StartPos + i, brFadeWheel(Freq)); break;
      case 5: strip.setPixelColor(StartPos + i, rgFadeWheel(Freq)); break;
      case 6: strip.setPixelColor(StartPos + i, gpWheel(Freq)); break;
      case 7: strip.setPixelColor(StartPos + i, byWheel(Freq)); break;
      case 8: strip.setPixelColor(StartPos + i, rcWheel(Freq)); break;
      case 9: strip.setPixelColor(StartPos + i, Wheel(Freq)); break;
    }
  }
}*/

void WaterfallDown(int StartPos, int EndPos, byte Freq)
{
  int totalLength = EndPos - StartPos + 1;

  for (int i = 0; i<totalLength-1 ; i++ )//Shift the current values.
  {
    history[StartPos+i] = history[StartPos+i+1];//max to remove compiler nag
    //history[StartPos+i] = history[299];//max to remove compiler nag
  }
  
  history[EndPos] = Freq; //Fill in the new value at the end of each array

  //Go through each Pixel on the strip and set its color
  for (int i=0; i<totalLength; i++)
  {
    strip.setPixelColor(StartPos+i, Wheel(history[StartPos+i]));
    
  }
}

void WaterfallUp(int StartPos, int EndPos, byte Freq)
{
  int totalLength = EndPos - StartPos + 1;

  for (int i = 0; i<totalLength-1; i++ )//Shift the current values.
  {
    history[EndPos-i] = history[EndPos-i-1];
  }
  
  history[StartPos] = Freq; //Fill in the new value at the end of each array

  //Go through each Pixel on the strip and set its color
  for (int i=0; i<stripLength; i++)
  {
    strip.setPixelColor(StartPos+i, Wheel(history[StartPos+i]));
  }
}

void WaterfallRandom(int StartPos, int EndPos, byte Freq)
{
  int totalLength = EndPos - StartPos + 1;
  strip.setPixelColor(random(StartPos, EndPos +1), Wheel(Freq));
}

void AlternateColors(int StartPos, int EndPos, uint32_t Color1, uint32_t Color2)
{
  int totalLength = EndPos - StartPos + 1;

  for (int i=0; i<totalLength; i++)
  {
    if(strobeCurrent) { strip.setPixelColor(StartPos+i, Color1); }
    else { strip.setPixelColor(StartPos+i, Color2); }
  }
}

void ChaseColors(int StartPos, int EndPos, uint32_t Color1, uint32_t Color2)
{
  int totalLength = EndPos - StartPos + 1;

  for(int i = 0;i<totalLength;i++)
  {
    strip.setPixelColor(i, Color1);
  }
  
  for (int i=strobeCurrent; i<totalLength; i=i+2)
  {
    strip.setPixelColor(StartPos+i, Color2);
  }
}

void ChaseColorGapDown(int StartPos, int EndPos, uint32_t Color1)
{
  int totalLength = EndPos - StartPos + 1;
  SingleColorChaseCounter;
  byte colorCounter = SingleColorChaseCounter;
  for(int i = 0;i<totalLength;i++)
  {
    colorCounter++;
    if(colorCounter == 5) { colorCounter = 0; }
    if(colorCounter == 0) {strip.setPixelColor(i, Color1); }
  }
  SingleColorChaseCounter++;
  if(SingleColorChaseCounter == 5) SingleColorChaseCounter = 0;
}

void ChaseColorGapUp(int StartPos, int EndPos, uint32_t Color1)
{
  int totalLength = EndPos - StartPos + 1;
  SingleColorChaseCounter;
  byte colorCounter = SingleColorChaseCounter;
  for(int i = totalLength;i>0;i--)
  {
    colorCounter++;
    if(colorCounter == 5) { colorCounter = 0; }
    if(colorCounter == 0) {strip.setPixelColor(i, Color1); }
  }
  SingleColorChaseCounter++;
  if(SingleColorChaseCounter == 5) SingleColorChaseCounter = 0;
}

void ChaseColorGapDown2(int StartPos, int EndPos, uint32_t Color1, uint32_t Color2)
{
  int totalLength = EndPos - StartPos + 1;
  byte colorCounter = DoubleColorChaseCounter;
  for(int i = 0;i<totalLength;i++)
  {
    colorCounter++;
    if(colorCounter == 6) { colorCounter = 0; }
    if(colorCounter == 0) {strip.setPixelColor(i, Color1); }
    if(colorCounter == 1) {strip.setPixelColor(i, Color2); }
  }
  DoubleColorChaseCounter++;
  if(DoubleColorChaseCounter == 6) DoubleColorChaseCounter = 0;
}

void ChaseColorGapUp2(int StartPos, int EndPos, uint32_t Color1, uint32_t Color2)
{
  int totalLength = EndPos - StartPos + 1;
  byte colorCounter = DoubleColorChaseCounter;
  for(int i = totalLength;i>0;i--)
  {
    colorCounter++;
    if(colorCounter == 6) { colorCounter = 0; }
    if(colorCounter == 0) {strip.setPixelColor(i, Color1); }
    if(colorCounter == 1) {strip.setPixelColor(i, Color2); }
  }
  DoubleColorChaseCounter++;
  if(DoubleColorChaseCounter == 6) DoubleColorChaseCounter = 0;
}

void ChaseColorGapDown3(int StartPos, int EndPos, uint32_t Color1, uint32_t Color2, uint32_t Color3)
{
  int totalLength = EndPos - StartPos + 1;
  byte colorCounter = TrippleColorChaseCounter;
  for(int i = 0;i<totalLength;i++)
  {
    colorCounter++;
    if(colorCounter == 7) { colorCounter = 0; }
    if(colorCounter == 0) {strip.setPixelColor(i, Color1); }
    if(colorCounter == 1) {strip.setPixelColor(i, Color2); }
    if(colorCounter == 2) {strip.setPixelColor(i, Color3); }
  }
  TrippleColorChaseCounter++;
  if(TrippleColorChaseCounter == 7) TrippleColorChaseCounter = 0;
}

void ChaseColorGapUp3(int StartPos, int EndPos, uint32_t Color1, uint32_t Color2, uint32_t Color3)
{
  int totalLength = EndPos - StartPos + 1;
  byte colorCounter = TrippleColorChaseCounter;
  for(int i = totalLength;i>0;i--)
  {
    colorCounter++;
    if(colorCounter == 7) { colorCounter = 0; }
    if(colorCounter == 0) {strip.setPixelColor(i, Color1); }
    if(colorCounter == 1) {strip.setPixelColor(i, Color2); }
    if(colorCounter == 2) {strip.setPixelColor(i, Color3); }
  }
  TrippleColorChaseCounter++;
  if(TrippleColorChaseCounter == 7) TrippleColorChaseCounter = 0;
}

void colorWipe(uint32_t Color1) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Color1);
    strip.show();
    delay(frameDelay);
  }
}

void ClearStrip(){
  for(int i = 0;i<stripLength;i++)
  {
    strip.setPixelColor(i, Color(0,0,0));
  }
}
