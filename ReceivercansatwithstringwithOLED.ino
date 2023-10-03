#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
String Temp = "";
String Pressure = "";
String X = "";
String Y = "";
String Z = "";
String inString = ""; 
String TempinDisplay = ""; 
String PressureinDisplay = ""; 
String XinDisplay = ""; 
String YinDisplay = ""; 
String ZinDisplay = ""; 
String MyMessage = ""; 

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000);
  display.clearDisplay();
 
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
      Temp += (char)inChar;
      Pressure += (char)inChar;
      X += (char)inChar;
      Y += (char)inChar;
      Z += (char)inChar;
      inString += (char)inChar;
      TempinDisplay = Temp;
      PressureinDisplay = Pressure;
      XinDisplay = X;
      YinDisplay = Y;
      ZinDisplay = Z;
      MyMessage = inString;
    }
    Temp = "";
    Pressure = "";
    X = "";
    Y = "";
    Z = "";
    inString = "";
    LoRa.packetRssi();
  }

  Serial.println(MyMessage);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(1, 1);
  display.print(TempinDisplay);
  display.println(PressureinDisplay);
  display.println(XinDisplay);
  display.println(YinDisplay);
  display.println(ZinDisplay);
  display.display();
  delay(10000);

}