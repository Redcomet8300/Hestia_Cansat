#include <SPI.h>
#include <LoRa.h> 

int counter = 0;

// Change your string here (Just leave "...")
String messsage = "Hello";

void setup() {
  Serial.begin(9600);

  while (!Serial);  
  Serial.println("LoRa Sender");
  LoRa.end();
  if (!LoRa.begin(433E6)) {  
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  if(counter >= 0) {
    LoRa.beginPacket();
    LoRa.print(messsage);
    LoRa.endPacket();
    Serial.println("Send!");
    counter++;
  }
  delay(1000);  
}
