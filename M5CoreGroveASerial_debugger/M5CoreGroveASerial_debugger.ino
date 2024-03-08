#include <M5Stack.h>
#define CONSOLE Serial

#include <SoftwareSerial.h>
SoftwareSerial GroveA(21, 22);

const int BUFFER_SIZE = 128;
char buffer[BUFFER_SIZE];
int bufferIndex = 0;
int Line = 0;
bool btnAPressed = false;

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(M5.Lcd.color565(255, 255, 255));
  CONSOLE.begin(9600);
  GroveA.begin(9600);
  delay(5000);
  CONSOLE.println("-- HELLO (baud rate = 9600)");
}

void processSerialData() {
  while (GroveA.available()) {
 
    char c = GroveA.read();
    if (c == '\n') {
      buffer[bufferIndex] = '\0'; // Null-terminate the string
      CONSOLE.print("Recv: ");
      CONSOLE.println(buffer);
         ++Line;
      M5.Lcd.print("Re ");
      M5.Lcd.print(Line);
      M5.Lcd.print(":");
      M5.Lcd.println(buffer);
      bufferIndex = 0; // Reset buffer index for next data
    } else {
      if (bufferIndex < BUFFER_SIZE - 1) {
        buffer[bufferIndex++] = c; // Store character in buffer
      }
    }
  }
}

void loop() {

 M5.update();
  processSerialData();


  
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.fillScreen(M5.Lcd.color565(0, 0, 0)); // Clear the display
   M5.Lcd.setCursor(0, 0); 
   Line=0;
  }

  unsigned long uptime_ms = millis();
  GroveA.println(uptime_ms);
  CONSOLE.print("<<Send>>: ");
  CONSOLE.println(uptime_ms);

  delay(100);
}
