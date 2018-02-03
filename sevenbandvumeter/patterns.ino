/*
 * 63Hz 160Hz 400Hz 1000Hz 2500Hz 6250Hz 16000Hz
 */

byte VUDown(int StartPos, int EndPos, byte Freq, byte MaxFreq, byte ColourSelector)
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


byte BlockPulse(int StartPos, int EndPos, byte Freq, byte ColourSelector)
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
}

void WaterfallDown(int StartPos, int EndPos, byte Freq, byte ColourSelector)
{
  int totalLength = EndPos - StartPos + 1;

  for (int i = 0; i<totalLength-1; i++ )//Shift the current values.
  {
    history[StartPos+i] = history[StartPos+i+1];
  }
  
  history[EndPos] = Freq; //Fill in the new value at the end of each array

  //Go through each Pixel on the strip and set its color
  for (int i=0; i<totalLength; i++)
  {
    switch (ColourSelector) {
      case 0: strip.setPixelColor(StartPos+i, grFadeWheel(history[StartPos+i])); break;
      case 1: strip.setPixelColor(StartPos+i, rbFadeWheel(history[StartPos+i])); break;
      case 2: strip.setPixelColor(StartPos+i, bgFadeWheel(history[StartPos+i])); break;
      case 3: strip.setPixelColor(StartPos+i, gbFadeWheel(history[StartPos+i])); break;
      case 4: strip.setPixelColor(StartPos+i, brFadeWheel(history[StartPos+i])); break;
      case 5: strip.setPixelColor(StartPos+i, rgFadeWheel(history[StartPos+i])); break;
      case 6: strip.setPixelColor(StartPos+i, gpWheel(history[StartPos+i])); break;
      case 7: strip.setPixelColor(StartPos+i, byWheel(history[StartPos+i])); break;
      case 8: strip.setPixelColor(StartPos+i, rcWheel(history[StartPos+i])); break;
      case 9: strip.setPixelColor(StartPos+i, Wheel(history[StartPos+i])); break;
    }
  }
}

void WaterfallUp(int StartPos, int EndPos, byte Freq, byte ColourSelector)
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
    switch (ColourSelector) {
      case 0: strip.setPixelColor(StartPos+i, grFadeWheel(history[StartPos+i])); break;
      case 1: strip.setPixelColor(StartPos+i, rbFadeWheel(history[StartPos+i])); break;
      case 2: strip.setPixelColor(StartPos+i, bgFadeWheel(history[StartPos+i])); break;
      case 3: strip.setPixelColor(StartPos+i, gbFadeWheel(history[StartPos+i])); break;
      case 4: strip.setPixelColor(StartPos+i, brFadeWheel(history[StartPos+i])); break;
      case 5: strip.setPixelColor(StartPos+i, rgFadeWheel(history[StartPos+i])); break;
      case 6: strip.setPixelColor(StartPos+i, gpWheel(history[StartPos+i])); break;
      case 7: strip.setPixelColor(StartPos+i, byWheel(history[StartPos+i])); break;
      case 8: strip.setPixelColor(StartPos+i, rcWheel(history[StartPos+i])); break;
      case 9: strip.setPixelColor(StartPos+i, Wheel(history[StartPos+i])); break;
    }
  }
}

