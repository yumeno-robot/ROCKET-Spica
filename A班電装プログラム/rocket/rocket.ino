
#include <SPI.h>
#include <SD.h>

//SDカード書き込み必須グローバル変数
const int Data_Nunber_Of_Pieces = 20;
volatile double writeData[Data_Nunber_Of_Pieces];
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


bool motor_flag = false;//トワイライトによるフラグ
bool motor_forced_open = false;


unsigned long prevMillis = 0;
const unsigned long interval = 10;  // [ms] ループ周期（10msごとに監視）

// 加速度Z監視用
int overGcount = 0;
const float ACCEL_THRESHOLD = 19.6;     // 2G = 9.8 * 2 [m/s^2]
const int CONTINUE_TIME_MS = 200;       // 継続時間 [ms]
const int REQUIRED_COUNT = CONTINUE_TIME_MS / interval;

// 気圧監視用
float prevPressure = 0.0;
int decreaseCount = 0;

// 離床検知フラグ（例）
bool liftOffDetected = false;  // 離床検知済みか
float ACCEL_THRESHOLD_HIGH = 19.6; // 2G [m/s²]
float ACCEL_THRESHOLD_LOW  = 9.0;  // 閾値下回り判定（自由落下近似）

// 状態管理用フラグ
bool accelOver2G = false;
bool burnEndFlag = false;


bool Write_Sd_FirstTIME = false;
double Flight = false;
double flight_time;


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
  //servo(0);//閉じる
}

void setup1() {
}


void loop() {
  if (debug) {
    Serial.println("start__________________________________");
  }



  //______________全センサー値の取得__________________________
  Read_Twilight();
  Read_BME280();
  Read_Twilight();
  Read_BNO055();
  Read_Twilight();
  Read_GPS();
  controlMotor();

  //___________SD書き込みコール______________________________
  writeRequest = true;






  //___________以下にアルゴリズム追加____________________________

  
  Read_filtepin();


  unsigned long now = millis();
  if (now - prevMillis >= interval) {
    prevMillis = now;


    if (Flight) {


      // ---- 加速度Zの判定 ----
      float accelZ = Return_AccelZ();  // Gravity_Z の値が返る
      if (accelZ > ACCEL_THRESHOLD) {
        overGcount++;
      } else {
        overGcount = 0;
      }

      // ---- 気圧の判定 ----
      float currentPressure = Return_Pressure();  // 実際の気圧センサ値を取得
      if (currentPressure < prevPressure) {
        decreaseCount++;
      } else {
        decreaseCount = 0;
      }
      prevPressure = currentPressure;





      // ---- 条件成立 ----
      if (overGcount >= REQUIRED_COUNT || decreaseCount >= REQUIRED_COUNT) {// ジャイロと気圧の連続性条件分岐文

        checkBurnEnd();
        if (burnEndFlag ) {
          Serial.println("解放します！！");
        }



        // 一度出力後にリセット
        //overGcount = 0;
        //decreaseCount = 0;
      }
    }

  }









  //Sent_TWELITE();

}




void loop1() {
  if (error_SD) {
    if (writeRequest) {
      File dataFile = SD.open("datalog.txt", FILE_WRITE);

      if (dataFile) {
        dataFile.print(millis());
        dataFile.print(",");
        for (int i = 0; i < writeIndex; i++) {
          dataFile.print(writeData[i]);
          dataFile.print(",");
        }
        dataFile.println();
        dataFile.flush();
        dataFile.close();
      }
      if (debug) {
        Serial.println("SD書き込み完了");
      }

      writeIndex = 0;
      writeRequest = false;
    }
  } else {
    if (debug) {
      Serial.println("NO SD______________________________________________");
    }
  }
}
