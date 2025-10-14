void setup() {
  // シリアル通信を開始し、ボーレートを設定
  Serial.begin(9600);
}

void loop() {
  // 1. 0から100までカウントアップ
  Serial.println("--- 0 to 100 ---");
  for (int i = 0; i <= 100; i++) {
    Serial.println(i);
    delay(10); // 出力間隔を調整
  }

  // 2. 100から-100までカウントダウン
  Serial.println("--- 100 to -100 ---");
  for (int i = 100; i >= -100; i--) {
    Serial.println(i);
    delay(10); // 出力間隔を調整
  }

  // 3. -100から0までカウントアップ
  Serial.println("--- -100 to 0 ---");
  for (int i = -100; i <= 0; i++) {
    Serial.println(i);
    delay(10); // 出力間隔を調整
  }

  // 全てのループが終わったら、次のサイクルまで待機
  Serial.println("--- Loop finished, waiting for 5 seconds ---");
  delay(5000); 
}
