#include <SpritzCipher.h> 
#include <Int64String.h>
#define hex_char(n) ((n) < 10 ? '0' + (n) : 'A' + ((n)-10)) 

//M5 Atom
#include "M5Atom.h"
const bool SerialEnable = true;
const bool I2CEnable = false; //falseにしとかないとハングする
const bool DisplayEnable = true;

//LEDカラーセット M5.dis.drawpix(0, Name );
const int red     = 0xff0000 ;
const int yellow  = 0xffff00 ;
const int blue    = 0x0000ff ;
const int green   = 0x00ff00 ;
const int purple  = 0xff00ff ;
const int skyBlue = 0x00ffff ;
const int white   = 0xffffff ;
const int off     = 0x000000 ;
const int smallGreen   = 0x001100 ;
const int smallSkyblue   = 0x001111 ;
const int smallBlue   = 0x000011 ;
const int smallRed   = 0x110000 ;
const int smallPurple  = 0x110011 ;
const int smallYellow  = 0x111100 ;

//モード設定
long M = 0; //M数はコマンドモードを判別する数値。Mが1以上ならコマンドモード。実体としてはフリーループ数で、M数の分だけ作業をスキップする。
const long FL = 1126925; //M数に設定するフリーループ数の初期値1126925で約1分作業をスキップする。

//コマンド用
String input = "0"; //\nまでの取得した文字列を格納する。

//データ保存用map
#include <SimpleMap.h> //https://github.com/spacehuhn/SimpleMap
SimpleMap<String, String>* dataMap; //dataMapはkey, dataで格納される。
SimpleMap<String, String>* receiveMap; //receiveMapは、取得した順番(0~), 内容 で格納される。
char ENDCODE  = '\n';
const char s[2] = " "; // コマンドのパース文字
char *miniData;

//作業用
long C = 6000 ; //作業サイクルを設定する(ms) millisは50日でオーバーフローする。
unsigned long C0 = 0;

//Wifi接続
#include <WiFi.h>
#include <ESP32Ping.h>

//Ambient接続
#include <Ambient.h>
Ambient ambient;
bool ambientStatus;
WiFiClient client;

//再接続用
unsigned long previousMillis = 0;
unsigned long interval = 30000;

//各モード
bool modeSetup = false;
int upCount = 0;

void setup() {
  
  M5.begin( SerialEnable, I2CEnable, DisplayEnable);// ボーレートは115200のシリアル通信がセットされる。
  delay(10000);
  Serial.println("Start Ambient Connector.");
  M5.dis.drawpix(0, smallRed );

  M = 0; //M数

  createDatamap();
  createReceivemap();
  setDefaultdata();
  bool W = connectWifi();
  bool A = connectAmbient();
  C = dataMap->get( "cycleTime" ).toInt();
  
}



void loop() {

  commandTrigger(); //コマンド入力時割り込み処理
  bool W = wifiCheck();
  
  if( M == 0 ){ //モード分岐

    if( dataMap->get( "iotMode" ) == "cycle" ){
        if( millis() - C0 > C ){
          //サイクル作業モードにはいる
          C0 = millis(); //作業開始サイクル地点
          M5.dis.drawpix(0, skyBlue );
          //作業開始
            cycleAction();
          //作業終了
          M5.dis.drawpix(0, off );
        }else{
          //サイクル待機モードにはいる
          M5.dis.drawpix(0, smallSkyblue );
        }
    }else if( dataMap->get( "iotMode" ) == "trigger" ){
 
       M5.dis.drawpix(0, smallBlue );
       triggerAction();
       
    }else{
      //IoTモード例外
    }
    
  }else if( M == 1 ){
    //作業モード復帰前処理
    Serial.print( "作業モードへ入ります。" );
    variablePrint( "モード", dataMap->get( "iotMode" ) );
    
    
  }else{
    //コマンドモードにはいる
    M5.dis.drawpix(0, yellow );
  }

  //M値を処理
  M = M - 1;
  if( M < 0 )M = 0;
  M5.update();
  
} // loop終了

void triggerAction(){ //トリガーモードでの動作

  String sm = dataMap->get("sensorMode"); // 設定されたセンサーモードを取得

  if( sm == "button" ){
    buttonFunc();
  }else{ //センサーモード分岐の末尾
    Serial.println("センサーモード設定がおかしいようです。");
  }
  
}

void cycleAction(){ // サイクルモードでの動作
   String sm = dataMap->get("sensorMode");
    sendInt( 1, random(0,101) );
   Serial.println("cycle送信しました。");
}

void buttonFunc(){
  if ( M5.Btn.wasReleased() ) {
    M5.dis.drawpix(0, smallYellow );
    Serial.println( "ボタンが押されましたよ。" );
    bool r = sendInt( 1, 1 ); // sendInt( int, int );
    variablePrint( "ambientデータ送信", String( r ) );
    M5.dis.drawpix(0, off );
  }
}

void variablePrint(String a, String b){
  // a は b です。と表示する関数
  Serial.print( a );
  Serial.print( "は" );
  Serial.print( b );
  Serial.println("です。");
  
}


bool sendInt( int f, int D ){ // f:フィールド１～８を指定。D:送信するデータ
   
   ambient.set( f, D );
   return ambient.send();

}

bool sendChar( int f, char D ){ // f:フィールド１～８を指定。D:送信するデータ
   
   ambient.set( f, D );
   return ambient.send();

}

bool sendDouble( int f, double D ){ // f:フィールド１～８を指定。D:送信するデータ
   
   ambient.set( f, D );
   return ambient.send();

}



bool connectAmbient(){
  String ids = dataMap->get( "ambientChannelid" );
  int id = (int)ids.toInt();
  char wkey[100];
  String wkeys = dataMap->get( "ambientWritekey" );
  wkeys.toCharArray( wkey, 100);
  
  ambientStatus = ambient.begin( id , wkey, &client );
  variablePrint( "アンビエント接続", String( ambientStatus ) );
  int i = 0;

  /*
  while( ambientStatus == false ){
    i = i + 1;
    Serial.println("クラウドサービスAmbientとの接続に失敗しました。再起動します。");
    ambientStatus = ambient.begin( id , wkey, &client );
    if( i > 5 ){
      break;
    }
  }
  */
  if( ambientStatus == false){
    Serial.println("クラウドサービスAmbientとの接続に失敗しました。再起動します。");
    ESP.restart();
  }


  return ambientStatus;
}

bool wifiCheck(){

  switch( WiFi.status() ){
    case 1:
      Serial.println("1:SSIDがありません。");
      connectWifi();
      break;
    case 4:
      Serial.println("4:接続に失敗しました。");
      connectWifi();
      break;
    case 5:
      Serial.println("5:接続を見失いました。");
      connectWifi();
      break;
    case 6:
      Serial.println("6:接続が切れました。");
      ESP.restart();
      break;
    default:
      break;
  
  }

  return true;

    /* WiFi.status() 値の結果
     *   255:WL_NO_SHIELD,
     *    0:WL_IDLE_STATUS, 
     *    1:WL_NO_SSID_AVAIL,
     *    2:WL_SCAN_COMPLETED
     *    3:WL_CONNECTED,
     *    4:WL_CONNECTED_FAILED,
     *    5:WL_CONNECTION_LOST,
     *    6:WL_DISCONNECTED
     *  
     */

}

void commandTrigger(){

    readCommand(); //入力された文字をsのパース文字で区切ってReceiveMapに格納
  if( receiveMap->size() > 0 ){
    //コマンド処理
    doCommand( receiveMap->getData(0) );
    receiveMap->clear();
    Serial.flush(); //シリアル受信データの初期化。コマンド実施中に他のコマンドを打つものは受け付けない。
  }

}

void disconnectWifi(){
  
  bool ds = WiFi.disconnect( true, true);
  while ( ds != false ){
    ds = WiFi.disconnect( true, true);
    Serial.print( ds );
    Serial.println( ":WiFi.disconnect" );
  }
  Serial.print( ds );
  Serial.println( ":WiFi.disconnect" );

}


bool connectWifi(){
  //https://github.com/espressif/arduino-esp32/pull/6226 のWifiライブラリを入れることで接続が安定化する。（接続バグがfixされている。)

  M5.dis.drawpix(0, smallPurple );
  Serial.println( "Wifi接続初期設定を開始。" );
  unsigned long cnt = 0;
  int cnt2 = 0;

  disconnectWifi();

  WiFi.begin( dataMap->get("wifi").c_str(), dataMap->get("pass").c_str() );
  Serial.println( "Wifiへの接続をはじめます。" );
  int cl = millis();
  
  while(WiFi.status() != WL_CONNECTED) {

   // Serial.println( WiFi.status() ); 
    /* WiFi.status() 値の結果
     *   255:WL_NO_SHIELD,
     *    0:WL_IDLE_STATUS, 
     *    1:WL_NO_SSID_AVAIL,
     *    2:WL_SCAN_COMPLETED
     *    3:WL_CONNECTED,
     *    4:WL_CONNECTED_FAILED,
     *    5:WL_CONNECTION_LOST,
     *    6:WL_DISCONNECTED
     *  
     */
    cnt++;
    //delay(5000);
    if( WiFi.status() == 4 ){
      Serial.println("接続に失敗しました。再起動します。");
      ESP.restart();
    }

    //https://randomnerdtutorials.com/solved-reconnect-esp32-to-wifi/
    unsigned long currentMillis = millis();
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
      Serial.print(millis());
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
      previousMillis = currentMillis;
    }


  }
  
    Serial.print( "接続に" );
    Serial.print( millis() - cl );
    Serial.println( "ミリ秒かかりました。" );


  bool p = Ping.ping("furukawakikaku.jp");

  float pingTime = Ping.averageTime();
  Serial.print( "pingは" );
  Serial.print( String( pingTime ) );
  Serial.println( "ミリ秒でした。" );
  Serial.println("Wifiに接続しました。");
  M5.dis.drawpix(0, off );

  return true;
}


void setDefaultdata(){
  
  dataMap->put("wifi" , "Yokote_wifi" );
  dataMap->put("pass", "yokoteyokote");
//  dataMap->put("wifi" , "12345678" );
//  dataMap->put("pass", "abcdefg");
  dataMap->put("deviceName", "ChocoSensor_v1");
  dataMap->put("devicePassword", "0000");
  dataMap->put("ambientReadkey", "58c3f9698b899738");
  dataMap->put("ambientWritekey", "c1644dbd6fbe52af");
  dataMap->put("ambientChannelid", "31837");
  dataMap->put("iotMode", "cycle"); 
  // IoTモード null モードなし、cycle 周期実行モード、trigger トリガー実行モード
  dataMap->put("sensorMode", "bps_unit"); 
  // センサーモード SensorMode(iotModer)->button(trigger):ボタン、bps_unit(cycle)
  //ambientのデータ フィールド番号（１～８）, データ内容（int または double )
  dataMap->put("cycleTime", "60000"); //サイクルモードのサイクル実行間隔を設定する。
  dataMap->put("1", "0000");
  dataMap->put("2", "0000");
  dataMap->put("3", "0000");
  dataMap->put("4", "0000");
  dataMap->put("5", "0000");
  dataMap->put("6", "0000");
  dataMap->put("7", "0000");
  dataMap->put("8", "0000");
  dataMap->put("cmnt", ""); //コメント用データ
 
  //Serial.println( toJSON( dataMap ) );
  
}


void doCommand( String c ){

    Serial.println( c );
    c.trim(); //格納された文字もtrimしないとifで一致しない。
  if( c == "cmd-on" ){ //cmd-onコマンドはM数を初期化することでコマンドモード時間を伸ばすことができる。
    M = FL; //正しいコマンドならコマンド受付時間を延長する。
    Serial.println("コマンドの受付をはじめます。");
  }else if( c == "cmd-off" ){ //cmd-offコマンドはM数を0にすることでコマンドモードを終了する。
    Serial.println("コマンドの受付を終了します。");
    M = 10; // M = 0だと復帰前処理をパスする。
    C0 = millis() - C + 5000; //コマンドモード終了後5秒後に初期実行を開始する
  }else if( c == "set-c" ){
    if( receiveMap->size() == 2){ 
      M = FL;
      String data1 = receiveMap->getData(1);// get xxxx の xxxxを取り出す。
      data1.trim(); 
      long C9 = data1.toInt();
      if( String( C9 ) == data1 ){
        C = C9;
        Serial.print("C:");
        Serial.print( C );
        Serial.println(" にセットしました。");   
      }else{
        Serial.println("セットできない値です。Cの設定範囲は、0~2147483647です。");
      }
    }else{
      Serial.println("なにかコマンドがおかしいようです。");
    }
  }else if( c == "get-c" ){
    M = FL;
    Serial.print("C:");
    Serial.print( C );
  }else if( c == "restart"){
    Serial.println("再起動を実行します。");
    esp_deep_sleep(1*1000000);
  }else if( c == "get"){ //
    M = FL; //コマンド受付時間を延長
    Serial.println("情報取得モードを開始します。");
    if( receiveMap->size() == 2){ 
      //getコマンドは、get xxxx でxxxxに該当するデータを取得する。よってパースした文字列は２つ。
      Serial.println("正しいコマンドです。");
      String data1 = receiveMap->getData(1);// get xxxx の xxxxを取り出す。
      data1.trim(); // trimしないとif文は一致しない。
      if( dataMap->has( data1 ) ){ //dataMapにdata1があればtrue
        int i = dataMap->getIndex( data1 ); //data1のインデックス数を取得
        String returnValue = dataMap->getData(i); //data1 のkeyに該当するデータを取り出す。
        Serial.print( data1 );
        Serial.print( ":");
        Serial.println( returnValue );
      }else{
        Serial.print( data1 );
        Serial.println(":そのデータはありませんでした。");
      }
    }else{
      Serial.println("コマンドがおかしいようです。");
    }
  }else if( c == "set"){ //
    M = FL; //コマンド受付時間を延長
    Serial.println("情報設定モードを開始します。");
    if( receiveMap->size() == 3){ 
      //setコマンドは、set xxxx AAAAでxxxxにAAAAを書き込む。よってパースした文字列は3つ。
      Serial.println("正しいコマンドです。");
      String data1 = receiveMap->getData(1);// get xxxx の xxxxを取り出す。
      String data2 = receiveMap->getData(2);// get xxxx の xxxxを取り出す。
      data1.trim(); // trimしないとif文は一致しない。
      data2.trim();
      if( dataMap->has( data1 ) ){ //dataMapにdata1があればtrue
        dataMap->put( data1, data2 ); //data1の項目にdata2を書き込み
        Serial.println( "以下の情報を設定しました。" );
        Serial.print( data1 );
        Serial.print( ":");
        Serial.println( dataMap->get( data1 ) );
               
      }else{
        Serial.print( data1 );
        Serial.println(":そのデータはありませんでした。");
      }
    }else{
      Serial.println("コマンドがおかしいようです。");
    }
  }else if( c == "get-m"){
    Serial.print("M = ");
    Serial.print( M );
    Serial.println( "." );

  }else{ //コマンドのelse文
    Serial.println("知らないコマンドのようです。");
  }
  
}

void readCommand(){
  
       String data1;
       if (Serial.available() > 0){
         data1 = Serial.readStringUntil( ENDCODE );

         char Buf[50];
         data1.toCharArray(Buf, 50);
         miniData = strtok( Buf, s);
         
         int n = 0;
         receiveMap->clear(); //receiveMapを初期化
         while( miniData != NULL ) {
            receiveMap->put( String(n) , miniData ); // 受信したデータをreceiveMapに格納
            miniData = strtok(NULL, s);
            n = n + 1;
         }
         Serial.println(" receiveMap : ");
         Serial.println( toJSON( receiveMap ) );       
       }
}





void bytes_to_hexstr(char *string, byte *bytes, int size) 
{ 
    for (int i = 0; i < size; i++) 
    { 
     string[i*2] = bytes[i] < 16 ? '0' : hex_char((bytes[i] >> 4) & 0xF); 
     string[i*2+1] = hex_char(bytes[i] & 0xF); 
    } 
    string[size*2] = 0; 
} 

bool compareString( String a, String b ){

  if( a == b ){
    return true;
  }else{
    return false;
  }
  
}

//map関数
void createDatamap(){
  
     dataMap = new SimpleMap<String, String>([](String& a, String& b) -> int {
        if (a == b) return 0;
        if (a > b) return 1;
        /*if (a < b) */ return -1;
    });
}

void createReceivemap(){
  
     receiveMap = new SimpleMap<String, String>([](String& a, String& b) -> int {
        if (a == b) return 0;
        if (a > b) return 1;
        /*if (a < b) */ return -1;
    });
}

String toJSON( SimpleMap<String, String>* mp ) {
    String s;

    s += '{';

    for (int i = 0; i < mp->size(); i++) {
        s += "\"" + mp->getKey(i) + "\":\"" + mp->getData(i) + "\"";
        if (i < mp->size() - 1){ 
          s += ',';
          s += '\n';
        }
    }
    s += "}";
    return s;
}

String boolTostring( boolean x ){
  
  if( x == true){
    return "TRUE";
  }else{
    return "FALSE";
  }

}
