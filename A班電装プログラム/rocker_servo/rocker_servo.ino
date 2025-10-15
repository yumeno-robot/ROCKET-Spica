#include <Servo.h>

Servo myservo;  // サーボ用オブジェクト


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



void setup() {
  myservo.attach(6, 500, 2400); // GP6ピンをPWM出力に割り当て

  pinMode(2, OUTPUT);


  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);

  pinMode(26, INPUT);//フォトトランジスタ
  pinMode(27, INPUT);//スイッチ
  pinMode(28, INPUT);//フライトピン
}

void loop() {

  Serial.print("フォトトランジスタ");
  Serial.print(analogRead(26));
  Serial.print("　　");

  Serial.print("　　スイッチ");
  Serial.print(analogRead(27));
  Serial.print("　　");

  Serial.print("フライトピン");
  Serial.println(analogRead(28));



  /*
    // 0度 → 90度 → 180度 と順番に動かす
    myservo.write(0);
    delay(1000);
    myservo.write(180);
    delay(1000);
    myservo.write(0);
    delay(1000);




    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(15, HIGH);

    delay(1000);

    digitalWrite(10, LOW);
    delay(500);
    digitalWrite(11, LOW);
    delay(500);
    digitalWrite(15, LOW);
    delay(500);
    digitalWrite(14, LOW);
    delay(500);

    delay(500);

    for (int i = 0; i < 8; i++) {
      tone(2, notes[i], 400); // 400ms 音を鳴らす
      delay(250);                       // ちょっと休む
    }

    ]*/
}
