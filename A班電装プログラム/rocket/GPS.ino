

#include <TinyGPSPlus.h>
TinyGPSPlus gps;


void Setup_GPS() {
  Serial1.begin(9600);
}

void Read_GPS_time() {
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());



    // ---- 日付と時間の更新 ----
    if (gps.date.isUpdated() || gps.time.isUpdated()) {

      // 日付
      int year_  = gps.date.year();
      int month_ = gps.date.month();
      int day_   = gps.date.day();

      // 時刻（UTC）
      int hour_   = gps.time.hour();
      int minute_ = gps.time.minute();
      int second_ = gps.time.second();

      // デバッグ表示
      if (debug) {
        Serial.print("Date: ");
        Serial.print(year_);
        Serial.print("/");
        Serial.print(month_);
        Serial.print("/");
        Serial.println(day_);

        Serial.print("Time (UTC): ");
        Serial.print(hour_);
        Serial.print(":");
        Serial.print(minute_);
        Serial.print(":");
        Serial.println(second_);
      }

      File dataFile = SD.open("datalog.txt", FILE_WRITE);

      if (dataFile) {
        dataFile.print(year_);
        dataFile.print("年");
        dataFile.print(month_);
        dataFile.print("月");
        dataFile.print(day_);
        dataFile.print("日,");
        dataFile.print(hour_);
        dataFile.print("時");
        dataFile.print(minute_);
        dataFile.print("分");
        dataFile.print(second_);

        dataFile.println();
        dataFile.flush();
        dataFile.close();
      }
    }


  }
}

void Read_GPS() {
  double Latitude;
  double Longitude;
  double Altitude;

  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());

    if (gps.location.isUpdated()) {
      Latitude = gps.location.lat();
      Longitude = gps.location.lng();
      if (debug) {
        Serial.print("Latitude: ");
        Serial.println(Latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
      }
    }

    if (gps.altitude.isUpdated()) {
      Altitude = gps.altitude.meters();
      if (debug) {
        Serial.print("Altitude (m): ");
        Serial.println(Altitude);
      }
    }
  }

  if (!SD_WRITE_NOW) {
    double val_GPS[3] = {Latitude, Longitude, Altitude};
    storeData(val_GPS, 3);
  }

  Make_Twelite_Log(Latitude);
  Make_Twelite_Log(Longitude);
  Make_Twelite_Log(Altitude);


}
