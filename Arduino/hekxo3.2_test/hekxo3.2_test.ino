///*************************************************************
//@filename main.ino
//@author Jian Yet Lee
//Copyright (c) 2014 WebTeam Corporation. All rights reserved.
//**************************************************************
#define NO_OPCODE 

#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <Adafruit_BLE_UART.h>
#include <SPI.h>
#include <EEPROM.h>
#include "constants.h"
#include "Picaso_Serial_4DLib.h"
#include "Picaso_const4D.h"

void setup(void) {  
  Serial.begin(115200);
  initializeLED();
  ble_setup();
  nfc_setup();
  tft_setup();
}

void loop(void) {
  ble_loop();
  nfc_loop();
}
