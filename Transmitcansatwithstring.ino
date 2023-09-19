#include <Wire.h>
#include <MS5611.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <SPI.h>
#include <LoRa.h>

MS5611 MS5611(0x77);

float Celsius = 0;
const int numReadings = 100;
float pressureReadings[numReadings];

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int valx, valy, valz;

void setup() {
  Serial.begin(9600);

  if (!MS5611.begin()) {
    Serial.println("MS5611 not found, check wiring!");
    while (1);
  }

  Wire.begin();
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
  // Read temperature from MS5611
  MS5611.read();
  Celsius = MS5611.getTemperature();

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

  String MyMessage = String(Celsius) + "," + String(aveP) + "," + String(valx) + "," + String(valy) + "," + String(valz);
  LoRa.println(MyMessage);
  LoRa.endPacket();
  delay(1000);
}
