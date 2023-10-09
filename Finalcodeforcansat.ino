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

const int ledPin = 13; // Change to the pin where your LED is connected
bool loraMessageSent = false;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int valx, valy, valz;
int x, y, z;
int vx, vy, vz;

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

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  valx = map(ax, -17000, 17000, 0, 179);
  valy = map(ay, -17000, 17000, 0, 179);
  valz = map(az, -17000, 17000, 0, 179);
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

  vx = x - valx;
  vy = y - valy;
  vz = z - valz;

  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.print(" C Pressure: ");
  Serial.print(avePressure);
  Serial.print(" Pa");
  Serial.print(" Axis X = ");
  Serial.print(vx);
  Serial.print(" Axis Y = ");
  Serial.print(vy);
  Serial.print(" Axis Z = ");
  Serial.println(vz);

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
  display.print("Axis X = ");
  display.println(vx);
  display.print("Axis Y = ");
  display.println(vy);
  display.print("Axis Z = ");
  display.println(vz);
  display.display();

  snprintf(message, sizeof(message), "%d,%ld,%d,%d,%d", celsius, avePressure, vx, vy, vz);

  if (LoRa.beginPacket()) {
    Serial.println(message);
    loraMessageSent = LoRa.print(message);
    loraMessageSent &= LoRa.endPacket();
  } else {
    loraMessageSent = false;
  }

  digitalWrite(ledPin, loraMessageSent ? HIGH : LOW);
}
