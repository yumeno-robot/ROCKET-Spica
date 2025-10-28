

#include <TinyGPSPlus.h>
TinyGPSPlus gps;


void Setup_GPS() {
  Serial1.begin(9600);
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
