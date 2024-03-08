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

                if (modifier & 1) {
                    Keyboard.press(KEY_LEFT_SHIFT);
                }

                if (modifier & (1 << 1)) {
        // 右から2番目のビットが1の場合の処理
                    Keyboard.press(KEY_LEFT_CTRL);
                } 
                if (keycode != 0) {
                    delay(50);
                    Keyboard.pressRaw(keycode);
                    Keyboard.releaseAll();
                } else {
                    delay(50);
                    Keyboard.releaseAll();
                    bufferIndex = 0;
                }

                if (modifier & 1) {
                    Keyboard.release(KEY_LEFT_SHIFT);
                }
            }

            bufferIndex = 0;
        }
    }

    int buttonState = digitalRead(buttonPin);
    if ((buttonState != previousButtonState) && (buttonState == LOW)) {
        counter++;
        for (int i = 0; i < 3; i++) {
            Keyboard.press(16);
            delay(330);
            Keyboard.press(20);
            delay(330);
            Keyboard.release(16);
            Keyboard.press(16);
            delay(330);
            Keyboard.press(20);
            delay(330);
            Keyboard.release(16);
            Keyboard.press(16);
            delay(330);
            Keyboard.press(20);
            delay(330);
            Keyboard.release(16);
        }
        Keyboard.print("You pressed the button ");
        Keyboard.print(counter);
        Keyboard.println(" times.");
    }
    previousButtonState = buttonState;
 delay(1); // チャタリング対策のため、20ミリ秒のディレイを挿入
}
