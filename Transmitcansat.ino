#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <MS5611.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <SPI.h>
#include <LoRa.h>

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
  if (!MS5611.begin()) {
    Serial.println("MS5611 not found, check wiring!");
    while (1);
  }

  Wire.begin();
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");

  while (!Serial);
  Serial.println("LoRa Sender");
 
  if (!LoRa.begin(433E6)) {
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

  Serial.println("Sending packet: ");
 
  // send packet
  LoRa.beginPacket();
  LoRa.print("Temperature: ");
  LoRa.print(Celsius);
  LoRa.print(" C\tPressure: ");
  LoRa.print(aveP);
  LoRa.print(" Pa");
  LoRa.print(" axis x = ");
  LoRa.print(valx);
  LoRa.print(" axis y = ");
  LoRa.print(valy);
  LoRa.print(" axis z = ");
  LoRa.print(valz);

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
  LoRa.endPacket();
  delay(500);
}