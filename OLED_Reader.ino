#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  display.begin(SSD1306_I2C_ADDRESS, OLED_RESET);
  display.display();
  delay(2000);
  display.clearDisplay();
  Serial.begin(9600); // Initialize Serial communication
}

void loop() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Check if there is data available to read from the serial monitor
  if (Serial.available()) {
    String serialData = Serial.readString(); // Read the data from the serial monitor
    display.println(serialData); // Display the data on the OLED screen
  }

  display.display();
  delay(1000);
}
