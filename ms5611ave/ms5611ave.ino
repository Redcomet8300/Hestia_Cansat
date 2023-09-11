#include "MS5611.h"

MS5611 MS5611(0x77);
const int numReadings = 100; // Number of pressure readings to average
float pressureReadings[numReadings];

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!MS5611.begin()) {
    Serial.println("MS5611 not found, check wiring!");
    while (1);
  }
}

void loop() {
  float totalP = 0;

  for (int i = 0; i < numReadings; i++) {
    MS5611.read();
    pressureReadings[i] = MS5611.getPressure();
    totalP += pressureReadings[i];
    delay(10); // Add a small delay between readings if needed
  }

  float aveP = totalP / numReadings;

  Serial.print("Temperature: ");
  Serial.print(MS5611.getTemperature());
  Serial.print("\tPressure: ");
  Serial.print(aveP);
  Serial.println();
  delay(500);
}
