
#include <Servo.h>
Servo myservo;


int notes[] = {
  262, // ド (C4)
  294, // レ (D4)
  330, // ミ (E4)
  349, // ファ (F4)
  392, // ソ (G4)
  440, // ラ (A4)
  494, // シ (B4)
  523  // ド (C5)
};


void Setup_other_pin() {
  myservo.attach(6, 500, 2400); // GP6ピンをPWM出力に割り当て

  pinMode(2, OUTPUT);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);


  pinMode(26, INPUT);//フォトトランジスタ
  pinMode(27, INPUT);//スイッチ
  pinMode(28, INPUT);//フライトピン
}



void buzz_start() {
  tone(2, 880, 150);
  delay(200);
  //tone(2, 880, 150);
  //delay(200);
  //tone(2, 987, 400);
  //delay(450);
  //tone(2, 1319, 600);
  //delay(700);
  //noTone(2);
}


void buzz_error() {
  tone(2, 2000, 400);
  delay(100);
  tone(2, 2000, 400);
  delay(100);
  tone(2, 2000, 400);
  delay(100);
  tone(2, 2000, 400);
  delay(100);
  tone(2, 2000, 400);
  delay(100);
  tone(2, 2000, 400);
  delay(100);
  tone(2, 2000, 400);
  delay(100);
}



void LED_SD() {
  digitalWrite(10, LOW);
  error_SD = false;
}
void LED_BNO055() {
  digitalWrite(11, LOW);
  error_BNO055 = false;
}
void LED_BNE280() {
  digitalWrite(15, LOW);
  error_BNE280 = false;
}
void LED_GPS() {
  digitalWrite(14, LOW);
  error_GPS = false;
}



void servo(int power) {
  myservo.write(power);
}
