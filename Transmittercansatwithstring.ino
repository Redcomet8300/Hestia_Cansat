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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
MS5611 ms5611(0x77);
MPU6050 mpu;

int ax, ay, az;
int gx, gy, gz;
int x, y, z;
int counter = 0;

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Change the address to your device address
    Serial.println(F("SSD1306 allocation failed"));
    while (1); // Don't proceed, loop forever
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  Wire.begin();
  mpu.initialize();
  if (!ms5611.begin()) {
    Serial.println("MS5611 not found, check wiring!");
    while (1);
  }

  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  ms5611.read();
  float celsius = static_cast<float>(ms5611.getTemperature());

  float totalPressure = 0;
  const int NUM_READINGS = 100; // Define the number of readings
  for (int i = 0; i < NUM_READINGS; i++) {
    ms5611.read();
    totalPressure += ms5611.getPressure();
    delay(10);
  }
  float avePressure = totalPressure / NUM_READINGS;

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
  display.println(" hPa");
  display.print("Axis X: ");
  display.println(x);
  display.print("Axis Y: ");
  display.println(y);
  display.print("Axis Z: ");
  display.println(z);
  display.display();

  LoRa.beginPacket();
  LoRa.print(celsius);
  LoRa.print(",");
  LoRa.print(avePressure);
  LoRa.print(",");
  LoRa.print(x);
  LoRa.print(",");
  LoRa.print(y);
  LoRa.print(",");
  LoRa.print(z);
  LoRa.endPacket();
  delay(1000);
  // message = "";
  // message += String(celsius) + ",";
  // message += String(avePressure) + ",";
  // message += String(x) + ",";
  // message += String(y) + ",";
  // message += String(z);

  // Serial.println(message);
  // LoRa.beginPacket();
  // LoRa.print(message); // Modify this line to send "hello world"
  // LoRa.endPacket();
  // counter++;
  // delay(2000); // Add delay here if needed for sending frequency
}
