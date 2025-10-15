
#include <SPI.h>
#include <SD.h>

//SDカード書き込み必須グローバル変数
const int Data_Nunber_Of_Pieces = 20;
volatile double writeData[Data_Nunber_Of_Pieces];
volatile int writeCount = 0;
volatile bool writeRequest = false;
volatile int writeIndex = 0;    // 現在の書き込み位置

//センサー実装可視化変数
volatile bool error_SD = true;
bool error_BNO055 = true;
bool error_BNE280 = true;
bool error_GPS = true;

//デバック変数
//bool debug = true;    //serial.printをloop関数内でやるか否なか
bool debug = false;     //serial.printをloop関数内でやるか否なか




void setup() {
  delay(5000);
  Serial.begin(115200);

  Setup_other_pin();

  Setup_SD();
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  delay(50);
  dataFile.print("2025年御宿共同打ち上げ実験　AチームログSTART");
  dataFile.println();
  dataFile.flush();
  dataFile.close();

  Setup_BNO055();
  Setup_BME280();
  Setup_TWELITE();
  Setup_GPS();

  buzz_start();
}

void setup1() {
}


void loop() {
  if (debug) {
    Serial.println("start__________________________________");
  }



  Read_BME280();
  Read_BNO055();
  //Read_GPS();
  Read_Twilight();



  //SD書き込みコール
  writeRequest = true;
  writeIndex = 0;
  writeRequest = false;



  //送信情報"開始信号　時間　気圧　高度　温度　加速度XYZ　ジャイロZYX　緯度　経度　解放否か　フライトピン否か　終了信号"
  Sent_TWELITE();
  Sent_test_TWELITE();

}


void loop1() {
  if (error_SD) {
    if (writeRequest) {
      writeRequest = false;
      File dataFile = SD.open("datalog.txt", FILE_WRITE);

      if (dataFile) {
        dataFile.print(millis());
        dataFile.print(",");
        for (int i = 0; i < writeCount; i++) {
          dataFile.print(writeData[i]);
          dataFile.print(",");
        }
        dataFile.println();
        dataFile.flush();
        dataFile.close();
      }
      //if (debug) {
      // Serial.println("SD書き込み完了");
      //}
    }
  } else {
    if (debug) {
      Serial.println("NO SD______________________________________________");
    }
  }
}
