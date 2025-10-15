
const float frequency = 1.0; // 正弦波の周波数 (Hz)
const int sampleRate = 500;  // サンプリングレート (Hz) - 波形の滑らかさとCPU負荷に影響
const float phaseShift = 2.0 * M_PI / 3.0; // 120度 (360度 / 3)の位相差

void setup() {
  Serial.begin(9600);
 
}

void loop() {
  // サンプリング間隔の管理
  static unsigned long lastUpdateTime = 0;
  const unsigned long interval = 1000000 / sampleRate; // マイクロ秒単位の間隔

  if (micros() - lastUpdateTime >= interval) {
    lastUpdateTime = micros();

    // 現在の時間を秒単位で取得
    float timeSec = millis() / 1000.0;

    // 基本となる位相 (時間経過に伴い変化)
    float basePhase = 2.0 * M_PI * frequency * timeSec;

    // --- 各波形の値を計算 ---

    // 1. 位相 0°
    float phase1 = basePhase;
    int value1 = (int)((sin(phase1) + 1.0) * 127.5);
  

    // 2. 位相 +120°
    float phase2 = basePhase + phaseShift;
    int value2 = (int)((sin(phase2) + 1.0) * 127.5);


    // 3. 位相 +240° (または -120°)
    float phase3 = basePhase + 2.0 * phaseShift; // または basePhase - phaseShift
    int value3 = (int)((sin(phase3) + 1.0) * 127.5);
  

    // デバッグ用にシリアルモニタに出力 (3つの値を確認できます)
    Serial.print(value1);
    Serial.print("\t");
    Serial.print(value2);
    Serial.print("\t");
    Serial.println(value3);
  }
}
