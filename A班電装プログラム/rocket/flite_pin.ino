
bool Write_Sd_FirstTIME = false;
double Flight = false;
double flight_time;

void Read_filtepin() {

  int situation_flitepin = analogRead(28);
  if (situation_flitepin < 500 || Write_Sd_FirstTIME == true) {//開いていないもしくは一回書いてる。
    return;
  } else if (situation_flitepin > 500 && Write_Sd_FirstTIME == false) {//初めて開いた。
    Flight = true;
    Write_Sd_FirstTIME = true;
    flight_time = millis();
  }

}

int Return_Filght_time() {
  return flight_time;
}
