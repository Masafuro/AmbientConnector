#include "EEPROM.h"  //①
#include <Wire.h>
#include "M5Atom.h"
const bool SerialEnable = true;
const bool I2CEnable = false; //falseにしとかないとハングする
const bool DisplayEnable = true;

//eeprom 初期設定
/*
 * 1.wifi
 * 2.pass
 * 3.ambientReadkey
 * 4.ambientWriteKey
 * 5.ambientChannelid
 * 6.iotMode
 * 7.systemVersion
 * 8.cycleTime
 * 9.cmnt
 * 
 */

EEPROMClass  WSSID("eeprom0");
EEPROMClass  PASS("eeprom1");
EEPROMClass  RKEY("eeprom2");
EEPROMClass  WKEY("eeprom3");
EEPROMClass  CID("eeprom4");
EEPROMClass  IOTMODE("eeprom5");
EEPROMClass  SYSTEMVERSION("eeprom6");
EEPROMClass  CYCLETIME("eeprom7");
EEPROMClass  CMNT("eeprom8");

const char* wssid = "Teo Swee Ann";
char awssid[64];
const char* pass = "46";
char apass[64];
const char* rkey = "175.2";
char arkey[64];
const char* wkey = "Teo Swee Ann";
char awkey[64];
const char* cid = "46";
char acid[64];
const char* iotmode = "175.2";
char aiotmode[64];
const char* systemversion = "Teo Swee Ann";
char asystemversion[64];
const char* cycletime = "46";
char acycletime[64];
const char* cmnt = "175.2";
char acmnt[64];

void setup() {
  M5.begin();
  M5.update();
  Serial.begin(115200);
  Serial.println("\nTesting EEPROM Library\n");
  eepromSetup();
  
}
 

void loop() {
  M5.update();
  
  if(M5.Btn.wasPressed()){
      NAMES.writeString(0, name);
      HEIGHT.writeString(0, height );
      AGE.writeString(0, age);
      NAMES.commit();
      HEIGHT.commit();
      AGE.commit();
      // eeprom.end(); もあるよ。
      Serial.print("PUT: ");
      Serial.print("name: ");   Serial.println(name);
      Serial.print("height: "); Serial.println(height);
      Serial.print("age: ");    Serial.println(age);
      Serial.println("------------------------------------\n");    
    delay(1000);
  }

  NAMES.get(0, rname);
  HEIGHT.get(0, rheight);
  AGE.get(0, rage);
  Serial.print("GET:");
  Serial.print("name: ");   Serial.println(rname);
  Serial.print("height: "); Serial.println(rheight);
  Serial.print("age: ");    Serial.println(rage);
  Serial.println("Done!");
  delay(1000);

}

void eepromSetup(){
  String error = "EEPROM_ERROR:";
  
  if (!WSSID.begin(64)) { //0x500
    error = error + "WSSID:";
  }
  if (!PASS.begin(64)) {
    error = error + "PASS:";
  }
  if (!RKEY.begin(64)) {
    error = error + "RKEY:";
  }
  if (!WKEY.begin(64)) {
    error = error + "WKEY:";
  }
  if (!CID.begin(64)) {
    error = error + "CID:";
  }
  if (!IOTMODE.begin(64)) {
    error = error + "IOTMODE:";
  }
  if (!SYSTEMVERSION.begin(64)) {
    error = error + "SYSTEMVERSION:";
  }
  if (!CYCLETIME.begin(64)) {
    error = error + "CYCLETIME:";
  }
  if (!CMNT.begin(64)) {
    error = error + "CMNT:";
  }

  /*
EEPROMClass  SYSTEMVERSION("eeprom6");
EEPROMClass  CYCLETIME("eeprom7");
EEPROMClass  CMNT("eeprom8");
   */

}
