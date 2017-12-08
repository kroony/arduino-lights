class Button
{
public:
  const byte pin;
  bool last;

  Button(byte pin_) : pin(pin_), last(false)
  { }

  bool pressed()
  {
    if (digitalRead(pin) == HIGH)
    {
      if (last) return false;
      last = true;
      return true;
    } else {
      last = false;
      return false;
    }
  }
};


Button buttonBlueDefend(1), buttonBlueAttack(2),
       buttonRedDefend(3), buttonRedAttack(4);
