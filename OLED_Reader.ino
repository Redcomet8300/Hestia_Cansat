#include <Adafruit_SSD1306.h>
#include <Wire.h> // Include the Wire library for I2C communication

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define your I2C address and reset pin (if used) here
#define OLED_ADDRESS 0x3C  // Change this to your OLED display's I2C address
#define OLED_RESET -1       // Change this to your OLED display's reset pin if used, or set to -1 if not used

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  display.begin(OLED_ADDRESS, OLED_RESET);
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
