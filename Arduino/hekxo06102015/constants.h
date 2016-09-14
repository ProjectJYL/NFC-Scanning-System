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
#define iUserimages4H 0x01B9 //no image found
#define iUserimages4L 0x8800

#define iVideo1H 0x017E //clock animation
#define iVideo1L 0x0600


