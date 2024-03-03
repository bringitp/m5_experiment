#include <M5Stack.h>
#define CONSOLE Serial

#include <SoftwareSerial.h> /* EspSoftwareSerial */
/*                 rxPin, txPin | https://github.com/plerup/espsoftwareserial/blob/master/src/SoftwareSerial.h#L93 */
SoftwareSerial GroveA(21, 22);

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(M5.Lcd.color565(255, 255, 255));
  CONSOLE.begin(9600); /* ref: https://qiita.com/inouetai/items/a4f8b134be7cc40bc42e */
  GroveA.begin(9600); /* ref: https://github.com/plerup/espsoftwareserial/blob/master/src/SoftwareSerial.h#L118 */
  delay(5000);
  CONSOLE.println("-- HELLO (baud rate = 9600)");
}

void loop() {
  M5.Lcd.fillScreen(M5.Lcd.color565(0, 0, 0));
  M5.Lcd.setCursor(0, 0);
  while (GroveA.available()) {
    String recv_str = GroveA.readStringUntil('\n');
    CONSOLE.print("Recv: ");
    CONSOLE.println(recv_str);
    M5.Lcd.print("Recv: ");
    M5.Lcd.println(recv_str);
  }

  unsigned long uptime_ms = millis();
  GroveA.println(uptime_ms);
  CONSOLE.print("<<Send>>: ");
  CONSOLE.println(uptime_ms);

  delay(100);
}