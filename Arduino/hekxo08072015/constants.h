#include <SoftwareSerial.h>
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_Const4D.h"

//from ble
#define PLAY_CORRECT_LED 7
#define PLAY_WRONG_LED 6
#define NFC_READ_LED 5

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 3
#define ADAFRUITBLE_RST 9

#define UID_LENGTH_4 4
#define UID_LENGTH_7 7

uint8_t nfc_cmd[20];
uint8_t dev_name[7];

static Adafruit_BLE_UART uart(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
//from nfc
#define IRQ   (2)
#define RESET (9)

uint8_t uid_buffer[UID_LENGTH_4];
uint8_t nfc_response;
uint8_t global_tag_info[16];

static Adafruit_NFCShield_I2C nfc(IRQ, RESET);

//from image
//for arduino uno 
//SoftwareSerial DisplaySerial(12,13) ; // pin 12 = TX of display, pin 13 = RX
#define DisplaySerial Serial1
#define RESETLINE 4

static Goldelox_Serial_4DLib Display(&DisplaySerial);

word hndl;

#define iUserimages1 0x0000 //start up animation 
#define iUserimages2H 0x0004 //object image
#define iUserimages2L 0x0200 //object image
#define iUserimages3H 0x017B //great job animation
#define iUserimages3L 0x8400 //great job animation
#define iUserimages4H 0x01B9
#define iUserimages4L 0x8800
#define iVideo1H 0x017E
#define iVideo1L 0x0600
#define iVideo2H 0x01BA
#define iVideo2L 0x0A00
#define iVideo3H 0x01EB
#define iVideo3L 0x0C00
#define iVideo4H 0x01FE
#define iVideo4L 0x8E00

//
//  opCodes.h
//  MagicWandInteraction
//
//  Created by RIPL on 12/12/14.
//  Copyright (c) 2014 rangam.com. All rights reserved.
//

typedef enum {
    
    
//===============================From App to Hardware OpCodes   =======================================
    
  REGISTER_UID =            0x30,
  STOP_SCANNING =           0x31,    
  START_SCANNING =          0x32, 
  WRITE_DATA =              0x33,
  WRITE_DEVICE_NAME =       0X34,
	PLAY_SOUND =              0X35,
  PLAY_ANIMATION =          0X36,
  DISPLAY_IMAGE =           0X37,
  CONVERTNSEND =            0X38,
  IMAGE_SENT =              0X39,
  SYNC_DATA =               0x40,
    
    //=============================== Seprator for opCode and Data   =======================================
    
    SEPARATOR = 0x26,  // &

    //=============================== Seprator for opCode and Data   =======================================


//===============================From Hardware to App OpCodes   =======================================

   SEND_UID =             0x50,
   IMAGE_PRESENT =        0X51,
   IMAGE_ABSENT =         0X52,

//==============================For Hardware to App OpCodes   =======================================
//extra globals to help in debugging
 kReader_Turned_On_For_Registration = 0x41,
 kReader_Turned_Off = 0x42,
 kAuthentication_Failed = 0x43,
 kReader_Turned_On_For_Reading = 0x45,
 kWrite_On_Reader_Successful = 0x46,
 kWrite_On_Reader_Fail = 0x47,
 kImage_Transfer_Ack = 0x48,
 kWrite_Device_Name_Ok = 0x49,
 kWrite_Device_Name_Failed = 0x51
 
} opCodes;



