#include "EEPROM.h"  //①
#include <Wire.h>
#include "M5Atom.h"
const bool SerialEnable = true;
const bool I2CEnable = false; //falseにしとかないとハングする
const bool DisplayEnable = true;

EEPROMClass  NAMES("eeprom0");
EEPROMClass  HEIGHT("eeprom1");
EEPROMClass  AGE("eeprom2");

const char* name = "Teo Swee Ann";
char rname[32];
const char* age = "46";
char rage[32];
const char* height = "175.2";
char rheight[32];

void setup() {
  M5.begin();
  M5.update();
  Serial.begin(115200);
  Serial.println("\nTesting EEPROM Library\n");

  if (!NAMES.begin(0x500)) {
    Serial.println("Failed to initialise NAMES");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  if (!HEIGHT.begin(0x200)) {
    Serial.println("Failed to initialise HEIGHT");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  if (!AGE.begin(0x100)) {
    Serial.println("Failed to initialise AGE");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }

/*  
  if (!EEPROM.begin(1000)) {  //②
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
*/

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
