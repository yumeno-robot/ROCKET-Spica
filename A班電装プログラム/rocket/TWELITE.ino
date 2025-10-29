

void Setup_TWELITE() {
  Serial2.setTX(4);
  Serial2.setRX(5);
  Serial2.begin(115200);
}




bool Reset_Twelite_Log = false;
int Log_Write_Count = 0;
const int Log_count_max = 10;
double Log_Write_Datas[Log_count_max];
unsigned long t1 = 0;

void Make_Twelite_Log(double Value) {
  if (Reset_Twelite_Log) {
    for (int i = 0; i < Log_count_max; i++) {
      Log_Write_Datas[i] = 0;
    }
    Log_Write_Count = 0;
    Reset_Twelite_Log = false;
  }

  if (Log_Write_Count < Log_count_max) {
    Log_Write_Datas[Log_Write_Count] = Value;
    Log_Write_Count++;
  }
}

void Sent_TWELITE() {
  if (t1 == 0) {
    t1 = millis();
  } else if (millis() - t1 >= 50) {
    Serial2.print("START");
    //Serial.print("START");
    for (int i = 0; i < Log_Write_Count; i++) {//ここの数を変えたほうが良き。
      Serial2.print(",");
      Serial2.print(Log_Write_Datas[i], 3);
      //Serial.print(",");
      //Serial.print(Log_Write_Datas[i], 3);
    }
    Serial2.print(",FINISH\n");
    Serial2.flush();
    Serial.flush();
    delayMicroseconds(50);

    t1 = 0;
  }
  //Serial.print(",FINISH\n");
  Reset_Twelite_Log = true;

}




//Sent_test_TWELITE();
void Sent_test_TWELITE() {
  Serial2.print("test_start!!  ");
  Serial2.print("123456789  ");
  Serial2.print("yumenoshin  ");
  Serial2.print("夢之進  ");
  Serial2.print("🙇  ");
  Serial2.print(111111);
  Serial2.print(876543);
  Serial2.print(987654);
}







//以下chatGPT_______________________________________________________________________

#define RX_BUFFER_SIZE 64  // 受信バッファサイズ（必要に応じて調整）
char rxBuffer[RX_BUFFER_SIZE];  // 受信文字列を格納
int rxIndex = 0;                // 現在の受信位置
bool First_reception_Twilight = false;

// -------------------------------------------------
// Twilightから1行受信して処理する関数
// -------------------------------------------------
void Read_Twilight() {

  if (!First_reception_Twilight) {

    if (Serial2.available() == 0)return;

    while (Serial2.available() > 0) {   // Twilightが接続されているシリアルポート
      delay(3);
      char c = Serial2.read();          // 1文字受信

      if (c == '\n' || c == '\r') {     // 改行が来たら処理
        if (rxIndex > 0) {
          rxBuffer[rxIndex] = '\0';     // 文字列終端
          Serial.print("受信データ: ");
          Serial.println(rxBuffer);     // まとめて出力

          if (strcmp(rxBuffer, "あける") == 0) {
            Serial.println("「開ける」受信したわーーーーーーー！！ok");
            motor_flag = true;
            First_reception_Twilight = true;
          }

          rxIndex = 0;                  // 次のデータに備えてリセット
        }
        delay(3);

      } else {
        if (rxIndex < RX_BUFFER_SIZE - 1) {
          rxBuffer[rxIndex++] = c;      // バッファに追加
        }
      }
    }
  }


}
