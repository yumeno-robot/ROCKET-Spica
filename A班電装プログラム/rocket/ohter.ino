void checkBurnEnd() {

  float z = Return_AccelZ();

  // 1. Z軸が一度 2G を超えたかチェック
  if (z >= ACCEL_THRESHOLD_HIGH) {
    accelOver2G = true;
  }

  // 2. 2G超過済みなら、閾値を下回ったかチェック
  if (accelOver2G && z <= ACCEL_THRESHOLD_LOW) {
    burnEndFlag = true;       // 燃焼終了
    accelOver2G = false;      // 状態リセット（必要に応じて）
  }
}
