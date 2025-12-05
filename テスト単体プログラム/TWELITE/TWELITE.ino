void setup() {

  Serial.begin(115200);


  Serial2.setTX(4);
  Serial2.setRX(5);
  Serial2.begin(115200);

}

void loop() {

  Serial2.println("test_start!!");
  Serial2.println("123456789");
  Serial2.println("yumenoshin");
  Serial2.println("こんにちは");
  delay(100);


  if (Serial2.available()) {
    delay(100);
    char val = Serial2.read();
    Serial.println(val);
  }

}
