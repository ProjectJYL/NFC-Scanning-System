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

/*
uint8_t transfer_img(char * filename) {
  //open a new file for write only
  hndl = Display.file_Open(filename, 'w');
  if (Display.file_Error() != 0)
  {
    Serial.println("File error");
    return Display.file_Error();
  }
  //if the streaming is not finished keep copying ble buffer
  while (nfc_cmd[0] != 0x39 && strlen((char *)nfc_cmd) != 1) {
    //loop  until all bytes are transferred
    if (uart.available())
    {
      //read next byte from buffer
      Display.file_PutC(uart.read(), hndl);
    }
  }
  //wait for cmd 0x39 and then exits the loop
  //release the file handle
  Display.file_Close(hndl);
  return 0x39;
}

void displayReadByte(char * data, word &handle) {
  Serial.print(F("return: ")); Serial.println(Display.file_Read(data , 10, handle));
  if (Display.file_Error() > 0)
  {
    Serial.print(F("File Error after reading: ")); Serial.println(Display.file_Error());
    //    hndl = Display.file_Open("image8.txt", 'r');
  }
}

void mount_sd() {
  disk = Display.file_Mount();
  if (!disk) {
    Display.putstr("Mount uSD card failed\n");
    Display.putstr("Please insert uSD card\n");
    while (!disk) {
      disk = Display.file_Mount();
    }
    Display.gfx_Cls();
    Display.putstr("File mounted\n");
    delay(1000);
  } else {
    Display.putstr("File mounted\n");
    delay(1000);
  }
}
*/
