#include <M5Atom.h>
#include <WiFi.h>       //WiFi接続用
#include <WebServer.h>  //サーバー設定用

// アクセスポイント設定
const char ssid_AP[] = "lolo";  //SSID
const char pass_AP[] = "4444";  //パスワード(8文字以上)

// サーバー設定ポート80で接続
WebServer server(80);
// htmlメイン画面データ（「"」は「\"」に置き換え、htmlの改行は「\n」、コードの改行は「\」）
char html[] = "\
<!DOCTYPE html><html lang=\"jp\"><head>\n\
<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
<title>REMOTE-SWITCH</title>\n\
</head><body>\n\
  <h1>REMOTE-SWITCH</h1>\n\
  <div>\n\
    <button style=\"height:50px\" onclick=\"location.href='/get/btn_on'\">　ON　</button>\n\
    <button style=\"height:50px\" onclick=\"location.href='/get/btn_off'\">　OFF　</button>\n\
  </div>\n\
</body></html>\n";

// FastLED（CRGB構造体）設定
CRGB dispColor(uint8_t r, uint8_t g, uint8_t b) {
  return (CRGB)((r << 16) | (g << 8) | b);
}
//-------------------------------------------------
// サーバーリクエスト時処理関数
//-------------------------------------------------
// ルートアクセス時の応答関数
void handleRoot() {
  server.send(200, "text/html", html);  //レスポンス200を返しhtml送信
}
// エラー（Webページが見つからない）時の応答関数
void handleNotFound() {
  server.send(404, "text/plain", "404 Not Found!");  //text送信
}
// ブラウザONボタン処理
void btnOn() {
  digitalWrite(21, HIGH);                   //出力0 ON（青色LED）
  M5.dis.drawpix(0, dispColor(0, 0, 200));  //本体LED（青）
  server.send(200, "text/html", html);  //レスポンス200を返しhtml送信
}
// ブラウザOFFボタン処理
void btnOff() {
  digitalWrite(21, LOW);                    //出力0 OFF（青色LED）
  M5.dis.drawpix(0, dispColor(20, 20, 20)); //本体LED（白）
  server.send(200, "text/html", html);  //レスポンス200を返しhtml送信
}
//-------------------------------------------------
// 初期設定
//-------------------------------------------------
void setup() {
  M5.begin(true, false, true);    //Serial,POWER,LED
  Serial.begin(9600);             //標準のシリアル通信設定

  // アクセスポイント設定
  WiFi.softAP(ssid_AP, pass_AP);  //ソフトAP設定

  // サーバー設定
  server.on("/", handleRoot);         //ルートアクセス時の応答関数を設定
  server.onNotFound(handleNotFound);  //Webページが見つからない時の応答関数を設定
  server.on("/get/btn_on", btnOn);    //ボタンオン受信処理
  server.on("/get/btn_off", btnOff);  //ボタンオフ受信処理
  server.begin();                     //Webサーバー開始

  // シリアル出力
  Serial.printf("\nSSID:%s\n", ssid_AP);//SSID
  Serial.printf("PASS:%s\n", pass_AP);  //PASSWORD
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());      //IPアドレス（配列）

  // 本体LED設定
  M5.dis.drawpix(0, dispColor(20, 20, 20)); //LED点灯（白）
  // 入出力端子設定
  pinMode(21, OUTPUT);  //出力端子
}
//-------------------------------------------------
// メイン
//-------------------------------------------------
void loop() {
  server.handleClient();  //クライアントからのアクセス確認
  delay(200);             //遅延時間（ms）
}
