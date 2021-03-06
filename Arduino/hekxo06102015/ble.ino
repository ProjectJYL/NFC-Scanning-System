 /*************************************************************
 * //@filename ble_test.ino
 * //@author Jian Yet Lee
 * //Copyright (c) 2015 WebTeam Corporation. All rights reserved.
 *************************************************************/
extern uint8_t uid_buffer[UID_LENGTH_4];
extern uint8_t nfc_response;
extern uint8_t global_tag_info[16];
extern word hndl;

/**************************************************************************/
/*!
 This function is called whenever select ACI events happen
 */
/**************************************************************************/
void aciCallback(aci_evt_opcode_t event)
{
  switch(event)
  {
  case ACI_EVT_DEVICE_STARTED:
    Serial.println(F("Advertising started")); 
    break;
  case ACI_EVT_CONNECTED:
    Serial.println(F("Connected!"));
    uart.print(F("Connected"));
    break;
  case ACI_EVT_DISCONNECTED:
    Serial.println(F("Disconnected or advertising timed out"));
    //turn off nfc in case it's on
    nfc_cmd[0] = kTurn_Reader_Off;
    //play intro
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
    break;
  default:
    break;
  }
}

/**************************************************************************/
/*!
 This function is called whenever data arrives on the RX channel
 */
/**************************************************************************/
void rxCallback(uint8_t *buf, uint8_t len)
{
  Serial.print(F("Received "));
  Serial.print(len);
  Serial.print(F(" bytes: "));
  for(int i=0; i<len; i++)
    Serial.print((char)buf[i]); 

  Serial.print(F(" ["));

  for(int i=0; i< len; i++)
  {
    Serial.print(F(" 0x")); 
    Serial.print((char)buf[i], HEX); 
  }
  Serial.println(F(" ]"));
  
  if (buf[0] == kGood_Job) {
    digitalWrite(PLAY_CORRECT_LED, HIGH);
    delay(400);
    digitalWrite(PLAY_CORRECT_LED, LOW);
    delay(200);
    digitalWrite(PLAY_CORRECT_LED, HIGH);
    delay(400);
    digitalWrite(PLAY_CORRECT_LED, LOW);
    
    Display.media_SetAdd(iUserimages3H, iUserimages3L);
    
    for(int i = 0; i < 2; i++)
    {
      word frame = 0;
      do
      {
        Display.media_VideoFrame(0, 0, frame);
        delay(100);
        frame++;
      }while(frame < 5);
    }
    
    Display.gfx_BGcolour(DEEPSKYBLUE);
    Display.gfx_Cls();
  } 
  else if (buf[0] == kTry_Again) 
  {
    digitalWrite(PLAY_WRONG_LED, HIGH);
    delay(400);
    digitalWrite(PLAY_WRONG_LED, LOW);
    delay(200);
    digitalWrite(PLAY_WRONG_LED, HIGH);
    delay(400);
    digitalWrite(PLAY_WRONG_LED, LOW);
  }
  //show image when the app is in play mode 
 else if(buf[0] == kShow_Image)
 {
   int i = len;
   while(buf[i-1] != '&')
   {
     i--;
   }
   uint16_t img_ind;
   img_ind = buf[i] << 8;
   img_ind |= buf[i+1];
   Serial.print(F("img_ind :")); Serial.println(img_ind);
   
   Display.media_SetAdd(iUserimages2H, iUserimages2L);

   if(img_ind >= 0x0030 && img_ind <= 0x031E)
   {
     Display.media_VideoFrame(0, 0, img_ind - 0x0030);
   }else
   {
//    Display.media_SetAdd(iUserimages4H, iUserimages4L);
//    Display.media_Image(0, 0);
      Display.media_SetAdd(iVideo1H, iVideo1L);
      Display.media_Video(0, 0);
      delay(4000);
   }
 }
  else if(buf[0] == kWrite_Device_Name) 
  {
    char* dev_name_ptr;
    strtok((char *)buf, "&");
    dev_name_ptr = strtok(NULL, "&");
    Serial.println(dev_name_ptr);
    //if failed, send response
    if(setDevName(dev_name_ptr) == 0x00)
    {
      uart.write(kWrite_Device_Name_Failed);
    } else {
      uart.write(kWrite_Device_Name_Ok);
    }
    delete dev_name_ptr;
  } 
  else if (buf[0] > 0x30 && buf[0] < 0x39) 
  {
    memcpy(nfc_cmd, (const uint8_t[]){
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0    }
    , 20);    
    memcpy(nfc_cmd, buf, len);    
  }  
}

/**************************************************************************/
/*!
 Configure the Arduino and start advertising with the radio
 */
/**************************************************************************/


/**************************************************************************/
/*!
 Constantly checks for new events on the nRF8001
 */
/**************************************************************************/
void ble_loop()
{
  uart.pollACI();

  if(nfc_response == kReader_Turned_On_For_Registration) 
  {
    sendUID(kReader_Turned_On_For_Registration);
    nfc_cmd[0] = 0;
    nfc_response = 0;
  } 
  else if(nfc_response == kReader_Turned_Off)
  {
    digitalWrite(NFC_READ_LED, LOW);
    Serial.println(F("Turning off nfc reading..."));
    nfc_cmd[0] = 0;
    nfc_response = 0;
    
    
    //play hekxo start up animation
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
  }
  else if (nfc_response == kAuthentication_Failed) {
    Serial.println(F("Read UID fail..."));
    uart.write(kAuthentication_Failed);
    nfc_cmd[0] = 0;
    nfc_response = 0;
  }
  else if(nfc_response == kWrite_On_Reader_Successful) {
    Serial.println(F("Write tag with message successful..."));
    uart.write(kWrite_On_Reader_Successful);
    nfc_cmd[0] = 0;
    nfc_response = 0;
  } 
  else if(nfc_response == kWrite_On_Reader_Fail) {
    Serial.println(F("Write tag with message fail..."));
    uart.write(kWrite_On_Reader_Fail);
    nfc_cmd[0] = 0;
    nfc_response = 0;
  } 
  else if(nfc_response == kReader_Turned_On_For_Reading) {
    sendTagInfo(kReader_Turned_On_For_Reading);
    nfc_response = 0;
  }
}

size_t sendUID(uint8_t code) {
  size_t success;
  uint8_t dummy[UID_LENGTH_4] = {0, 0, 0, 0};
      
  if( memcmp( uid_buffer, dummy, sizeof(uid_buffer)
  ) != 0){
    //use strlen to determine the actual character counts excluding whitespaces
    if(strlen((char *)uid_buffer) == UID_LENGTH_4){
      Serial.println(F("Sending UID..."));
      uint8_t uid_buf_container[] = {code, kopCode_Data_Separator, uid_buffer[0], 
    uid_buffer[1], uid_buffer[2], uid_buffer[3]};
      success = uart.write(uid_buf_container, 6);
      return success; 
    } else {
      return 0x00;
    }
  } else {
    return 0x00;
  }
}

size_t sendTagInfo(const uint8_t code) {
  size_t success;
  uint8_t tag_info[26]; 
  uint8_t dummy[UID_LENGTH_4] = {0, 0, 0, 0};
  
  if( memcmp( uid_buffer, dummy, sizeof(uid_buffer)
  ) != 0){

    if(strlen((char *)uid_buffer) == UID_LENGTH_4){
      Serial.println(F("Sending UID..."));
      tag_info[0] = code;
      tag_info[1] = kopCode_Data_Separator;
      tag_info[2] = 0;//end of this string to concatenate 
      strncat( (char *)tag_info, (char *)uid_buffer, (size_t)UID_LENGTH_4);
      strncat( (char *)tag_info, "&", 1);
      strncat( (char *)tag_info, (char *)global_tag_info, 16);
      Serial.print(F("tag info: ")); 
      Serial.println( (char *)tag_info);
      success = uart.print( (char *)tag_info);
    } 
  }
  return success;
}

uint8_t getDevName(void) {
  uint8_t index = 2;
  if(EEPROM.read(index++) != 0x00) {
    return 0x00;
  }
  Serial.print(F("Device Name: "));
  uint8_t c;
  do {
//    Serial.print("hello");
    c = EEPROM.read(index);
    dev_name[index-3] = c;
    index++;
    Serial.print( (char)c);
  } while(c != 0x00 && index-4 <= 7); //index-4 = len of dev name
  Serial.println("");
  return index-4;
}

uint8_t setDevName(const char* name) {
  if(strlen(name) > 7) {
    return 0x00;
  }
  Serial.println(F("Writing EEPROM..."));
  uint8_t index = 2;
  //write a starting point 0x00
  EEPROM.write(index++, 0x00); 
  for(; index < strlen(name) + 3; ) {
    EEPROM.write(index, name[index - 3]);
    index++;
  }
  //write a stopping point 0x00
  EEPROM.write(index, 0x00);
  Serial.println(F("Writing done..."));
  delay(500);
  return index-4;
}






