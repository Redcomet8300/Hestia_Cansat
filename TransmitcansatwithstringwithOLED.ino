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
int x, y, z;
int vx, vy, vz;

const int ledPin = 13; // Change to the pin where your LED is connected
bool loraTempinDisplaysent = false;
bool loraPressureinDisplaysent = false;
bool loraXinDisplaysent = false;
bool loraYinDisplaysent = false;
bool loraZinDisplaysent = false;
bool loraMessageSent = false;

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

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  valx = map(ax, -17000, 17000, 0, 179);
  valy = map(ay, -17000, 17000, 0, 179);
  valz = map(az, -17000, 17000, 0, 179);
  
}

void loop() {

  MS5611.read();
  Celsius = MS5611.getTemperature();

  float totalP = 0;
  for (int i = 0; i < numReadings; i++) {
    MS5611.read();
    pressureReadings[i] = MS5611.getPressure();
    totalP += pressureReadings[i];
    delay(10);
  }
  float aveP = totalP / numReadings;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  x = map(ax, -17000, 17000, 0, 179);
  y = map(ay, -17000, 17000, 0, 179);
  z = map(az, -17000, 17000, 0, 179);

  vx = x - valx;
  vy = y - valy;
  vz = z - valz;

  Serial.print("Temperature: ");
  Serial.print(Celsius);
  Serial.print(" C Pressure: ");
  Serial.print(aveP);
  Serial.print(" Pa");
  Serial.print(" Axis X = ");
  Serial.print(vx);
  Serial.print(" Axis Y = ");
  Serial.print(vy);
  Serial.print(" Axis Z = ");
  Serial.println(vz);

  String TempinDisplay = "Temperature: " + String(Celsius) + " C";
  String PressureinDisplay = "Pressure: " + String(aveP) + " Pa";
  String XinDisplay = "Axis X = " + String(vx);
  String YinDisplay = "Axis Y = " + String(vy);
  String ZinDisplay = "Axis Z = " + String(vz);
  String MyMessage = String(Celsius) + "," + String(aveP) + "," + String(vx) + "," + String(vy) + "," + String(vz);

  if (LoRa.beginPacket()) {
    loraTempinDisplaysent = LoRa.print(TempinDisplay);
    loraPressureinDisplaysent = LoRa.println(PressureinDisplay);
    loraXinDisplaysent = LoRa.println(XinDisplay);
    loraYinDisplaysent = LoRa.println(YinDisplay);
    loraZinDisplaysent = LoRa.println(ZinDisplay);
    loraMessageSent = LoRa.print(MyMessage);
    loraTempinDisplaysent &= LoRa.endPacket();
    loraPressureinDisplaysent &= LoRa.endPacket();
    loraXinDisplaysent &= LoRa.endPacket();
    loraYinDisplaysent &= LoRa.endPacket();
    loraZinDisplaysent &= LoRa.endPacket();
    loraMessageSent &= LoRa.endPacket();
  } else {
    loraTempinDisplaysent = false;
    loraPressureinDisplaysent = false;
    loraXinDisplaysent = false;
    loraYinDisplaysent = false;
    loraZinDisplaysent = false;
    loraMessageSent = false;
  }

  if (loraMessageSent) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
