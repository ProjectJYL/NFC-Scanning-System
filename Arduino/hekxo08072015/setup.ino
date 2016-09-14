/*************************************************************
 * //@filename setup.ino
 * //@author Jian Yet Lee
 * //Copyright (c) 2015 WebTeam Corporation. All rights reserved.
 *************************************************************/

void initializeLED() {
  pinMode(PLAY_CORRECT_LED, OUTPUT);
  pinMode(PLAY_WRONG_LED, OUTPUT);
  pinMode(NFC_READ_LED, OUTPUT);
  digitalWrite(PLAY_CORRECT_LED, LOW);
  digitalWrite(PLAY_WRONG_LED, LOW);
  digitalWrite(NFC_READ_LED, LOW);
}

void ble_setup(void)
{ 
  while(!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Bluefruit demo"));

  uart.setRXcallback(rxCallback);
  uart.setACIcallback(aciCallback);
  
  //check eeprom value for the device name defined by user
  if(getDevName() == 0x00) { 
    //set default bluetooth name
    uart.setDeviceName("HEKXO");
  } else {
    uart.setDeviceName( (char *)dev_name);
  }
  uart.begin(0, 120);
}

void nfc_setup(void) {
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print(F("Didn't find PN53x board"));
    return;
  }

  nfc.SAMConfig();
}

void tft_setup() {
  pinMode(RESETLINE, OUTPUT);
  reset_display();
  delay(5000);
  DisplaySerial.begin(9600);
  Display.gfx_ScreenMode(LANDSCAPE);
  Display.TimeLimit4D = 5000;
  Display.Callback4D = mycallback;
  hndl = Display.media_Init();
  while (!hndl)
  {
    Serial.println(F("Drive not mounted"));
    delay(500);
    Display.gfx_Cls();
    hndl = Display.media_Init();
    Serial.print(F("hndl = ")); Serial.println(hndl);
  }
  Display.gfx_Cls();
  
  //access raw images in uSD
  //set hi and low address
  /*
  Display.media_SetAdd(iUserimages1, iUserimages1);
  for(int i = 0; i < 2; i++)
  {
    word frame = 0;
    do
    {
      Display.media_VideoFrame(0, 0, frame);
      delay(100);
      frame++;
    }while(frame < 8);
  }
  */
  Display.txt_MoveCursor(3, 3);
  Display.putstr("CONNECTING...");
}

void reset() {
  //reset the main board
  digitalWrite(9, LOW);
  delay(2000);
  digitalWrite(9, HIGH);
  //reset display
//  reset_display(); 
//  delay(5000);
}


