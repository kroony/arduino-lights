
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

void buttonPressCheck()
{
  int inactiveRed = dotIndexInactive(dotsRed); //get index of inactive red dot
  if (inactiveRed != -1) { //if there is actually an inactive dot, do stuff, otherwise skip anything further
    if (buttonRedDefend.pressed()) {
      dotsRed[inactiveRed].activate(true, false); //activate(bool teamRed_, bool attack_)
    }
    if (buttonRedAttack.pressed()) {
      dotsRed[inactiveRed].activate(true, true); //activate(bool teamRed_, bool attack_)
    }
  }
  
  int inactiveBlue = dotIndexInactive(dotsBlue); //get index of inactive blue dot
  if (inactiveBlue != -1) { //if there is actually an inactive dot, do stuff, otherwise skip anything further
    if (buttonBlueDefend.pressed()) {
      dotsBlue[inactiveBlue].activate(false, false); //activate(bool teamRed_, bool attack_)
    }
    if (buttonBlueAttack.pressed()) {
      dotsBlue[inactiveBlue].activate(false, true); //activate(bool teamRed_, bool attack_)
    }
  }
}

