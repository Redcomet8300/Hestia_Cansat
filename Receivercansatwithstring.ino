#include <SPI.h>
#include <LoRa.h>
String inString = "";
String MyMessage = "";
 
void setup() {
  Serial.begin(9600);
 
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    while (LoRa.available())
    {
      int inChar = LoRa.read();
      inString += (char)inChar;
      MyMessage = inString;
    }
    inString = "";
    LoRa.packetRssi();
  }
  Serial.println(MyMessage);
}