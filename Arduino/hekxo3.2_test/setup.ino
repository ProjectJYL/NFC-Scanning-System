
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
  uart.begin(0, 50);
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
  disk = Display.file_Mount();
  while (disk < 10)
  {
    Serial.println(F("Drive not mounted"));
    disk = Display.file_Mount();
    Serial.print(F("Disk = ")); Serial.println(disk);
  }
  Display.gfx_Cls();
//  Serial.print(F("Disk = ")); Serial.println(disk);
  
  hndl = Display.file_LoadImageControl("HEKXO_~1.dat", "HEKXO_~1.gci", 1);
  while (hndl < 4000)
  {
    Serial.println(F("Load image control failed"));
    hndl = Display.file_LoadImageControl("HEKXO_~1.dat", "HEKXO_~1.gci", 1);
    Serial.print(F("Handle = ")); Serial.println(hndl);
  }

  if(!Display.media_Init())
  {
    Serial.println("File not mounted...");
  }
//  Serial.print(F("Handle = ")); Serial.println(hndl);
  
  for(int i = 0; i < 2; i++)
  {
    word frame = 0;
    do
    {
      Display.img_SetWord(hndl, iUserimages1, IMAGE_INDEX, frame);
      Display.img_Show(hndl, iUserimages1);
      frame++;
    }while(frame < 8);
  }
  
//  Display.gfx_Cls();
  Display.file_Close(hndl);
//  Display.file_Unmount();
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




