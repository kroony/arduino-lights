void fireIR () {
  sendPulse(IRtransmitPin, 4); // Transmit Header pulse, send pulse subroutine deals with the details
  delayMicroseconds(IRpulse);

  for(int i = 0; i < 8; i++) { // Transmit Byte1
    if(byte1[i] == 1){
      sendPulse(IRtransmitPin, 1);
      Serial.print("1 ");
    }
    else{Serial.print("0 ");}
    sendPulse(IRtransmitPin, 1);
    delayMicroseconds(IRpulse);
  }

  for(int i = 0; i < 8; i++) { // Transmit Byte2
    if(byte2[i] == 1){
      sendPulse(IRtransmitPin, 1);
      Serial.print("1 ");
    }
    else{Serial.print("0 ");}
    sendPulse(IRtransmitPin, 1);
    delayMicroseconds(IRpulse);
  }
  
  if(myParity == 1){ // Parity
    sendPulse(IRtransmitPin, 1);
  }
  sendPulse(IRtransmitPin, 1);
  delayMicroseconds(IRpulse);
  Serial.println("success");
}

void sendPulse(int pin, int length){ // importing variables like this allows for secondary fire modes etc.
// This void genertates the carrier frequency for the information to be transmitted
  int i = 0;
  int o = 0;
  while( i < length ){
    i++;
    while( o < IRpulses ){
      o++;
      digitalWrite(pin, HIGH);
      delayMicroseconds(IRt);
      digitalWrite(pin, LOW);
      delayMicroseconds(IRt);
    }
  }
}


void frequencyCalculations() { // Works out all the timings needed to give the correct carrier frequency for the IR signal
  IRt = (int) (500/IRfrequency);  
  IRpulses = (int) (IRpulse / (2*IRt));
  IRt = IRt - 4;
  // Why -4 I hear you cry. It allows for the time taken for commands to be executed.
  // More info: http://j44industries.blogspot.com/2009/09/arduino-frequency-generation.html#more

  Serial.print("Oscilation time period /2: ");
  Serial.println(IRt);
  Serial.println(IRpulses);
  timeOut = IRpulse + 50; // Adding 50 to the expected pulse time gives a little margin for error on the pulse read time out value
}


void tagCode() { // Works out what the players tagger code (the code that is transmitted when they shoot) is
  byte1[0] = currentPitch >> 2 & B1;
  byte1[1] = currentPitch >> 1 & B1;
  byte1[2] = currentPitch >> 0 & B1;

  byte1[3] = currentPattern >> 4 & B1;
  byte1[4] = currentPattern >> 3 & B1;
  byte1[5] = currentPattern >> 2 & B1;
  byte1[6] = currentPattern >> 1 & B1;
  byte1[7] = currentPattern >> 0 & B1;


  byte2[0] = currentPitch >> 2 & B1;
  byte2[1] = currentPitch >> 1 & B1;
  byte2[2] = currentPitch >> 0 & B1;

  byte2[3] = currentColourWheel >> 4 & B1;
  byte2[4] = currentColourWheel >> 3 & B1;
  byte2[5] = currentColourWheel >> 2 & B1;
  byte2[6] = currentColourWheel >> 1 & B1;
  byte2[7] = currentColourWheel >> 0 & B1;

  myParity = 0;
  for (int i=0; i<8; i++) {
   if(byte1[i] == 1){myParity = myParity + 1;}
   if(byte2[i] == 1){myParity = myParity + 1;}
   myParity = myParity >> 0 & B1;
  }

  // Next few lines print the full tagger code.
  Serial.print("Byte1: ");
  Serial.print(byte1[0]);
  Serial.print(byte1[1]);
  Serial.print(byte1[2]);
  Serial.print(byte1[3]);
  Serial.print(byte1[4]);
  Serial.print(byte1[5]);
  Serial.print(byte1[6]);
  Serial.print(byte1[7]);
  Serial.println();

  Serial.print("Byte2: ");
  Serial.print(byte2[0]);
  Serial.print(byte2[1]);
  Serial.print(byte2[2]);
  Serial.print(byte2[3]);
  Serial.print(byte2[4]);
  Serial.print(byte2[5]);
  Serial.print(byte2[6]);
  Serial.print(byte2[7]);
  Serial.println();

  Serial.print("Parity: ");
  Serial.print(myParity);
  Serial.println();
}

