/*************************************************************
//@filename hekxo06102015.ino
//@author Jian Yet Lee
//Copyright (c) 2015 WebTeam Corporation. All rights reserved.
This is hekxo program for Uno and Goldelox graphics processor

*///**************************************************************
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <Adafruit_BLE_UART.h>
#include <MemoryFree.h>
#include <SPI.h>
#include <EEPROM.h>
#include "constants.h"
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_Const4D.h"
#include <SoftwareSerial.h>

void setup(void) {  
  Serial.begin(115200);
  //  Serial.print(F("Free RAM: ")); Serial.println(freeMemory(), DEC);
  initializeLED();
  ble_setup();
  nfc_setup();
  tft_setup();
}

void loop(void) {
  ble_loop();
  nfc_loop();
  //tft_loop();
}
