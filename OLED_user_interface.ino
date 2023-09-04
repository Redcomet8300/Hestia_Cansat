#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
}

void loop() {
  printBinary("01001100");
  delay(500);

  printBinary("01101111");
  delay(500);

  printBinary("01100001");
  delay(500);

  printBinary("01100100");
  delay(500);

  printBinary("01101001");
  delay(500);

  printBinary("01101110");
  delay(500);

  printBinary("01100111");
  delay(500);

  display.clearDisplay();
  display.display();
}

void printBinary(const char *binaryString) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(binaryString);
  display.display();
  Serial.println(binaryString);
}
