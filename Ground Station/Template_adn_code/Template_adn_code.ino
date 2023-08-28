void setup() {
  Serial.begin(9600); //เปลี่ยนได้
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(1000); //เขียนต่อ
}
