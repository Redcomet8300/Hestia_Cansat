#include <Adafruit_SSD1306.h> 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  display.begin(SSD1306_I2C_ADDRESS, OLED_RESET);
  display.display(); 
  delay(2000); 
  display.clearDisplay(); 
}

void loop() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Status Monitor");

  display.display();
  delay(1000); 
}
