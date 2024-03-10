#include <M5Unified.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;
#include <SoftwareSerial.h>

#define BUFFER_SIZE 64

char serialBuffer[BUFFER_SIZE];
int bufferIndex = 0;
SoftwareSerial mySerial(G2, G1);

const int buttonPin = 41;
int previousButtonState = HIGH;
int counter = 0;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  mySerial.begin(38400);

  pinMode(buttonPin, INPUT_PULLUP);
  Keyboard.begin();
  USB.begin();
}

void loop() {
  while (mySerial.available() > 0) {
    char receivedChar = mySerial.read();

    if (bufferIndex < BUFFER_SIZE - 1) {
      serialBuffer[bufferIndex++] = receivedChar;
    }

    if (receivedChar == '\n' || bufferIndex >= BUFFER_SIZE - 1) {
      serialBuffer[bufferIndex] = '\0';

      char *separator = strstr(serialBuffer, "-");

      if (separator != NULL) {
        int keycode = 0;
        for (char *ptr = serialBuffer; ptr != separator; ++ptr) {
          if (*ptr >= '0' && *ptr <= '9') {
            keycode = keycode * 10 + (*ptr - '0');
          } else {
            Serial.println("Error: Non-numeric character found in keycode");
            return;
          }
        }

        int modifier = atoi(separator + 1);

//30 - 3  Ctrl Shift 1
if (keycode == 30 && modifier == 3 ) {
  Keyboard.press(35); //日本語変換キー
  bufferIndex = 0;
  delay(4);
  Keyboard.release(35);
  return ;
}

        if (modifier & 1) {
          Keyboard.press(KEY_LEFT_CTRL);
        }
        if (modifier & (1 << 1)) {
          // 右から2番目のビットが1の場合の処理
          Keyboard.press(KEY_LEFT_SHIFT);
        }

        if (modifier & (1 << 5)) {
        // 右から5番目のビットが1の場合の処理
        // ここに適切な処理を記述する
              Keyboard.press(KEY_RIGHT_SHIFT);
        }
        if (keycode != 0) {
          delay(4);
          Keyboard.pressRaw(keycode);
          Keyboard.releaseAll();
        }

        if (modifier & 1) {
          Keyboard.release(KEY_LEFT_CTRL);
        }

        if (modifier & (1 << 1)) {
          // 右から2番目のビットが1の場合の処理
          Keyboard.release(KEY_LEFT_SHIFT);
        }

      if (modifier & (1 << 5)) {
      // 右から5番目のビットが1の場合の処理3 times.
      
      // ここに適切な処理を記述する
        Keyboard.release(KEY_RIGHT_SHIFT);
       }

      }
      bufferIndex = 0;
    }
  }
  int buttonState = digitalRead(buttonPin);
  if ((buttonState != previousButtonState) && (buttonState == LOW)) {
    counter++;
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('`');
      delay(330);
           Keyboard.releaseAll();
    Keyboard.print(counter);
    Keyboard.println(" times.");
  }
  previousButtonState = buttonState;
  delay(1);  // チャタリング対策のため、20ミリ秒のディレイを挿入
}
