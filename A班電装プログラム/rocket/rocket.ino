
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

//トワイライトによるフラグ
bool motor_flag = false;
bool motor_forced_open = false;


//時間監視変数(アルゴリズム用)
unsigned long prevMillis = 0;
const unsigned long interval = 10;// [ms] ループ周期（10msごとに監視）

// 加速度Z監視用(アルゴリズム用)
int overGcount = 0;
const float ACCEL_THRESHOLD = 19.6;// 2G = 9.8 * 2 [m/s^2]
const int CONTINUE_TIME_MS = 200;// 継続時間 [ms]
const int REQUIRED_COUNT = CONTINUE_TIME_MS / interval;

// 気圧監視用(アルゴリズム用)
float prevPressure = 0.0;
int decreaseCount = 0;

// 離床検知フラグ(アルゴリズム用)
bool liftOffDetected = false;  // 離床検知済みか
float ACCEL_THRESHOLD_HIGH = 19.6;// 2G [m/s²]
float ACCEL_THRESHOLD_LOW  = 9.0;// 閾値下回り判定（自由落下近似）

// 状態管理用フラグ(アルゴリズム用)
bool accelOver2G = false;
bool burnEndFlag = false;

//フライトピン監視用フラグ(SDカード用)
bool Write_Sd_FirstTIME = false;
bool Flight = false;
//double flight_time;

bool Launch_completed = false;//発射が完全にOK出たらtrueになる
unsigned long Launch_completion_time;
bool First_Launch_completion = false;//一回だけ時間を書けるようにするやつ

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






  //prevMillis=前回処理を実行した時刻
  //interval=どのくらいの間隔で処理をしたいか200ミリ秒だから20回分
  //20ミリ秒に一度だけ実行する(以下全部において)
  unsigned long now = millis();
  if (now - prevMillis >= interval) {
    prevMillis = now;


    //_____________________________________________________________________________________以下離床検出プログラム_____________________________________________________________________________________

    Read_filtepin();//フライトピンの確認


    //もしフライトピンが抜けていたら以下を実行
    if (Flight) {



      // ---- 加速度Zの判定 ----
      float accelZ = Return_AccelZ();  // Z軸加速度を取得
      if (accelZ > ACCEL_THRESHOLD) {//______読んだ値が19.8m/s^2を越しているか否か。越してたら＋＋でなければリセット
        overGcount++;
      } else {
        overGcount = 0;
      }
      /*
        加速度Zが 19.6 m/s² (≒2G) を超えると「上昇中」とみなしてカウントを増やします。
        超えなかったらリセットします。
        つまり、「連続して2G以上が続いたら本当に上昇している」と判断します。
      */





      // ---- 気圧の判定 ----
      float currentPressure = Return_Pressure();  // 実際の気圧センサ値を取得
      if (currentPressure < prevPressure) {//_________今回のデータ(currentPressure)が前回のデータ(prevPressure)を越しているか否か。越していれば＋＋でなければリセット
        decreaseCount++;
      } else {
        decreaseCount = 0;
      }
      prevPressure = currentPressure;
      /*
        気圧が前回より下がっていたら decreaseCount++。
        上昇中は高度が上がり気圧が下がるため、「気圧が減少している = 上昇中」と判断します。
      */





      //_______________________________________________________________以下条件達成の場合減速装置放出機構動作プログラム___________________________________________________________________________
      if (overGcount >= REQUIRED_COUNT || decreaseCount >= REQUIRED_COUNT) {// ジャイロと気圧の連続性条件分岐文
        Launch_completed = true;
        if (!First_Launch_completion) {
          Launch_completion_time = millis();
        }
        First_Launch_completion = true;
      }


      if (Launch_completed) {
        float z = Return_AccelZ();

        // 1. Z軸が一度 2G を超えたかチェック
        if (z >= ACCEL_THRESHOLD_HIGH) {
          accelOver2G = true;// 2G超えたら「燃焼中」と判定
        }

        // 2. 2G超過済みなら、閾値を下回ったかチェック
        if (accelOver2G && z <= ACCEL_THRESHOLD_LOW) {
          burnEndFlag = true;       // 2Gを超えたあとに9m/s²以下になった → 燃焼終了
          //accelOver2G = false;      // 状態リセット（必要に応じて）
        }
      }
      /*
              連続して「加速度が2G超え」または「気圧が減少」した場合に、
              「燃焼終了」を確認します。
              もし燃焼が終わった（burnEndFlag == true）なら、「解放します！！」と出力。
      */





      if (burnEndFlag && (millis() - Launch_completion_time > 20)) {

        Serial.println("解放します！！");
      }





      // 一度出力後にリセット
      //overGcount = 0;
      //decreaseCount = 0;



    }//フライトピン終了カッコ
  }//20ミリ秒一回終了カッコ



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
