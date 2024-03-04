#include <M5Unified.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;
#include <SoftwareSerial.h>
// バッファサイズの定義
// バッファサイズの定義
#define BUFFER_SIZE 64

// バッファの宣言
char serialBuffer[BUFFER_SIZE];
int bufferIndex = 0;
SoftwareSerial mySerial(G2, G1); // G1をRXピン、G2をTXピンとしてソフトウェアシリアルを初期化

const int buttonPin = 41;
int previousButtonState = HIGH;
int counter = 0;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  mySerial.begin(9600);
  
  pinMode(buttonPin, INPUT_PULLUP);
  Keyboard.begin();


  USB.begin();


}



void loop() {

  while (mySerial.available() > 0) {
    char receivedChar = mySerial.read();
    // バッファにデータを追加
    if (bufferIndex < BUFFER_SIZE - 1) { // バッファオーバーフローを防ぐためのチェック
      serialBuffer[bufferIndex++] = receivedChar;
    }
    // 終端文字が来た場合、データを処理
    if (receivedChar == '\n' || bufferIndex >= BUFFER_SIZE - 1) {

      // 終端文字を追加
      serialBuffer[bufferIndex] = '\0';
      // キーコードとmodifierを取得
      char *separator = strstr(serialBuffer, "-");
// セパレータが見つかった場合
// セパレータが見つかった場合
if (separator != NULL) {
    // セパレータの直前までの文字列をキーコードとして取得します
    int keycode = 0;
    for (char *ptr = serialBuffer; ptr != separator; ++ptr) {
        if (*ptr >= '0' && *ptr <= '9') {
            keycode = keycode * 10 + (*ptr - '0');
        } else {
            // 数字以外の文字が見つかった場合はエラー処理を行います
            // ここでは単に処理を中断し、エラーメッセージを出力します
            Serial.println("Error: Non-numeric character found in keycode");
            return; // キーコードの処理を中断します
        }
    }
    // modifier部分を取得します
    int modifier = atoi(separator + 1); // separatorの次の位置からmodifierを数値に変換します

    // キーボードにエミュレートされたキーを押します
 // セパレータが見つかった場合
if (separator != NULL) {
    // セパレータの直前までの文字列をキーコードとして取得します
    int keycode = 0;
    for (char *ptr = serialBuffer; ptr != separator; ++ptr) {
        if (*ptr >= '0' && *ptr <= '9') {
            keycode = keycode * 10 + (*ptr - '0');
        } else {
            // 数字以外の文字が見つかった場合はエラー処理を行います
            // ここでは単に処理を中断し、エラーメッセージを出力します
            Serial.println("Error: Non-numeric character found in keycode");
            return; // キーコードの処理を中断します
        }
    }

    // modifier部分を取得します
    int modifier = atoi(separator + 1); // separatorの次の位置からmodifierを数値に変換します

    // キーボードにエミュレートされQたキーを押します
    Keyboard.pressRaw(keycode);
    Keyboard.releaseAll(); // すべてのキーの押下を解除します
    // デバッグ用にシリアルモニタに出力します
}
    delay(30);
    Keyboard.releaseAll(); // すべてのキーの押下を解除します
    // デバッグ用にシリアルモニタに出力します
}
      // バッファをクリア
      bufferIndex = 0;
    }
  }

  int buttonState = digitalRead(buttonPin);
  if ((buttonState != previousButtonState) && (buttonState == LOW)) {
    counter++;
    Keyboard.print("You pressed the button ");
    Keyboard.print(counter);
    Keyboard.println(" times.");
  }
  previousButtonState = buttonState;
  delay(1);
}