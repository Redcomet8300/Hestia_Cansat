#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MS5611.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h>
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

#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
MS5611 MS5611(0x77);

float Celsius = 0;
const int numReadings = 100;
float pressureReadings[numReadings];

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int valx, valy, valz;

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
  Serial.begin(38400);
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of your module
    Serial.println("Starting LoRa failed!");
    while (1);
  }
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

  // Read MPU6050 data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  valx = map(ax, -17000, 17000, 0, 179);
  valy = map(ay, -17000, 17000, 0, 179);
  valz = map(az, -17000, 17000, 0, 179);

  Serial.print("Temperature: ");
  Serial.print(Celsius);
  Serial.print(" C\tPressure: ");
  Serial.print(aveP);
  Serial.print(" Pa");
  Serial.print(" axis x = ");
  Serial.print(valx);
  Serial.print(" axis y = ");
  Serial.print(valy);
  Serial.print(" axis z = ");
  Serial.println(valz);

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
  display.print("axis x = ");
  display.print(valx);
  display.print("axis y = ");
  display.print(valy);
  display.print("axis z = ");
  display.println(valz);
  display.display();

  // Add your LED control logic here
  if (valy > 150) {
    // Code for LED when valy is greater than 150
  } else if (valy < 50) {
    // Code for LED when valy is less than 50
  } else if (valx < 50) {
    // Code for LED when valx is less than 50
  } else if (valz > 150) {
    // Code for LED when valz is greater than 150
  }

  String MyMessage = "Temperature: " + String(Celsius) + " C\tPressure: " + String(aveP) + " Pa axis x = " + String(valx) + " axis y = " + String(valy) + " axis z = " + String(valz);
  LoRa.beginPacket();
  LoRa.print(MyMessage);
  LoRa.endPacket();
  delay(100);
}
