/*
  | 変数名                 | 意味                     |
  | ------------------- | ---------------------- |
  | `interval`          | センサーをチェックする周期（ms）      |
  | `prevMillis`        | 前回チェックした時間             |
  | `Flight`            | フライトピンが抜けたか（trueで離床開始） |
  | `Return_AccelZ()`   | 加速度Z軸の値を返す関数           |
  | `Return_Pressure()` | 気圧センサーの値を返す関数          |
  | `overGcount`        | 連続して2Gを超えた回数           |
  | `decreaseCount`     | 連続して気圧が減った回数           |
  | `REQUIRED_COUNT`    | 一定時間継続したとみなすための必要回数    |
  | `burnEndFlag`       | 燃焼終了を検出したフラグ           |
  | `accelOver2G`       | 燃焼中を一時的に記憶するフラグ        |

*/
