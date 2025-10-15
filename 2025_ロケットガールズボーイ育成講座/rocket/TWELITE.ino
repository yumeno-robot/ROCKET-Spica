

void Setup_TWELITE() {

  Serial2.setTX(4);
  Serial2.setRX(5);
  Serial2.begin(115200);


}


void Sent_TWELITE() {
  /*

    Serial2.println("test_start!!");
    Serial2.println("123456789");
    Serial2.println("yumenoshin");
    Serial2.println("夢之進");
    Serial2.println("🙇");
    delay(100);

    ////delay(100); // 少し待つと受信が安定します

    if (Serial2.available()) {
      delay(100);
      char val = Serial2.read();
      Serial.println(val);
    }

  */
}
