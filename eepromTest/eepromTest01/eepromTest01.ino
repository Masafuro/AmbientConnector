#include "EEPROM.h"  //①
#include <Wire.h>
#include "M5Atom.h"
const bool SerialEnable = true;
const bool I2CEnable = false; //falseにしとかないとハングする
const bool DisplayEnable = true;


int address=0;  
int count=0;


void setup() {
  M5.begin();
  M5.update();
  Serial.begin(115200);
  Serial.println("\nTesting EEPROM Library\n");
  if (!EEPROM.begin(1000)) {  //②
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }

  count=EEPROM.readInt(address);  //③
  Serial.print(SETUP:);
  Serial.println(count);
   
}
 

void loop() {
  M5.update();
  Serial.println(count);
  M5.Lcd.setCursor(0, 0);
  
  if(M5.Btn.wasPressed()){
    count=count+1;
    EEPROM.writeInt(address,count);//③
    EEPROM.commit();    
    Serial.println(count);
    delay(100);
  }

  if(M5.BtnB.wasPressed()){
    if(count>0){
      count=count-1;
      EEPROM.writeInt(address,count);
      EEPROM.commit();          
      Serial.println(count);
      delay(100);
    }
  }
}
