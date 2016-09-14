/*************************************************************
 * //@filename image.ino
 * //@author Jian Yet Lee
 * //Copyright (c) 2015 WebTeam Corporation. All rights reserved.
 *************************************************************/

void tft_loop() {
  delay(2000);
}

void reset_display() {
  digitalWrite(RESETLINE, HIGH);
  delay(100);
  digitalWrite(RESETLINE, LOW);
}

void mycallback(int errorCode, unsigned char errorByte) {
  const char *Error4DText[] = {"OK\0", "Timeout\0", "NAK\0", "Length\0", "Invalid\0"};
  Serial.print(F("Serial 4D Library reports error ")) ;
  Serial.print(Error4DText[errorCode]);
  if (errorCode == Err4D_NAK)
  {
    Serial.print(F(" returned data= "));
    Serial.println(errorByte);
  }
  else {
    Serial.println(F(""));
  }
}

