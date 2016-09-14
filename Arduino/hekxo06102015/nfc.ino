
/*************************************************************
 * //@filename nfc_test.ino
 * //@author Jian Yet Lee
 * //Copyright (c) 2015 WebTeam Corporation. All rights reserved.
 *************************************************************/

extern uint8_t nfc_cmd[20];
void nfc_loop(void) {
  uint8_t uid[] = { 
    0, 0, 0, 0, 0, 0, 0                                 };  // Buffer to store the returned UID

  memcpy(uid_buffer, (const uint8_t[]) {
    0, 0, 0, 0, 0, 0, 0            }
  , 7);
  memcpy(global_tag_info, (const uint8_t[]) {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0            }
  , 16);

  uint8_t uidLength;                        
  int i = 20; //img index counter to help find the last two bytes of nfc_cmd
  switch(nfc_cmd[0]) {
    case (kTurn_Reader_On_For_Registration):
    if(!readUID(uid, uidLength, 0)) 
    {
      nfc_response = kAuthentication_Failed;
    } 
    else 
    {
      nfc_response = kReader_Turned_On_For_Registration;
    }
    
//    nfc_cmd[0] = 0;
    break;
    
    case (kTurn_Reader_Off):
    nfc_response = kReader_Turned_Off;
//    digitalWrite(NFC_READ_LED, LOW);
//    Serial.println(F("Turning off nfc reading..."));
//    nfc_cmd[0] = 0;
      
    break;

    case (kWrite_On_Reader):
    char *uid_ptr;
    char *tag_info_ptr;
    char *ind_ptr;
    //find the position for the last two bytes in nfc_cmd
    while(nfc_cmd[i-1] != 0x26)
    {
      i--;
    }
    //parse nfc_cmd to find the uid, text to write on tag and image index
    strtok( (char *)nfc_cmd, "&");
    uid_ptr = strtok( NULL, "&");
    Serial.print(F("received uid: "));
    nfc.PrintHexChar((uint8_t *)uid_ptr, 4);
    tag_info_ptr = strtok( NULL, "&");
    Serial.print(F("write text: ")); 
    Serial.println(tag_info_ptr);
    //parse nfc_cmd for image index
    uint8_t ind_array[2];
    ind_array[0] = nfc_cmd[i];
    ind_array[1] = nfc_cmd[i+1]; 
    Serial.print(F("i = ")); Serial.println(i);
    Serial.print(F("ind_ptr[0] = ")); Serial.println(ind_array[0], HEX);
    Serial.print(F("ind_ptr[1] = ")); Serial.println(ind_array[1], HEX);
    uint16_t img_ind;
    //form a 16-bit integer of the image index
    img_ind = ind_array[0] << 8;
    img_ind |= ind_array[1];
    Serial.print(F("index = ")); Serial.println(img_ind);
    
    if(!writeBlock(uid, (uint8_t *)uid_ptr, uidLength, (uint8_t *)tag_info_ptr, 4, strlen(tag_info_ptr))) 
    {
      nfc_response = kWrite_On_Reader_Fail;
    } 
    else 
    {
      nfc_response = kWrite_On_Reader_Successful;
    }
    //write image index on block 6
    if(!writeBlock(uid, (uint8_t*)uid_ptr, uidLength, ind_array, 6, sizeof(img_ind)))
    {
      Serial.println(F("Failed to write image index on block 6"));
    } 
    else
    {
      Serial.println(F("Written image index on block 6"));
    }
    break;

    case (kTurn_Reader_On_For_Reading):
    digitalWrite(NFC_READ_LED, HIGH);
    retrieve_tag_info(uid, uidLength, 100, 4);
    nfc_response = kReader_Turned_On_For_Reading;
    break;
    
  default:
  break;
  }

}

uint8_t readUID(uint8_t* uid, uint8_t &uidLength, const uint16_t timeout) {
  uint8_t success;

  Serial.println(F("read uid. show me a nfc tag"));
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, timeout);

  if (success) {
    // Display some basic information about the card
    Serial.print(F("UID Value: "));
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    memcpy(uid_buffer, uid, uidLength);
//    Serial.print("UID Value in uid_buffer"); 
//    nfc.PrintHex(uid_buffer, uidLength);

    if (uidLength == UID_LENGTH_4 || uidLength == UID_LENGTH_7)
    {        
      uint8_t keya[6] = {'p', 'e', 'n', 'g', 'u', 'i'};
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);    
    }
    else 
    {
      success = 0x00;
    }
  }
//if we have timeout do we still need delay?
//  delay(2000);
  return success;
}

uint8_t writeBlock(uint8_t* read_uid, const uint8_t* received_uid, uint8_t &uidLength, const uint8_t* message, const int block, const int numBytes) {
  uint8_t success = 0x00;
  Serial.println(F("write nfc tag. show me a nfc tag"));
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, read_uid, &uidLength);

  if(success) {
    if(uidLength == 4) {
        uint8_t keya[6] = {'p', 'e', 'n', 'g', 'u', 'i'};
      success = nfc.mifareclassic_AuthenticateBlock(read_uid, uidLength, 4, 0, keya);

      if(success) {
        if(success) {
          if(memcmp(read_uid, received_uid, 4) != 0) {
            Serial.println(F("Failed to match UID."));
            success = 0x00;
            return success;
          }
        }
        else 
        {
          return 0x00;
        }
        uint8_t write_data[16] = {
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                                                        };
        
        //copying sizeof(write_data) bytes writes random bytes on write_data. What should be the correct one?
        memcpy( write_data, message, numBytes);
        success = nfc.mifareclassic_WriteDataBlock (block, write_data);    

        if(success) 
        {
          Serial.print(F("Written ")); 
          nfc.PrintHex(write_data, numBytes); 
          Serial.println(F(" to tag."));
        }
        else 
        {
          return 0x00;
        }    
      }
      else 
      {
        return 0x00;
      }
    } 
    else
    {
      return 0x00;
    }
  } 
  else 
  {
    return 0x00;
  }
//do we need delay here?
//  delay(2000);
  return success;
}

uint8_t retrieve_tag_info(uint8_t* uid, uint8_t &uidLength, uint16_t timeout, int block) {
  uint8_t success;

  Serial.println(F("read uid. show me a nfc tag"));
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, timeout);

  if (success) {
    Serial.print(F("UID Value: "));
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    memcpy(uid_buffer, uid, uidLength);

    if (uidLength == UID_LENGTH_4 || uidLength == UID_LENGTH_7)
    {
      uint8_t keya[6] = {'p', 'e', 'n', 'g', 'u', 'i'};
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, block, 0, keya);

      if (success)
      {
        uint8_t data[16];
        success = nfc.mifareclassic_ReadDataBlock(block, data);

        if (success)
        {
          // Data seems to have been read ... spit it out
          Serial.println(F("Reading tag info:"));
          nfc.PrintHexChar(data, 16);
          Serial.println(F(""));
          memcpy(global_tag_info, data, 16);
        }
        
        //read image index in block 6
        success = nfc.mifareclassic_ReadDataBlock(6, data);
        
        if (success)
        {
          Serial.print(F("Reading block 6: "));
          nfc.PrintHexChar(data, 16);
          Serial.println(F(""));
          
          /*
          if(data[0] >= 0x0030 && data[0] <= 0xFFFF)
         {
           Display.img_SetWord(hndl, 0, IMAGE_INDEX, data[0]);
           Display.img_Show(hndl, 0);
           delay(1000);
           Display.gfx_Cls();
         } 
         */
        }
        
      } 
    }
  }
  return success;
}

uint8_t writeVersion(uint8_t* uid, uint8_t &uidLength) {
  uint8_t success;
  Serial.println(F("write nfc tag. show me a nfc tag"));
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if(success) {
    if(uidLength == 4) {
      uint8_t keya[6] = { 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF                                                                               };
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

      if(success) {
        Serial.println("Writing version: 12292014");
        uint8_t data[16];

        // If you want to write something to block 4 to test with, uncomment
        // the following line and this text should be read back in a minute
        memcpy(data, (const uint8_t[]){ 
          '1', '2', '2', '9', '2', '0', '1', '4', 0, 0, 0, 0, 0, 0, 0, 0                                 }
        , sizeof data);
        success = nfc.mifareclassic_WriteDataBlock (5, data);

        if (success)
        {
          Serial.println("Write version ok...");
        }    
      }
    } 
  }

  delay(2000);
  return success;
}












