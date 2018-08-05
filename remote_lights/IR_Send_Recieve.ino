void receiveIR() { // Void checks for an incoming signal and decodes it if it sees one.
  int error = 0;
 
  if(digitalRead(IRreceivePin) == LOW){    // If the receive pin is low a signal is being received.
    if(digitalRead(IRreceive2Pin) == LOW){ // Is the incoming signal being received by the head sensors?
      received[0] = 1;
    }
    else{
      received[0] = 0;
    }
   
    while(digitalRead(IRreceivePin) == LOW){
    }
    for(int i = 1; i <= 17; i++) {                        // Repeats several times to make sure the whole signal has been received
      //received[i] = pulseIn(IRreceivePin, LOW, timeOut);  // pulseIn command waits for a pulse and then records its duration in microseconds.
      received[i] = pulseIn(IRreceivePin, LOW);  // pulseIn command waits for a pulse and then records its duration in microseconds.
    } 
   
    Serial.print("sensor: ");                            // Prints if it was a head shot or not.
    Serial.print(received[0]); 
    Serial.print("...");
   
    for(int i = 1; i <= 17; i++) {  // Looks at each one of the received pulses
      int receivedTemp[18];
      receivedTemp[i] = 2;
      if(received[i] > (IRpulse - 200) &&  received[i] < (IRpulse + 200)) {receivedTemp[i] = 0;}                      // Works out from the pulse length if it was a data 1 or 0 that was received writes result to receivedTemp string
      if(received[i] > (IRpulse + IRpulse - 200) &&  received[i] < (IRpulse + IRpulse + 200)) {receivedTemp[i] = 1;}  // Works out from the pulse length if it was a data 1 or 0 that was received  
      received[i] = 3;                   // Wipes raw received data
      received[i] = receivedTemp[i];     // Inputs interpreted data
     
      Serial.print(" ");
      Serial.print(received[i]);         // Print interpreted data results
    }
    Serial.println("");                  // New line to tidy up printed results
   
    // Parity Check. Was the data received a valid signal?
    check = 0;
    for(int i = 1; i <= 16; i++) {
      if(received[i] == 1){check = check + 1;}
      if(received[i] == 2){error = 1;}
    }
    check = check >> 0 & B1;
    if(check != received[17]){error = 2;}
    if(error == 0){Serial.println("Valid Signal");}
    else{
      Serial.println("ERROR");
      if(error == 1){
        Serial.println("Parity");
      }
      else if (error == 2){
        Serial.println("Check");
      }
      for(int i = 0;i<stripLength;i++)
      {
        strip.setPixelColor(i, Color(255,0,0));
      }
      strip.show();
      delay(500);
    }
    if(error == 0){interpritReceived();}
    
  }
}


void interpritReceived(){  // After a message has been received by the ReceiveIR subroutine this subroutine decidedes how it should react to the data
  for(int i = 0;i<stripLength;i++)
  {
    strip.setPixelColor(i, Color(0,255,0));
  }
  strip.show();
  delay(500);

  
  // Next few lines Effectivly converts the binary data into decimal
  // Im sure there must be a much more efficient way of doing this
  currentPitch = 0;
  currentPattern = 0;
  currentColourWheel = 0;
  
  if(received[1] == 1){currentPitch = currentPitch + 4;}
  if(received[2] == 1){currentPitch = currentPitch + 2;}
  if(received[3] == 1){currentPitch = currentPitch + 1;} 

  if(received[4] == 1){currentPattern = currentPattern + 16;}
  if(received[5] == 1){currentPattern = currentPattern + 8;}
  if(received[6] == 1){currentPattern = currentPattern + 4;}
  if(received[7] == 1){currentPattern = currentPattern + 2;}
  if(received[8] == 1){currentPattern = currentPattern + 1;}
   
  /*if(received[9]  == 1){weapon[hitNo] = weapon[hitNo] + 4;}
  if(received[10] == 1){weapon[hitNo] = weapon[hitNo] + 2;}
  if(received[11] == 1){weapon[hitNo] = weapon[hitNo] + 1;} */

  if(received[12] == 1){currentColourWheel = currentColourWheel + 16;}
  if(received[13] == 1){currentColourWheel = currentColourWheel + 8;}
  if(received[14] == 1){currentColourWheel = currentColourWheel + 4;}
  if(received[15] == 1){currentColourWheel = currentColourWheel + 2;}
  if(received[16] == 1){currentColourWheel = currentColourWheel + 1;}
   
  //parity[hitNo] = received[17];

  if(currentPattern > 6) {currentPattern = 6;}
  if(currentColourWheel > 9) {currentColourWheel = 9;}

  Serial.print("currentPitch: ");
  Serial.print(currentPitch);
  Serial.print("  currentPattern: ");
  Serial.print(currentPattern);
  Serial.print("  currentColourWheel: ");
  Serial.println(currentColourWheel);
 
 
  /*if (team[hitNo] != myTeamID && player[hitNo] != myPlayerID) { // Make sure i have not hit myself
    
    ////This is probably where more code should be added to expand the game capabilities at the moment the code just checks that the received data was not a system message and deducts a life if it wasn't.
    if (player[hitNo] != 0){hit();}
    hitNo++ ;
    
  }*/
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

