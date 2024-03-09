#include <M5Unified.h>
#include "EspUsbHost.h"
#include <SoftwareSerial.h>
int count = 0;
SoftwareSerial mySerial(G1, G2); // G1をRXピン、G2をTXピンとしてソフトウェアシリアルを初期化

int preKey = 0; // 前回のキーコードを保存する変数
int nowKey = 0;
char modifiers = 0;
unsigned long preTime = 0; // 前回のキーコードが変化した時間を保存する変数
const int EVENT_THRESHOLD = 400; // イベントをトリガーするための閾値

hid_keyboard_report_t report;
hid_keyboard_report_t last_report;

class MyEspUsbHost : public EspUsbHost {
  void onKeyboard(hid_keyboard_report_t in_report, hid_keyboard_report_t in_last_report) {
    M5.Display.startWrite();
    // 以前の内容を上書きするために、同じ幅のスペースで埋める
    M5.Display.setCursor(0, 0); // カーソルを先頭に移動
    M5.Display.printf("                     "); // 以前の内容と同じ幅のスペース
    M5.Display.setCursor(0, 0); // カーソルを先頭に移動 ss
  ++count;




   memcpy(&report, &in_report, sizeof(report));
   memcpy(&last_report, &in_last_report, sizeof(last_report));
          modifiers |= (in_report.modifier & KEYBOARD_MODIFIER_LEFTSHIFT) ? (1 << 0) : 0;
          modifiers |= (in_report.modifier & KEYBOARD_MODIFIER_LEFTCTRL) ? (1 << 1) : 0;
          modifiers |= (in_report.modifier & KEYBOARD_MODIFIER_LEFTALT) ? (1 << 2) : 0;
          modifiers |= (in_report.modifier & KEYBOARD_MODIFIER_LEFTGUI ) ? (1 << 3) : 0;
          modifiers |= (in_report.modifier & KEYBOARD_MODIFIER_RIGHTSHIFT)? (1 << 4) : 0;
          modifiers |= (in_report.modifier & KEYBOARD_MODIFIER_RIGHTCTRL) ? (1 << 5) : 0;
          modifiers |= (in_report.modifier & KEYBOARD_MODIFIER_RIGHTALT) ? (1 << 6) : 0;
          modifiers |= (in_report.modifier & KEYBOARD_MODIFIER_RIGHTGUI ) ? (1 << 7) : 0;

// onKeyboardKey メソッド内でフラグをリセットする
//if (isKeyPressed) {
//    usbHost->onKeyboardKey(usbHost->getKeycodeToAscii(keyCodePressed, shift), keyCodePressed, modifiers);
//    isKeyPressed = false; // キーが処理された後にフラグをリセットする
//                        report       last report
///        report       0 0 0 0 0 0     0 0 0 0 0 0
//  type      a         a 0 0 0 0 0     0 0 0 0 0 0
//  release   a         0  0 0 0 0 0    a 0 0 0 0 0
///        report       0 0 0 0 0 0     0 0 0 0 0 0
//  type a              a 0 0 0 0 0     0 0 0 0 0 0
//  type a + b          a b 0 0 0 0    a 0 0 0 0 0 
//  release a           b 0 0 0 0 0     a b 0 0 0 0
//  release b           0 0 0 0 0 0     b 0 0 0 0 0 
//  type a              a 0 0 0 0 0     0 0 0 0 0 0
//  type a + b          a b 0 0 0 0    a 0 0 0 0 0 
//  type a + b  +c      a b c 0 0 0    a b 0 0 0 0 

    int get_char = 0;
    if (report.keycode[0] != 0 && last_report.keycode[0] == 0) {
        get_char = report.keycode[0];
    } else if (report.keycode[0] != 0 && last_report.keycode[0] != 0 && report.keycode[2] == 0 ) {
        get_char = report.keycode[1];
    } else if (report.keycode[0] != 0 && last_report.keycode[0] != 0 && report.keycode[2] != 0 ) {
        get_char = report.keycode[2]; // ??? 
    } else {

    }

    M5.Display.print(get_char); // 以前の内容と同じ幅のスペース
    M5.Display.print("-");
    M5.Display.print(modifiers); // 以前の内容と同じ幅のスペース
    M5.Display.print("-");
    M5.Display.println(count); // 以前の内容と同じ幅のスペースs

    unsigned long currentTime = millis();

    M5.Display.endWrite();
    nowKey = get_char;
    preKey = in_last_report.keycode[0]; // 現在のキーコードを保存
    preTime = currentTime; // 現在の時間を保存


     mySerial.print(nowKey);
     mySerial.print("-");
     mySerial.println(modifiers);    
     
  }
};
MyEspUsbHost usbHost;

void setup(void) {
  auto cfg = M5.config();
  M5.begin(cfg);
    mySerial.begin(38400);

  M5.Display.setTextSize(2);
 // ディスプレイの明るさを50に設定
  M5.Display.setBrightness(30);
  M5.Display.startWrite();
  M5.Display.print("ON!:");
  M5.Display.endWrite();

  usbHost.begin();
  usbHost.setHIDLocal(HID_LOCAL_Japan_Katakana);
}


void loop(void) {

  M5.update();
  usbHost.task(); 
  
  unsigned long currentTime = millis();
  if (nowKey != 0 && currentTime - preTime >= EVENT_THRESHOLD) {

  
   if (nowKey != 0 ) {
     mySerial.print(nowKey);
     mySerial.print("-");
     mySerial.println(modifiers);    
   }

  
   // 70ミリ秒程度で連打していく
    delay(70);
  }
  

  delay(1); // チャタリング対策のため、20ミリ秒のディレイを挿入
}