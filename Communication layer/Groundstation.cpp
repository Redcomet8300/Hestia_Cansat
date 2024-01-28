#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Starting LoRa");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.println();
  }
}
