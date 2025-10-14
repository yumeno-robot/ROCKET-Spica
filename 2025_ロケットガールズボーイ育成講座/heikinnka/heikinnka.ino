// 擬似センサデータを生成して、
// 生の値と二次関数近似を使った平均化値を出力するプログラム

float t_buf[3];
float y_buf[3];

const unsigned long SAMPLE_INTERVAL = 50;
unsigned long lastMillis = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("raw,smoothed"); // プロッタ用ラベル
}

void loop() {
  unsigned long now = millis();
  if (now - lastMillis >= SAMPLE_INTERVAL) {
    lastMillis = now;
    t += SAMPLE_INTERVAL;

    // ==== 擬似センサ値生成 ====
    float raw = 500 + 200 * sin(2 * PI * (t / 1000.0) * 0.5)  // 周期2秒のsin波
                + random(-50, 50);                           // ノイズ

    // データをシフト
    t_buf[0] = t_buf[1];
    t_buf[1] = t_buf[2];
    t_buf[2] = (float)t;
    y_buf[0] = y_buf[1];
    y_buf[1] = y_buf[2];
    y_buf[2] = raw;

    float smoothed = raw;

    // 3点揃ったら二次近似を使った平均化
    if (t_buf[0] > 0) {
      smoothed = quadraticAverage(t_buf, y_buf);
    }

    // ==== 出力 ====
    Serial.print(raw);
    Serial.print(",");
    Serial.println(smoothed);
  }
}

// 二次関数で3点近似して、その放物線上で3点分の値を平均化する
float quadraticAverage(float t_buf[3], float y_buf[3]) {
  float sum = 0.0;
  for (int i = 0; i < 3; i++) {
    sum += quadraticFitPredict(t_buf, y_buf, t_buf[i]);
  }
  return sum / 3.0;  // 3点の平均
}

// ラグランジュ補間（二次関数近似）
float quadraticFitPredict(float t_buf[3], float y_buf[3], float t_pred) {
  float t1 = t_buf[0], t2 = t_buf[1], t3 = t_buf[2];
  float y1 = y_buf[0], y2 = y_buf[1], y3 = y_buf[2];

  float L1 = ((t_pred - t2)*(t_pred - t3)) / ((t1 - t2)*(t1 - t3));
  float L2 = ((t_pred - t1)*(t_pred - t3)) / ((t2 - t1)*(t2 - t3));
  float L3 = ((t_pred - t1)*(t_pred - t2)) / ((t3 - t1)*(t3 - t2));

  return y1*L1 + y2*L2 + y3*L3;
}
