/*class Light
{
  public:
  const byte pin;
  bool isOn;

  Light(byte pin_);
  bool getState();//returns if the light is on
  void turnOn();//turns on the light
  void turnOff();//turns off the light
  void switchLight();//switches the light on if off or off if on
}*/

Light::Light(byte pin_) : pin(pin_), isOn(false)
{
  pinMode(pin, OUTPUT);
}

bool Light::getState()
{
  return isOn;
}

void Light::turnOn() //turns on the light
{
  isOn = true;
  digitalWrite(pin, HIGH);
}
void Light::turnOff() //turns off the light
{
  isOn = false;
  digitalWrite(pin, LOW);
}
void Light::switchLight()
{
  if(isOn){
    turnOff();
  } else {
    turnOn();
  }
}
