
Button::Button(byte pin_) : pin(pin_), last(true)
{
  pinMode(pin, INPUT_PULLUP);
}

bool Button::pressed()
{
  if (digitalRead(pin) == LOW)
  {
    if (last) return false;
    last = true;
    return true;
  } else {
    last = false;
    return false;
  }
}

