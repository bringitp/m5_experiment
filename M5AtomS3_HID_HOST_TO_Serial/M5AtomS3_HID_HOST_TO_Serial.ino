#include <M5Unified.h>
#include "EspUsbHost.h"
#include <SoftwareSerial.h>
#include <U8g2lib.h>
int count = 0;
SoftwareSerial mySerial(G1, G2); // ソフトウェアシリアルの初期化

int preKey = 0; // 前回のキーコード
int nowKey = 0; // 現在のキーコード
int modifiers = 0; // 修飾キー
unsigned long preTime = 0; // 前回のイベント時間
const int EVENT_REPEAT_THRESHOLD = 320; // イベントのリピート閾値
const int EVENT_2nd_REPEAT_THRESHOLD = 1200; // 2回目のリピート閾値


// 四角形の中心座標
int centerX = 64;
int centerY = 64;
// 四角形のサイズ
int rectWidth = 50;
int rectHeight = 50;
// 角度 (ラジアン)
float angle = 0;
// 回転速度 (ラジアン/秒)
float rotationSpeed = 0.1;


class MyEspUsbHost : public EspUsbHost {
  void onKeyboard(hid_keyboard_report_t report, hid_keyboard_report_t last_report) {
    M5.Display.startWrite();
    M5.Display.fillScreen(TFT_BLACK); // ディスプレイをクリア
    ++count;

    int get_char = 0;
    if (report.keycode[0] != 0 && last_report.keycode[0] == 0) {
        get_char = report.keycode[0];
    } else if (report.keycode[0] != 0 && last_report.keycode[0] != 0 && report.keycode[2] == 0 ) {
        get_char = report.keycode[1];
    } else if (report.keycode[0] != 0 && report.keycode[1] != 0 && report.keycode[2] != 0 && last_report.keycode[0] != 0 && last_report.keycode[1] != 0 ) {
        get_char = report.keycode[2];
    }

    unsigned long currentTime = millis();

    
    nowKey = get_char;
    preKey = last_report.keycode[0]; // 前回のキーコードを保存

    // 特殊キーは連打しないためカウントしない
    if ( nowKey != 0 ) {
      preTime = currentTime; // 前回のイベント時間を更新
    }
    modifiers = report.modifier ;

    M5.Display.startWrite();
    M5.Display.setCursor(0, 0); // カーソルを先頭に移動
   // M5.Display.printf("                            "); // 以前の内容と同じ幅のスペース
    //M5.Display.fillScreen(TFT_BLACK); // ディスプレイを黒で塗りつぶす
    M5.Display.setCursor(0, 0); // カーソルを先頭に移動 
    M5.Display.endWrite();


    if (0) {
    // デバッグ情報を出力
    M5.Display.print(get_char);
    M5.Display.print("-");
    M5.Display.print(modifiers);
    M5.Display.print("-");
    M5.Display.println(count);
    }

    mySerial.print(nowKey);
    mySerial.print("-");
    mySerial.println(modifiers);
  }
};

MyEspUsbHost usbHost;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  mySerial.begin(38400);

  //M5.Display.setTextSize(3);
  M5.Display.setBrightness(50); // ディスプレイの明るさを設定
  M5.Display.setTextColor(TFT_ORANGE); // テキストの色を設定
  M5.Display.startWrite();
  M5.Display.setFont(&fonts::lgfxJapanGothic_12);
  //M5.Display.setFont(u8g2_font_cu12_t_cyrillic);
  
  
  M5.Display.println("Кто сказал, что мы плохо жили..? ");
  M5.Display.println("石炭をば早はや積み果てつ。中等室の卓のほとりはいと静にて、熾熱燈の光の晴れがましきも徒なり ");
  
  // 矩形を描画
  int rectWidth = 50;
  int rectHeight = 50;
  int rectX = 39;
  int rectY = 50;
  //M5.Display.drawRect(rectX, rectY, rectWidth, rectHeight, TFT_WHITE);

  M5.Display.endWrite();

  usbHost.begin();
  usbHost.setHIDLocal(HID_LOCAL_Japan_Katakana);
}

void loop() {
  M5.update();
  usbHost.task(); 
  


  // 新しい四角形の座標を計算
  int rectX = centerX + (int)(cos(angle) * (float)rectWidth / 2.0);
  int rectY = centerY + (int)(sin(angle) * (float)rectHeight / 2.0);



  // 角度を更新
  angle += rotationSpeed * (float)millis() / 1000.0;
  if (angle >= 2 * PI) {
    angle -= 2 * PI;
  }
  
  M5.Display.startWrite();
  // 画面をクリア
  //M5.Display.fillScreen(TFT_BLACK);
  // 四角形を描画
  //M5.Display.drawRect(rectX - rectWidth / 2, rectY - rectHeight / 2, rectWidth, rectHeight, TFT_WHITE);
  M5.Display.endWrite();



  unsigned long currentTime = millis();
  if (nowKey != 0 && currentTime - preTime >= EVENT_REPEAT_THRESHOLD) {  
    if (nowKey != 0 ) {
      mySerial.print(nowKey);
      mySerial.print("-");
      mySerial.println(modifiers);    
    }
    
    // 連打間隔を調整
    if (currentTime - preTime >= EVENT_REPEAT_THRESHOLD && currentTime - preTime < EVENT_2nd_REPEAT_THRESHOLD) { 
      delay(32);
    }
    if (currentTime - preTime > EVENT_2nd_REPEAT_THRESHOLD) { 
      delay(8);
    }
  }
  
  delay(1); // チャタリング対策のためのディレイ
}
