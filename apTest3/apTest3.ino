#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

const char ssid[] = "ESP32AP-TEST";
const char pass[] = "12345678";       // パスワードは8文字以上
const IPAddress ip(192,168,123,45);
const IPAddress subnet(255,255,255,0);
AsyncWebServer server(80);            // ポート設定

void setup()
{
  Serial.begin(115200);
  Serial.println("SYSTEM STARTED.");
  // SPIFFSのセットアップ
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  WiFi.softAP(ssid, pass);           // SSIDとパスの設定
  delay(100);                        // このdelayを入れないと失敗する場合がある
  WiFi.softAPConfig(ip, ip, subnet); // IPアドレス、ゲートウェイ、サブネットマスクの設定
  
  IPAddress myIP = WiFi.softAPIP();  // WiFi.softAPIP()でWiFi起動

  // 各種情報を表示
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // GETリクエストに対するハンドラーを登録
  // rootにアクセスされた時のレスポンス
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });


  // style.cssにアクセスされた時のレスポンス
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/lit.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/lit.css", "text/css");
  });

  //URLクエリを処理
  server.on("/d/", HTTP_GET, [](AsyncWebServerRequest *request){
    int paramsNr = request->params();
    Serial.println(paramsNr);
    for(int i=0;i<paramsNr;i++){
        AsyncWebParameter* p = request->getParam(i);
        Serial.print("Param name: ");
        Serial.println(p->name());
        Serial.print("Param value: ");
        Serial.println(p->value());
        Serial.println("------");
    }
//    request->send(200, "text/plain", "message received");
      request->send(SPIFFS, "/finish.html");

  });

  // サーバースタート
  server.begin();

  Serial.println("Server start!");
  Serial.println(WiFi.localIP());
}

void loop() {}
