#include <M5Unified.h>
#include "EspUsbHost.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(G1, G2); // G1をRXピン、G2をTXピンとしてソフトウェアシリアルを初期化

class MyEspUsbHost : public EspUsbHost {
  void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier) {
    M5.Display.startWrite();
    // 以前の内容を上書きするために、同じ幅のスペースで埋める
    M5.Display.setCursor(0, 0); // カーソルを先頭に移動
    M5.Display.printf("          "); // 以前の内容と同じ幅のスペース
    M5.Display.setCursor(0, 0); // カーソルを先頭に移動
    M5.Display.print(keycode); // 以前の内容と同じ幅のスペース
    M5.Display.print("-");
    M5.Display.println(modifier); // 以前の内容と同じ幅のスペース

    mySerial.print(keycode);
    mySerial.print("-");
    mySerial.println(modifier);
    
  }
};
MyEspUsbHost usbHost;

void setup(void) {
  auto cfg = M5.config();
  M5.begin(cfg);
    mySerial.begin(9600);

  M5.Display.setTextSize(4);

  M5.Display.startWrite();
  M5.Display.print("ON!:");
  M5.Display.endWrite();

  usbHost.begin();
  usbHost.setHIDLocal(HID_LOCAL_Japan_Katakana);
}

void loop(void) {

  M5.update();
  M5.Display.startWrite();
  delay(10);
  usbHost.task();
  M5.Display.endWrite();
  M5.Display.display();
}