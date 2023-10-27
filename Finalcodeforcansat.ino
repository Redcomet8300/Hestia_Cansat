#include <Wire.h>
#include <MS5611.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <LoRa.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI 5
#define OLED_CLK 4
#define OLED_DC 7
#define OLED_CS 8
#define OLED_RESET 6
#define NUM_READINGS 100

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
MS5611 ms5611(0x77);
MPU6050 mpu;

bool loraMessageSent = false;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int x, y, z;

char message[25]; // Allocate memory for the message

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1); // Don't proceed, loop forever
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  if (!ms5611.begin()) {
    Serial.println("MS5611 not found, check wiring!");
    while (1);
  }

  Wire.begin();
  mpu.initialize();

  if (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of your module
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  x = map(ax, -17000, 17000, 0, 179);
  y = map(ay, -17000, 17000, 0, 179);
  z = map(az, -17000, 17000, 0, 179);
}

void loop() {
  ms5611.read();
  int16_t celsius = static_cast<int16_t>(ms5611.getTemperature());

  int32_t totalPressure = 0; // Use int32_t for more headroom
  for (int i = 0; i < NUM_READINGS; i++) {
    ms5611.read();
    totalPressure += static_cast<int32_t>(ms5611.getPressure());
    delay(10);
  }
  int32_t avePressure = totalPressure / NUM_READINGS; // Use int32_t

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  x = map(ax, -17000, 17000, 0, 179);
  y = map(ay, -17000, 17000, 0, 179);
  z = map(az, -17000, 17000, 0, 179);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 1);
  display.print("Temperature: ");
  display.print(celsius);
  display.println(" C");
  display.print("Pressure: ");
  display.print(avePressure);
  display.println(" Pa");
  display.print("Axis X: ");
  display.println(x);
  display.print("Axis Y: ");
  display.println(y);
  display.print("Axis Z: ");
  display.println(z);
  display.display();

  snprintf(message, sizeof(message), "%d,%ld,%d,%d,%d", celsius, avePressure, x, y, z);

  if (LoRa.beginPacket()) {
    Serial.println(message);
    loraMessageSent = LoRa.print(message);
    loraMessageSent &= LoRa.endPacket();
  } else {
    loraMessageSent = false;
  }

}
