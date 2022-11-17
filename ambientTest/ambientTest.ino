/*
 * M5StackとBME680をI2C接続し、温度、湿度、気圧を測定しプリントアプトする
 */
#include "Ambient.h"
//M5 Atom
#include "M5Atom.h"
const bool SerialEnable = true;
const bool I2CEnable = false; //falseにしとかないとハングする
const bool DisplayEnable = true;

WiFiClient client;
Ambient ambient;

const char* ssid = "Yokote_wifi";
const char* password = "yokoteyokote";
unsigned int channelId =  57790; // AmbientのチャネルID
const char* writeKey = "d0275d3574deaa6a"; // ライトキー

void setup(){
    delay(5000);
    M5.begin( SerialEnable, I2CEnable, DisplayEnable);// ボーレートは115200のシリアル通信がセットされる。
    delay(10000);

    WiFi.begin(ssid, password);  //  Wi-Fi APに接続
    while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fi AP接続待ち
        delay(100);
    }

    Serial.print("WiFi connected\r\nIP address: ");
    Serial.println(WiFi.localIP());

    ambient.begin(channelId, writeKey, &client); // チャネルIDとライトキーを指定してAmbientの初期化

}

void loop() {

    ambient.set(1, 10 );
    ambient.send();

    delay(10000);
}
