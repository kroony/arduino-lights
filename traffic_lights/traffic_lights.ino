// Basic 4 Realy board connection
// Each relay is turned on for 2 seconds and then off.
// You can here them click as there state changes from off to on and on to
// off.
// You will also see the corresponding Red LED on the 4 Relay board
// light up when the relay is on.
 
//  define names for the 4 Digital pins On the Arduino   
// These data pins link to 4 Relay board pins IN1, IN2, IN3,IN4

byte RED = 6;                        
byte YELLOW = 7;
byte GREEN = 8;                         

void setup()
{
  Serial.begin(9600);
// Initialise the Arduino data pins for OUTPUT
  pinMode(RED, OUTPUT);       
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  Serial.println("pinmode");  
}

void loop()
{
  digitalWrite(RED,LOW);           // Turns ON Relays 1  
  delay(2000);                                      // Wait 2 seconds
  digitalWrite(RED,HIGH);          // Turns Relay Off 
  delay(2000);
  //digitalWrite(RELAY2,LOW);           // Turns ON Relays 2
  //delay(2000);                                      // Wait 2 seconds
  //digitalWrite(RELAY2,HIGH);          // Turns Relay Off
 
  //digitalWrite(RELAY3,LOW);           // Turns ON Relays 3
  //delay(2000);                                      // Wait 2 seconds
  //digitalWrite(RELAY3,HIGH);          // Turns Relay Off        
}
