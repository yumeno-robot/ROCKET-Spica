import processing.serial.*;

Serial myPort;
PrintWriter logger;   // ★ログ保存用

// データ格納用変数
float temperature = 0;
float pressure = 1013.25;
float humidity = 0;
float accel = 9.8;
float latitude = 0;
float longitude = 0;
float altitude = 0;
float liftoffTime = 0;
float deployTime = 0;

// 気圧の基準値
float basePressure = 1013.25;

// グラフ履歴
int maxPoints = 300;
float[] altHistory = new float[maxPoints];
float[] presHistory = new float[maxPoints];
float[] accelHistory = new float[maxPoints];
int index = 0;

// 計測制御フラグ
boolean telemetryActive = false;
int launchStart = -1;

// ★送信コマンド入力欄
String sendText = "";


void setup() {
  size(1000, 820);
  surface.setTitle("Rocket Telemetry Monitor - Extended");
  printArray(Serial.list());

  if (Serial.list().length > 0) {
    myPort = new Serial(this, Serial.list()[0], 9600);
    myPort.bufferUntil('\n');
  } else {
    println("No serial ports found.");
  }

  // ★ログ開始
  logger = createWriter("telemetry_log.txt");
  logger.println("timestamp,temperature,pressure,humidity,accel,latitude,longitude,altitude,liftoffTime,deployTime");

  textAlign(LEFT, CENTER);

  for (int i = 0; i < maxPoints; i++) {
    altHistory[i] = 0;
    presHistory[i] = basePressure;
    accelHistory[i] = 0;
  }
}

void draw() {
  background(10);
  fill(255);
  textSize(18);
  text("🚀 Rocket Telemetry Monitor", 20, 25);

  drawClockDisplay();
  drawControlButton();
  drawDataPanel();
  drawDeployIndicator();
  drawGraphs();
  drawSendPanel();   // ★送信UI

  // ★安全のため毎フレーム flush（軽負荷）
  if (telemetryActive && logger != null) logger.flush();
}

////////////////////////////////////////////////////////////////
//  シリアル受信処理
////////////////////////////////////////////////////////////////
void serialEvent(Serial p) {
  String line = trim(p.readStringUntil('\n'));
  if (line == null || line.length() == 0) return;
  if (!line.startsWith("START") || !line.endsWith("FINISH")) return;

  String payload = line.substring(6, line.length() - 6);
  String[] values = split(payload, ',');

  if (values.length < 9) return;

  try {
    temperature = Float.parseFloat(values[0]);
    pressure    = Float.parseFloat(values[1]);
    humidity    = Float.parseFloat(values[2]);
    accel       = Float.parseFloat(values[3]);
    latitude    = Float.parseFloat(values[4]);
    longitude   = Float.parseFloat(values[5]);
    altitude    = Float.parseFloat(values[6]);
    liftoffTime = Float.parseFloat(values[7]);
    deployTime  = Float.parseFloat(values[8]);

    if (telemetryActive) {
      // 履歴更新
      altHistory[index]  = altitude;
      presHistory[index] = pressure;
      accelHistory[index] = accel;
      index = (index + 1) % maxPoints;

      if (launchStart == -1) launchStart = millis();

      // ★ログ保存
      String timestamp = str(millis() / 1000.0);
      logger.println(
        timestamp + "," +
        temperature + "," +
        pressure + "," +
        humidity + "," +
        accel + "," +
        latitude + "," +
        longitude + "," +
        altitude + "," +
        liftoffTime + "," +
        deployTime
      );
    }

  } catch (Exception e) {
    println("Parse error: " + e + " -- line: " + payload);
  }
}

////////////////////////////////////////////////////////////////
// 時刻表示
////////////////////////////////////////////////////////////////
void drawClockDisplay() {
  textSize(14);
  fill(255);
  String now = nf(hour(), 2) + ":" + nf(minute(), 2) + ":" + nf(second(), 2);
  text("Current Time : " + now, 20, 45);

  if (launchStart >= 0) {
    float elapsed = (millis() - launchStart) / 1000;
    text("Elapsed (since telemetry start): " + nf(elapsed, 0, 1) + " s", 250, 45);
  } else {
    text("Elapsed (since telemetry start): -- s", 250, 45);
  }
}

////////////////////////////////////////////////////////////////
// データ表示
////////////////////////////////////////////////////////////////
void drawDataPanel() {
  fill(255);
  textSize(14);
  int x = 20;
  int y = 90;

  text("Temperature : " + nf(temperature, 0, 2) + " °C", x, y);
  text("Pressure    : " + nf(pressure, 0, 2) + " hPa", x, y + 25);
  text("Humidity    : " + nf(humidity, 0, 2) + " %", x, y + 50);
  text("Acceleration: " + nf(accel, 0, 2) + " m/s²", x, y + 75);
  text("Latitude    : " + degToDMS(latitude, false), x, y + 100);
  text("Longitude   : " + degToDMS(longitude, true), x, y + 125);
  text("Altitude    : " + nf(altitude, 0, 2) + " m", x, y + 150);
  text("Liftoff Time: " + nf(liftoffTime, 0, 1) + " s", x, y + 175);
  if (deployTime == 0) text("Deploy Time : Not Deployed", x, y + 200);
  else text("Deploy Time : " + nf(deployTime, 0, 1) + " s (after launch)", x, y + 200);
}

String degToDMS(float deg, boolean isLon) {
  String dirStr = isLon ? (deg >= 0 ? "E" : "W") : (deg >= 0 ? "N" : "S");
  float absDeg = abs(deg);
  int d = int(absDeg);
  float remainder = (absDeg - d) * 60;
  int m = int(remainder);
  float s = (remainder - m) * 60;
  return d + "°" + m + "'" + nf(s, 0, 2) + "\"" + dirStr;
}

////////////////////////////////////////////////////////////////
// デプロイインジケータ
////////////////////////////////////////////////////////////////
void drawDeployIndicator() {
  int cx = 240;
  int cy = 160;
  float r = 40;

  if (deployTime > 0) fill(120, 255, 100);
  else fill(255, 60, 60);

  noStroke();
  ellipse(cx, cy, r, r);

  fill(255);
  textAlign(CENTER, CENTER);
  textSize(14);
  text("DEPLOY", cx, cy + 40);
  textAlign(LEFT, CENTER);
}

////////////////////////////////////////////////////////////////
// グラフ描画
////////////////////////////////////////////////////////////////
void drawGraphs() {
  int gx = 370;
  int gy = 70;
  int gw = 580;
  int gh = 150;
  int gap = 30;

  drawSingleGraph("Altitude (m)", altHistory, 0, 300, gx, gy, gw, gh, color(0, 255, 120));
  drawSingleGraph("Pressure (hPa)", presHistory, 970, 1020, gx, gy + gh + gap, gw, gh, color(0, 150, 255));
  drawSingleGraph("Acceleration (m/s²)", accelHistory, -10, 50, gx, gy + (gh + gap) * 2, gw, gh, color(255, 180, 50));
}

void drawSingleGraph(String label, float[] data, float minVal, float maxVal, int x, int y, int w, int h, int c) {
  fill(20);
  stroke(150);
  rect(x - 5, y - 5, w + 10, h + 10);

  textSize(15);
  fill(200);
  textAlign(RIGHT, CENTER);

  for (int i = 0; i <= 5; i++) {
    float py = y + map(i, 0, 5, h, 0);
    stroke(60);
    line(x, py, x + w, py);
    float val = map(i, 0, 5, minVal, maxVal);
    noStroke();
    text(nf(val, 0, 1), x - 10, py);
  }

  textAlign(LEFT, CENTER);

  stroke(c);
  noFill();
  beginShape();
  for (int i = 0; i < maxPoints; i++) {
    int idx = (index + i) % maxPoints;
    float v = constrain(data[idx], minVal, maxVal);
    float px = x + map(i, 0, maxPoints - 1, 0, w);
    float py = y + h - map(v, minVal, maxVal, 0, h);
    vertex(px, py);
  }
  endShape();

  fill(255);
  noStroke();
  textSize(12);
  text(label, x, y - 15);
}

////////////////////////////////////////////////////////////////
// テレメトリ開始ボタン
////////////////////////////////////////////////////////////////
void drawControlButton() {
  int bx = 20;
  int by = height - 160;
  int bw = 180;
  int bh = 40;

  fill(telemetryActive ? color(0, 200, 0) : color(100));
  stroke(255);
  rect(bx, by, bw, bh, 8);

  fill(255);
  textAlign(CENTER, CENTER);
  textSize(14);
  text(telemetryActive ? "Telemetry Active" : "Start Telemetry", bx + bw / 2, by + bh / 2);
  textAlign(LEFT, CENTER);
}

////////////////////////////////////////////////////////////////
// ★シリアル送信 UI
////////////////////////////////////////////////////////////////
void drawSendPanel() {
  fill(255);
  textSize(14);
  text("Send Command to Device:", 20, height - 110);

  // 入力欄
  fill(40);
  stroke(200);
  rect(20, height - 90, 300, 30);
  fill(255);
  text(sendText, 25, height - 75);

  // SEND ボタン
  fill(70, 160, 255);
  rect(330, height - 90, 100, 30, 5);
  fill(0);
  textAlign(CENTER, CENTER);
  text("SEND", 380, height - 75);
  textAlign(LEFT, CENTER);
}

////////////////////////////////////////////////////////////////
// 入力処理
////////////////////////////////////////////////////////////////
void keyTyped() {
  if (key == BACKSPACE) {
    if (sendText.length() > 0)
      sendText = sendText.substring(0, sendText.length() - 1);
  } else if (key == ENTER || key == RETURN) {
    sendCommand();
  } else if (key > 31) {
    sendText += key;
  }
}

////////////////////////////////////////////////////////////////
// ★送信処理本体
////////////////////////////////////////////////////////////////
void sendCommand() {
  if (myPort != null && sendText.length() > 0) {
    myPort.write("CMD:" + sendText + "\n");
    println("Sent: CMD:" + sendText);
  }
  sendText = "";
}

////////////////////////////////////////////////////////////////
// マウス押下（ボタン処理）
////////////////////////////////////////////////////////////////
void mousePressed() {
  int bx = 20;
  int by = height - 160;
  int bw = 180;
  int bh = 40;

  // テレメトリ開始
  if (mouseX > bx && mouseX < bx + bw && mouseY > by && mouseY < by + bh) {
    telemetryActive = !telemetryActive;
    if (telemetryActive) launchStart = millis();
  }

  // SEND ボタン
  if (mouseX > 330 && mouseX < 430 && mouseY > height - 90 && mouseY < height - 60) {
    sendCommand();
  }
}

////////////////////////////////////////////////////////////////
// ★ Processing 終了時の確実な保存
////////////////////////////////////////////////////////////////
void exit() {
  if (logger != null) {
    logger.flush();
    logger.close();
  }
  super.exit();
}
