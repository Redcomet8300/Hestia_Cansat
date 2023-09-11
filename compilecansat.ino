#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MS5611.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI 5
#define OLED_CLK 4
#define OLED_DC 7
#define OLED_CS 8
#define OLED_RESET 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
MS5611 MS5611(0x77);

MPU6050 mpu;
int16_t ax, ay, az;
float Celsius = 0;
const int numReadings = 100;
float pressureReadings[numReadings];

void setup() {
  sensors.begin();
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(1000);
  display.clearDisplay();
  if (!MS5611.begin()) {
    Serial.println("MS5611 not found, check wiring!");
    while (1);
  }
  Wire.begin();
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
}

void loop() {
  // Read temperature from DallasTemperature
  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);

  // Read pressure from MS5611
  float totalP = 0;
  for (int i = 0; i < numReadings; i++) {
    MS5611.read();
    pressureReadings[i] = MS5611.getPressure();
    totalP += pressureReadings[i];
    delay(10);
  }
  float aveP = totalP / numReadings;

  // Read motion data from MPU6050
  mpu.getMotion6(&ax, &ay, &az, nullptr, nullptr, nullptr);

  // Print and display data
  Serial.print("Temperature: ");
  Serial.print(Celsius);
  Serial.print(" C\tPressure: ");
  Serial.print(aveP);
  Serial.println(" Pa");

  Serial.print("Axis X: ");
  Serial.print(ax);
  Serial.print("\tAxis Y: ");
  Serial.print(ay);
  Serial.print("\tAxis Z: ");
  Serial.println(az);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 1);
  display.print("Temperature: ");
  display.print(Celsius);
  display.println(" C");
  display.print("Pressure: ");
  display.print(aveP);
  display.println(" Pa");
  display.print("Axis X: ");
  display.print(ax);
  display.print("          Axis Y: ");
  display.print(ay);
  display.print("          Axis Z: ");
  display.println(az);
  display.display();

  delay(500);
}
