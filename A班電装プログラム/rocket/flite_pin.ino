

/*

  bool Write_Sd_FirstTIME = false;
  double flight_time;

  void Read_filtepin() {
  int situation_flitepin = analogRead(28);
  if (situation_flitepin < 500 || Write_Sd_FirstTIME == true) {//開いていないもしくは一回書いてる。
    return;
  } else if (situation_flitepin > 500 && Write_Sd_FirstTIME == false) {//開いてるそして一回も書いてない。
    Write_Sd_FirstTIME = true;
    flight_time = millis();
  }

  }

  int Return_Filght_time() {
  return flight_time;
  }

*/


//解放機構の仕様　右閉じる　左開ける　「あける」開ける　※なので右にいても閉じてる時がある。　※通常動作は右に置いておく。

void controlMotor() {
  int toggle = Read_tlg_motor();

  // ① トワイライトによる絶対解放指令
  if (motor_flag == true) {
    servo(90);              // 開ける
    motor_forced_open = true;  // 強制開放状態を記録
    motor_flag = false;  //もう一度「開ける」が来ても開けれるようになる
  }
  
    // ② トワイライトで「解放しない」宣言が来たら解除可能に
    else if (motor_flag == false) {
      // トグルスイッチによる制御
      if (toggle == 0) {
        servo(90);              // トグルで開ける
        motor_forced_open = false;
      }
      else if (toggle == 1) {
        // トワイライト強制開放中は閉じない
        if (!motor_forced_open) {
          servo(0);             // 閉じる
        }
      }
    }
  
}
