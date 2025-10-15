

void Setup_TWELITE() {
  Serial2.setTX(4);
  Serial2.setRX(5);
  Serial2.begin(115200);
}




bool Reset_Twelite_Log = false;
int Log_Write_Count = 0;
const int Log_count_max = 10;
double Log_Write_Datas[Log_count_max];


void Make_Twelite_Log(double Value) {
  if (Reset_Twelite_Log) {
    for (int i = 0; i < 10; i++) {
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
  Serial2.print("START");
  for (int i = 0; i < Log_Write_Count; i++) {//ここの数を変えたほうが良き。
    Serial2.print(",");
    Serial2.print(Log_Write_Datas[i], 3);
  }
  Serial2.print(",FINISH\n");
  Reset_Twelite_Log = true;
}





void Sent_test_TWELITE() {
  Serial2.println("test_start!!");
  Serial2.println("123456789");
  Serial2.println("yumenoshin");
  Serial2.println("夢之進");
  Serial2.println("🙇");
}







//以下chatGPT_______________________________________________________________________

#define RX_BUFFER_SIZE 64  // 受信バッファサイズ（必要に応じて調整）
char rxBuffer[RX_BUFFER_SIZE];  // 受信文字列を格納
int rxIndex = 0;                // 現在の受信位置

// -------------------------------------------------
// Twilightから1行受信して処理する関数
// -------------------------------------------------
void Read_Twilight() {
  while (Serial2.available() > 0) {   // Twilightが接続されているシリアルポート
    char c = Serial2.read();          // 1文字受信

    if (c == '\n' || c == '\r') {     // 改行が来たら処理
      if (rxIndex > 0) {
        rxBuffer[rxIndex] = '\0';     // 文字列終端
        Serial.print("受信データ: ");
        Serial.println(rxBuffer);     // まとめて出力

        rxIndex = 0;                  // 次のデータに備えてリセット
      }
    }
    else {
      if (rxIndex < RX_BUFFER_SIZE - 1) {
        rxBuffer[rxIndex++] = c;      // バッファに追加
      }
    }
  }
}
