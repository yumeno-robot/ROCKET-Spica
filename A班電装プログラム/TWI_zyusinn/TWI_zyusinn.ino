#define BUFFER_SIZE 200
char sensorData[BUFFER_SIZE] = "";  // 送信機1のセンサーデータ
unsigned long lastReceiveTime = 0;  // 最後にデータを受信した時刻
const unsigned long TIMEOUT_MS = 10000;  // 10秒のタイムアウト
unsigned long t1 = 0;



void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(17, OUTPUT);
  digitalWrite(19, HIGH);
  lastReceiveTime = millis();  // 初期化
}



void loop() {
  static char buffer[BUFFER_SIZE] = "";
  static uint8_t index = 0;

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // 改行まで受信
    input.trim();  // 前後の空白や改行を削除

    if (input == "あける") {
      Serial.println("Serial1 に 'あける' を送信します。");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");
      Serial1.println("あける");

    } else {
      Serial.print("入力された文字列：");
      Serial.println(input);
    }
  }


  /*
    if (t1 == 0) {
    t1 = millis();
    } else if (millis() - t1 >= 5000) {
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial1.println("あける");
    Serial.println("あける");
    t1 = 0;
    }
  */



  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n' || index >= BUFFER_SIZE - 1) {
      buffer[index] = '\0';
      processReceivedData(buffer);
      index = 0;
    } else {
      buffer[index++] = c;
    }
    digitalWrite(17, HIGH);
  }
  digitalWrite(17, LOW);
  // 10秒以上データを受信していない場合はピン18を点灯
  if (millis() - lastReceiveTime > TIMEOUT_MS) {
    digitalWrite(18, HIGH);
  } else {
    digitalWrite(18, LOW);
  }
}




void processReceivedData(char* data) {
  // 送信機1のデータを更新してシリアル出力
  strncpy(sensorData, data, BUFFER_SIZE);
  lastReceiveTime = millis();  // 受信時刻を更新
  
  Serial.println(sensorData);
}
