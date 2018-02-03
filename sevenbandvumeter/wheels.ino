uint32_t Wheel(byte WheelPos)//full wheel from green -> red -> blue
{
  /*if (WheelPos == 0) {
    return Color(0, 0, 0);
  }
  else */if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

uint32_t grWheel(byte WheelPos)//green->red
{
  if(WheelPos < 127) { return Color(WheelPos, 255, 0); }
  else               { return Color(255, 255 - WheelPos, 0); }
}
uint32_t grFadeWheel(byte WheelPos)//off->green->red
{
  if(WheelPos < 85)        { return Color(0, WheelPos * 3, 0); }
  else if (WheelPos < 170) { return Color((WheelPos - 170) * 3, 255, 0); }
  else                     { return Color(255, 255 - ((WheelPos - 170) * 3), 0); }
}

uint32_t gbWheel(byte WheelPos)//green->blue
{
  if(WheelPos < 127) { return Color(0, 255, WheelPos); }
  else               { return Color(0, 255 - WheelPos, 255); }
}
uint32_t gbFadeWheel(byte WheelPos)//off->green->blue
{
  if(WheelPos < 85)        { return Color(0, WheelPos * 3, 0); }
  else if (WheelPos < 170) { return Color(0, 255, (WheelPos - 170) * 3); }
  else                     { return Color(0, 255 - ((WheelPos - 170) * 3), 255); }
}

uint32_t rbWheel(byte WheelPos)//red->blue
{
  if(WheelPos < 127) { return Color(255, 0, WheelPos); }
  else               { return Color(255 - WheelPos, 0, 255); }
}
uint32_t rbFadeWheel(byte WheelPos)//off->red->blue
{
  if(WheelPos < 85)        { return Color(WheelPos * 3, 0, 0); }
  else if (WheelPos < 170) { return Color(255, 0, (WheelPos - 170) * 3); }
  else                     { return Color(255 - ((WheelPos - 170) * 3), 0, 255); }
}

uint32_t bgWheel(byte WheelPos)//blue->green
{
  if(WheelPos < 127) { return Color(0, WheelPos, 255); }
  else               { return Color(0, 255, 255 - WheelPos); }
}
uint32_t bgFadeWheel(byte WheelPos)//off->blue->green
{
  if(WheelPos < 85)        { return Color(0, 0, WheelPos * 3); }
  else if (WheelPos < 170) { return Color(0, (WheelPos - 170) * 3, 255); }
  else                     { return Color(0, 255, 255 - ((WheelPos - 170) * 3)); }
}

uint32_t brWheel(byte WheelPos)//blue->red
{
  if(WheelPos < 127) { return Color(WheelPos, 0, 255); }
  else               { return Color(255, 0, 255 - WheelPos); }
}
uint32_t brFadeWheel(byte WheelPos)//off->blue->red
{
  if(WheelPos < 85)        { return Color(0, 0, WheelPos * 3); }
  else if (WheelPos < 170) { return Color((WheelPos - 170) * 3, 0, 255); }
  else                     { return Color(255, 0, 255 - ((WheelPos - 170) * 3)); }
}

uint32_t rgWheel(byte WheelPos) //red->Green
{
  if(WheelPos < 127) { return Color(255, WheelPos, 0); }
  else               { return Color(255 - WheelPos, 255, 0); }
}
uint32_t rgFadeWheel(byte WheelPos)//off->red->green
{
  if(WheelPos < 85)        { return Color(WheelPos * 3, 0, 0); }
  else if (WheelPos < 170) { return Color(255, (WheelPos - 170) * 3, 0); }
  else                     { return Color(255 - ((WheelPos - 170) * 3), 255, 0); }
}


uint32_t gpWheel(byte WheelPos)
{
  if(WheelPos < 127) { return Color(WheelPos, 255, WheelPos); }
  else               { return Color(255, 255 - WheelPos, 255); }
}

uint32_t byWheel(byte WheelPos)
{
  if(WheelPos < 127) { return Color(WheelPos, WheelPos, 255); }
  else { return Color(255, 255, 255 - WheelPos); }
}

uint32_t rcWheel(byte WheelPos)
{
  if(WheelPos < 127) { return Color(255, WheelPos, WheelPos); }
  else { return Color(255 - WheelPos, 255, 255); }
}

//From off to Single Colour
uint32_t rWheel(byte WheelPos) { return Color(WheelPos, 0, 0); } //off -> red
uint32_t gWheel(byte WheelPos) { return Color(0, WheelPos, 0); } //off -> green
uint32_t bWheel(byte WheelPos) { return Color(0, 0, WheelPos); } //off -> blue
uint32_t cWheel(byte WheelPos) { return Color(0, WheelPos, WheelPos); } //off -> Cyan
uint32_t pWheel(byte WheelPos) { return Color(WheelPos, 0, WheelPos); } //off -> Purple
uint32_t yWheel(byte WheelPos) { return Color(WheelPos, WheelPos, 0); } //off -> Yellow
uint32_t wWheel(byte WheelPos) { return Color(WheelPos, WheelPos, WheelPos); } //off -> White




