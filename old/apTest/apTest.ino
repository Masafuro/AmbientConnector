#include <M5Atom.h>
#include <WiFi.h>       //WiFi接続用
#include <WebServer.h>  //サーバー設定用
#include "ESPAsyncWebServer.h"


// サーバー設定ポート80で接続
//WebServer server(80);
AsyncWebServer server(80);

// アクセスポイント設定
const char ssid_AP[] = "ESP23-WiFi";  //SSID
const char pass_AP[] = "esp32wifi";  //パスワード(8文字以上)

const IPAddress ip(192, 168, 20, 2);      // IPアドレス
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク



// htmlメイン画面データ（「"」は「\"」に置き換え、htmlの改行は「\n」、コードの改行は「\」）

char html2[] = "\
<!DOCTYPE html><html lang=\"ja\"><head> <meta charset=\"utf-8\"> <title>Ambient Connector</title> <meta name=\"description\" content=\"\"> <meta name=\"author\" content=\"古川企画\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <style type=\"text/css\"> html{font-family: sans-serif; -ms-text-size-adjust: 100%; -webkit-text-size-adjust: 100%;}body{margin: 0;}article, aside, details, figcaption, figure, footer, header, hgroup, main, menu, nav, section, summary{display: block;}audio, canvas, progress, video{display: inline-block; vertical-align: baseline;}audio:not([controls]){display: none; height: 0;}[hidden], template{display: none;}a{background-color: transparent;}a:active, a:hover{outline: 0;}abbr[title]{border-bottom: 1px dotted;}b, strong{font-weight: bold;}dfn{font-style: italic;}h1{font-size: 2em; margin: 0.67em 0;}mark{background: #ff0; color: #000;}small{font-size: 80%;}sub, sup{font-size: 75%; line-height: 0; position: relative; vertical-align: baseline;}sup{top: -0.5em;}sub{bottom: -0.25em;}img{border: 0;}svg:not(:root){overflow: hidden;}figure{margin: 1em 40px;}hr{-moz-box-sizing: content-box; box-sizing: content-box; height: 0;}pre{overflow: auto;}code, kbd, pre, samp{font-family: monospace, monospace; font-size: 1em;}button, input, optgroup, select, textarea{color: inherit; font: inherit; margin: 0;}button{overflow: visible;}button, select{text-transform: none;}button, html input[type=\"button\"], input[type=\"reset\"], input[type=\"submit\"]{-webkit-appearance: button; cursor: pointer;}button[disabled], html input[disabled]{cursor: default;}button::-moz-focus-inner, input::-moz-focus-inner{border: 0; padding: 0;}input{line-height: normal;}input[type=\"checkbox\"], input[type=\"radio\"]{box-sizing: border-box; padding: 0;}input[type=\"number\"]::-webkit-inner-spin-button, input[type=\"number\"]::-webkit-outer-spin-button{height: auto;}input[type=\"search\"]{-webkit-appearance: textfield; -moz-box-sizing: content-box; -webkit-box-sizing: content-box; box-sizing: content-box;}input[type=\"search\"]::-webkit-search-cancel-button, input[type=\"search\"]::-webkit-search-decoration{-webkit-appearance: none;}fieldset{border: 1px solid #c0c0c0; margin: 0 2px; padding: 0.35em 0.625em 0.75em;}legend{border: 0; padding: 0;}textarea{overflow: auto;}optgroup{font-weight: bold;}table{border-collapse: collapse; border-spacing: 0;}td, th{padding: 0;}.container{position: relative; width: 100%; max-width: 960px; margin: 0 auto; padding: 0 20px; box-sizing: border-box;}.column, .columns{width: 100%; float: left; box-sizing: border-box;}@media (min-width: 400px){.container{width: 85%; padding: 0;}}@media (min-width: 550px){.container{width: 80%;}.column, .columns{margin-left: 4%;}.column:first-child, .columns:first-child{margin-left: 0;}.one.column, .one.columns{width: 4.66666666667%;}.two.columns{width: 13.3333333333%;}.three.columns{width: 22%;}.four.columns{width: 30.6666666667%;}.five.columns{width: 39.3333333333%;}.six.columns{width: 48%;}.seven.columns{width: 56.6666666667%;}.eight.columns{width: 65.3333333333%;}.nine.columns{width: 74.0%;}.ten.columns{width: 82.6666666667%;}.eleven.columns{width: 91.3333333333%;}.twelve.columns{width: 100%; margin-left: 0;}.one-third.column{width: 30.6666666667%;}.two-thirds.column{width: 65.3333333333%;}.one-half.column{width: 48%;}.offset-by-one.column, .offset-by-one.columns{margin-left: 8.66666666667%;}.offset-by-two.column, .offset-by-two.columns{margin-left: 17.3333333333%;}.offset-by-three.column, .offset-by-three.columns{margin-left: 26%;}.offset-by-four.column, .offset-by-four.columns{margin-left: 34.6666666667%;}.offset-by-five.column, .offset-by-five.columns{margin-left: 43.3333333333%;}.offset-by-six.column, .offset-by-six.columns{margin-left: 52%;}.offset-by-seven.column, .offset-by-seven.columns{margin-left: 60.6666666667%;}.offset-by-eight.column, .offset-by-eight.columns{margin-left: 69.3333333333%;}.offset-by-nine.column, .offset-by-nine.columns{margin-left: 78.0%;}.offset-by-ten.column, .offset-by-ten.columns{margin-left: 86.6666666667%;}.offset-by-eleven.column, .offset-by-eleven.columns{margin-left: 95.3333333333%;}.offset-by-one-third.column, .offset-by-one-third.columns{margin-left: 34.6666666667%;}.offset-by-two-thirds.column, .offset-by-two-thirds.columns{margin-left: 69.3333333333%;}.offset-by-one-half.column, .offset-by-one-half.columns{margin-left: 52%;}}html{font-size: 62.5%;}body{font-size: 1.5em; line-height: 1.6; font-weight: 400; font-family: \"Raleway\", \"HelveticaNeue\", \"Helvetica Neue\", Helvetica, Arial, sans-serif; color: #222;}h1, h2, h3, h4, h5, h6{margin-top: 0; margin-bottom: 2rem; font-weight: 300;}h1{font-size: 4.0rem; line-height: 1.2; letter-spacing: -.1rem;}h2{font-size: 3.6rem; line-height: 1.25; letter-spacing: -.1rem;}h3{font-size: 3.0rem; line-height: 1.3; letter-spacing: -.1rem;}h4{font-size: 2.4rem; line-height: 1.35; letter-spacing: -.08rem;}h5{font-size: 1.8rem; line-height: 1.5; letter-spacing: -.05rem;}h6{font-size: 1.5rem; line-height: 1.6; letter-spacing: 0;}@media (min-width: 550px){h1{font-size: 5.0rem;}h2{font-size: 4.2rem;}h3{font-size: 3.6rem;}h4{font-size: 3.0rem;}h5{font-size: 2.4rem;}h6{font-size: 1.5rem;}}p{margin-top: 0;}a{color: #1EAEDB;}a:hover{color: #0FA0CE;}.button, button, input[type=\"submit\"], input[type=\"reset\"], input[type=\"button\"]{display: inline-block; height: 38px; padding: 0 30px; color: #555; text-align: center; font-size: 11px; font-weight: 600; line-height: 38px; letter-spacing: .1rem; text-transform: uppercase; text-decoration: none; white-space: nowrap; background-color: transparent; border-radius: 4px; border: 1px solid #bbb; cursor: pointer; box-sizing: border-box;}.button:hover, button:hover, input[type=\"submit\"]:hover, input[type=\"reset\"]:hover, input[type=\"button\"]:hover, .button:focus, button:focus, input[type=\"submit\"]:focus, input[type=\"reset\"]:focus, input[type=\"button\"]:focus{color: #333; border-color: #888; outline: 0;}.button.button-primary, button.button-primary, input[type=\"submit\"].button-primary, input[type=\"reset\"].button-primary, input[type=\"button\"].button-primary{color: #FFF; background-color: #33C3F0; border-color: #33C3F0;}.button.button-primary:hover, button.button-primary:hover, input[type=\"submit\"].button-primary:hover, input[type=\"reset\"].button-primary:hover, input[type=\"button\"].button-primary:hover, .button.button-primary:focus, button.button-primary:focus, input[type=\"submit\"].button-primary:focus, input[type=\"reset\"].button-primary:focus, input[type=\"button\"].button-primary:focus{color: #FFF; background-color: #1EAEDB; border-color: #1EAEDB;}input[type=\"email\"], input[type=\"number\"], input[type=\"search\"], input[type=\"text\"], input[type=\"tel\"], input[type=\"url\"], input[type=\"password\"], textarea, select{height: 38px; padding: 6px 10px; background-color: #fff; border: 1px solid #D1D1D1; border-radius: 4px; box-shadow: none; box-sizing: border-box;}input[type=\"email\"], input[type=\"number\"], input[type=\"search\"], input[type=\"text\"], input[type=\"tel\"], input[type=\"url\"], input[type=\"password\"], textarea{-webkit-appearance: none; -moz-appearance: none; appearance: none;}textarea{min-height: 65px; padding-top: 6px; padding-bottom: 6px;}input[type=\"email\"]:focus, input[type=\"number\"]:focus, input[type=\"search\"]:focus, input[type=\"text\"]:focus, input[type=\"tel\"]:focus, input[type=\"url\"]:focus, input[type=\"password\"]:focus, textarea:focus, select:focus{border: 1px solid #33C3F0; outline: 0;}label, legend{display: block; margin-bottom: .5rem; font-weight: 600;}fieldset{padding: 0; border-width: 0;}input[type=\"checkbox\"], input[type=\"radio\"]{display: inline;}label > .label-body{display: inline-block; margin-left: .5rem; font-weight: normal;}ul{list-style: circle inside;}ol{list-style: decimal inside;}ol, ul{padding-left: 0; margin-top: 0;}ul ul, ul ol, ol ol, ol ul{margin: 1.5rem 0 1.5rem 3rem; font-size: 90%;}li{margin-bottom: 1rem;}code{padding: .2rem .5rem; margin: 0 .2rem; font-size: 90%; white-space: nowrap; background: #F1F1F1; border: 1px solid #E1E1E1; border-radius: 4px;}pre > code{display: block; padding: 1rem 1.5rem; white-space: pre;}th, td{padding: 12px 15px; text-align: left; border-bottom: 1px solid #E1E1E1;}th:first-child, td:first-child{padding-left: 0;}th:last-child, td:last-child{padding-right: 0;}button, .button{margin-bottom: 1rem;}input, textarea, select, fieldset{margin-bottom: 1.5rem;}pre, blockquote, dl, figure, table, p, ul, ol, form{margin-bottom: 2.5rem;}.u-full-width{width: 100%; box-sizing: border-box;}.u-max-full-width{max-width: 100%; box-sizing: border-box;}.u-pull-right{float: right;}.u-pull-left{float: left;}hr{margin-top: 3rem; margin-bottom: 3.5rem; border-width: 0; border-top: 1px solid #E1E1E1;}.container:after, .row:after, .u-cf{content: \"\"; display: table; clear: both;}@media (min-width: 400px){}@media (min-width: 550px){}@media (min-width: 750px){}@media (min-width: 1000px){}@media (min-width: 1200px){}</style></head><body> <div class=\"container\"> <div class=\"row\"> <div class=\"one-half column\" style=\"margin-top: 5%\"> <h1>Ambient_Connector</h1> </div></div><div class=\"row\"> <div class=\"four columns\"> <label for=\"exampleEmailInput\">Wifi名</label> <input class=\"u-full-width\" type=\"email\" placeholder=\"xxxxxxx\" id=\"exampleEmailInput\"> </div><div class=\"four columns\"> <label for=\"exampleEmailInput\">パスワード</label> <input class=\"u-full-width\" type=\"email\" placeholder=\"xxxxxxxx\" id=\"exampleEmailInput\"> </div></div><div class=\"row\"> <div class=\"four columns\"> <label for=\"exampleEmailInput\">チャネルID</label> <input class=\"u-full-width\" type=\"email\" placeholder=\"xxxxxxx\" id=\"exampleEmailInput\"> </div><div class=\"four columns\"> <label for=\"exampleEmailInput\">リードキー</label> <input class=\"u-full-width\" type=\"email\" placeholder=\"xxxxxxxx\" id=\"exampleEmailInput\"> </div><div class=\"four columns\"> <label for=\"exampleEmailInput\">ライトキー</label> <input class=\"u-full-width\" type=\"email\" placeholder=\"xxxxxxxx\" id=\"exampleEmailInput\"> </div></div><div class=\"row\"> <div class=\"four columns\"> <label for=\"exampleRecipientInput\">送信タイプ</label> <select class=\"u-full-width\" id=\"exampleRecipientInput\"> <option value=\"Option 1\">周期実行</option> <option value=\"Option 2\">トリガー実行</option> </select> </div><div class=\"four columns\"> <label for=\"exampleEmailInput\">実行周期(ms)</label> <input class=\"u-full-width\" type=\"email\" placeholder=\"xxxxxxxx\" id=\"exampleEmailInput\"> </div></div><input class=\"button-primary\" type=\"submit\" value=\"送信！！\"> <div class=\"row\"> <div class=\"twelve column\"> <p> developed by <a href=\"https://furukawakikaku.jp\">古川企画</a>.</p></div></div></div></body></html>";

/*
char html3[] = "\
<!DOCTYPE html><html lang=\"ja\"><head> <meta charset=\"utf-8\"> <title>Your page title here :)</title> <meta name=\"description\" content=\"\"> <meta name=\"author\" content=\"\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <style type=\"text/css\"> html{font-family: sans-serif; -ms-text-size-adjust: 100%; -webkit-text-size-adjust: 100%;}body{margin: 0;}article, aside, details, figcaption, figure, footer, header, hgroup, main, menu, nav, section, summary{display: block;}audio, canvas, progress, video{display: inline-block; vertical-align: baseline;}audio:not([controls]){display: none; height: 0;}[hidden], template{display: none;}a{background-color: transparent;}a:active, a:hover{outline: 0;}abbr[title]{border-bottom: 1px dotted;}b, strong{font-weight: bold;}dfn{font-style: italic;}h1{font-size: 2em; margin: 0.67em 0;}mark{background: #ff0; color: #000;}small{font-size: 80%;}sub, sup{font-size: 75%; line-height: 0; position: relative; vertical-align: baseline;}sup{top: -0.5em;}sub{bottom: -0.25em;}img{border: 0;}svg:not(:root){overflow: hidden;}figure{margin: 1em 40px;}hr{-moz-box-sizing: content-box; box-sizing: content-box; height: 0;}pre{overflow: auto;}code, kbd, pre, samp{font-family: monospace, monospace; font-size: 1em;}button, input, optgroup, select, textarea{color: inherit; font: inherit; margin: 0;}button{overflow: visible;}button, select{text-transform: none;}button, html input[type=\"button\"], input[type=\"reset\"], input[type=\"submit\"]{-webkit-appearance: button; cursor: pointer;}button[disabled], html input[disabled]{cursor: default;}button::-moz-focus-inner, input::-moz-focus-inner{border: 0; padding: 0;}input{line-height: normal;}input[type=\"checkbox\"], input[type=\"radio\"]{box-sizing: border-box; padding: 0;}input[type=\"number\"]::-webkit-inner-spin-button, input[type=\"number\"]::-webkit-outer-spin-button{height: auto;}input[type=\"search\"]{-webkit-appearance: textfield; -moz-box-sizing: content-box; -webkit-box-sizing: content-box; box-sizing: content-box;}input[type=\"search\"]::-webkit-search-cancel-button, input[type=\"search\"]::-webkit-search-decoration{-webkit-appearance: none;}fieldset{border: 1px solid #c0c0c0; margin: 0 2px; padding: 0.35em 0.625em 0.75em;}legend{border: 0; padding: 0;}textarea{overflow: auto;}optgroup{font-weight: bold;}table{border-collapse: collapse; border-spacing: 0;}td, th{padding: 0;}.container{position: relative; width: 100%; max-width: 960px; margin: 0 auto; padding: 0 20px; box-sizing: border-box;}.column, .columns{width: 100%; float: left; box-sizing: border-box;}@media (min-width: 400px){.container{width: 85%; padding: 0;}}@media (min-width: 550px){.container{width: 80%;}.column, .columns{margin-left: 4%;}.column:first-child, .columns:first-child{margin-left: 0;}.one.column, .one.columns{width: 4.66666666667%;}.two.columns{width: 13.3333333333%;}.three.columns{width: 22%;}.four.columns{width: 30.6666666667%;}.five.columns{width: 39.3333333333%;}.six.columns{width: 48%;}.seven.columns{width: 56.6666666667%;}.eight.columns{width: 65.3333333333%;}.nine.columns{width: 74.0%;}.ten.columns{width: 82.6666666667%;}.eleven.columns{width: 91.3333333333%;}.twelve.columns{width: 100%; margin-left: 0;}.one-third.column{width: 30.6666666667%;}.two-thirds.column{width: 65.3333333333%;}.one-half.column{width: 48%;}.offset-by-one.column, .offset-by-one.columns{margin-left: 8.66666666667%;}.offset-by-two.column, .offset-by-two.columns{margin-left: 17.3333333333%;}.offset-by-three.column, .offset-by-three.columns{margin-left: 26%;}.offset-by-four.column, .offset-by-four.columns{margin-left: 34.6666666667%;}.offset-by-five.column, .offset-by-five.columns{margin-left: 43.3333333333%;}.offset-by-six.column, .offset-by-six.columns{margin-left: 52%;}.offset-by-seven.column, .offset-by-seven.columns{margin-left: 60.6666666667%;}.offset-by-eight.column, .offset-by-eight.columns{margin-left: 69.3333333333%;}.offset-by-nine.column, .offset-by-nine.columns{margin-left: 78.0%;}.offset-by-ten.column, .offset-by-ten.columns{margin-left: 86.6666666667%;}.offset-by-eleven.column, .offset-by-eleven.columns{margin-left: 95.3333333333%;}.offset-by-one-third.column, .offset-by-one-third.columns{margin-left: 34.6666666667%;}.offset-by-two-thirds.column, .offset-by-two-thirds.columns{margin-left: 69.3333333333%;}.offset-by-one-half.column, .offset-by-one-half.columns{margin-left: 52%;}}html{font-size: 62.5%;}body{font-size: 1.5em; line-height: 1.6; font-weight: 400; font-family: \"Raleway\", \"HelveticaNeue\", \"Helvetica Neue\", Helvetica, Arial, sans-serif; color: #222;}h1, h2, h3, h4, h5, h6{margin-top: 0; margin-bottom: 2rem; font-weight: 300;}h1{font-size: 4.0rem; line-height: 1.2; letter-spacing: -.1rem;}h2{font-size: 3.6rem; line-height: 1.25; letter-spacing: -.1rem;}h3{font-size: 3.0rem; line-height: 1.3; letter-spacing: -.1rem;}h4{font-size: 2.4rem; line-height: 1.35; letter-spacing: -.08rem;}h5{font-size: 1.8rem; line-height: 1.5; letter-spacing: -.05rem;}h6{font-size: 1.5rem; line-height: 1.6; letter-spacing: 0;}@media (min-width: 550px){h1{font-size: 5.0rem;}h2{font-size: 4.2rem;}h3{font-size: 3.6rem;}h4{font-size: 3.0rem;}h5{font-size: 2.4rem;}h6{font-size: 1.5rem;}}p{margin-top: 0;}a{color: #1EAEDB;}a:hover{color: #0FA0CE;}.button, button, input[type=\"submit\"], input[type=\"reset\"], input[type=\"button\"]{display: inline-block; height: 38px; padding: 0 30px; color: #555; text-align: center; font-size: 11px; font-weight: 600; line-height: 38px; letter-spacing: .1rem; text-transform: uppercase; text-decoration: none; white-space: nowrap; background-color: transparent; border-radius: 4px; border: 1px solid #bbb; cursor: pointer; box-sizing: border-box;}.button:hover, button:hover, input[type=\"submit\"]:hover, input[type=\"reset\"]:hover, input[type=\"button\"]:hover, .button:focus, button:focus, input[type=\"submit\"]:focus, input[type=\"reset\"]:focus, input[type=\"button\"]:focus{color: #333; border-color: #888; outline: 0;}.button.button-primary, button.button-primary, input[type=\"submit\"].button-primary, input[type=\"reset\"].button-primary, input[type=\"button\"].button-primary{color: #FFF; background-color: #33C3F0; border-color: #33C3F0;}.button.button-primary:hover, button.button-primary:hover, input[type=\"submit\"].button-primary:hover, input[type=\"reset\"].button-primary:hover, input[type=\"button\"].button-primary:hover, .button.button-primary:focus, button.button-primary:focus, input[type=\"submit\"].button-primary:focus, input[type=\"reset\"].button-primary:focus, input[type=\"button\"].button-primary:focus{color: #FFF; background-color: #1EAEDB; border-color: #1EAEDB;}input[type=\"email\"], input[type=\"number\"], input[type=\"search\"], input[type=\"text\"], input[type=\"tel\"], input[type=\"url\"], input[type=\"password\"], textarea, select{height: 38px; padding: 6px 10px; background-color: #fff; border: 1px solid #D1D1D1; border-radius: 4px; box-shadow: none; box-sizing: border-box;}input[type=\"email\"], input[type=\"number\"], input[type=\"search\"], input[type=\"text\"], input[type=\"tel\"], input[type=\"url\"], input[type=\"password\"], textarea{-webkit-appearance: none; -moz-appearance: none; appearance: none;}textarea{min-height: 65px; padding-top: 6px; padding-bottom: 6px;}input[type=\"email\"]:focus, input[type=\"number\"]:focus, input[type=\"search\"]:focus, input[type=\"text\"]:focus, input[type=\"tel\"]:focus, input[type=\"url\"]:focus, input[type=\"password\"]:focus, textarea:focus, select:focus{border: 1px solid #33C3F0; outline: 0;}label, legend{display: block; margin-bottom: .5rem; font-weight: 600;}fieldset{padding: 0; border-width: 0;}input[type=\"checkbox\"], input[type=\"radio\"]{display: inline;}label > .label-body{display: inline-block; margin-left: .5rem; font-weight: normal;}ul{list-style: circle inside;}ol{list-style: decimal inside;}ol, ul{padding-left: 0; margin-top: 0;}ul ul, ul ol, ol ol, ol ul{margin: 1.5rem 0 1.5rem 3rem; font-size: 90%;}li{margin-bottom: 1rem;}code{padding: .2rem .5rem; margin: 0 .2rem; font-size: 90%; white-space: nowrap; background: #F1F1F1; border: 1px solid #E1E1E1; border-radius: 4px;}pre > code{display: block; padding: 1rem 1.5rem; white-space: pre;}th, td{padding: 12px 15px; text-align: left; border-bottom: 1px solid #E1E1E1;}th:first-child, td:first-child{padding-left: 0;}th:last-child, td:last-child{padding-right: 0;}button, .button{margin-bottom: 1rem;}input, textarea, select, fieldset{margin-bottom: 1.5rem;}pre, blockquote, dl, figure, table, p, ul, ol, form{margin-bottom: 2.5rem;}.u-full-width{width: 100%; box-sizing: border-box;}.u-max-full-width{max-width: 100%; box-sizing: border-box;}.u-pull-right{float: right;}.u-pull-left{float: left;}hr{margin-top: 3rem; margin-bottom: 3.5rem; border-width: 0; border-top: 1px solid #E1E1E1;}.container:after, .row:after, .u-cf{content: \"\"; display: table; clear: both;}@media (min-width: 400px){}@media (min-width: 550px){}@media (min-width: 750px){}@media (min-width: 1000px){}@media (min-width: 1200px){}</style></head><body> <div class=\"container\"> <div class=\"row\"> <div class=\"one-half column\" style=\"margin-top: 25%\"> <h4>Basic Page</h4> <p>This index.html page is a placeholder with the CSS, font and favicon. It's just waiting for you to add some content! If you need some help hit up the <a href=\"http://www.getskeleton.com\">Skeleton documentation</a>.</p></div></div></div></body></html>";

char html[] = "\
<!DOCTYPE html><html lang=\"jp\"><head>\n\
<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
<title>REMOTE-SWITCH</title>\n\kenn
</head><body>\n\
  <h1>REMOTE-SWITCH</h1>\n\
  <div>\n\
    <button style=\"height:50px\" onclick=\"location.href='/get/btn_on'\">　ON　</button>\n\
    <button style=\"height:50px\" onclick=\"location.href='/get/btn_off'\">　OFF　</button>\n\
  </div>\n\
</body></html>\n";
*/


// FastLED（CRGB構造体）設定
CRGB dispColor(uint8_t r, uint8_t g, uint8_t b) {
  return (CRGB)((r << 16) | (g << 8) | b);
}
//-------------------------------------------------
// サーバーリクエスト時処理関数
//-------------------------------------------------
// ルートアクセス時の応答関数
void handleRoot() {
  server.send(200, "text/html", html2);  //レスポンス200を返しhtml送信
}
// エラー（Webページが見つからない）時の応答関数
void handleNotFound() {
  server.send(404, "text/plain", "404 Not Found!");  //text送信
}
// ブラウザONボタン処理
void btnOn() {
  digitalWrite(21, HIGH);                   //出力0 ON（青色LED）
  M5.dis.drawpix(0, dispColor(0, 0, 200));  //本体LED（青）
  server.send(200, "text/html", html2);  //レスポンス200を返しhtml送信
}
// ブラウザOFFボタン処理
void btnOff() {
  digitalWrite(21, LOW);                    //出力0 OFF（青色LED）
  M5.dis.drawpix(0, dispColor(20, 20, 20)); //本体LED（白）
  server.send(200, "text/html", html2);  //レスポンス200を返しhtml送信
}
//-------------------------------------------------
// 初期設定
//-------------------------------------------------
void setup() {
  M5.begin(true, false, true);    //Serial,POWER,LED
  Serial.begin(9600);             //標準のシリアル通信設定

  // アクセスポイント設定
  WiFi.softAP(ssid_AP, pass_AP);  //ソフトAP設定
  delay(100);
  WiFi.softAPConfig(ip, ip, subnet); // IPアドレス、ゲートウェイ、サブネットマスクの設定


  // サーバー設定
  server.on("/", handleRoot);         //ルートアクセス時の応答関数を設定
  server.onNotFound(handleNotFound);  //Webページが見つからない時の応答関数を設定
  server.on("/get/btn_on", btnOn);    //ボタンオン受信処理 リンクで/get/btn_onに飛ぶとbtnON関数が実行される。
  server.on("/get/btn_off", btnOff);  //ボタンオフ受信処理
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
 
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
    request->send(200, "text/plain", "message received");
  });
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
