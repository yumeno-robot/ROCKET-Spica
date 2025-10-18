

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
      Serial.print("Latitude: ");
      Latitude = gps.location.lat();
      Serial.println(Latitude, 6);
      Serial.print("Longitude: ");
      Longitude = gps.location.lng();
      Serial.println(gps.location.lng(), 6);
    }

    if (gps.altitude.isUpdated()) {
      Serial.print("Altitude (m): ");
      Altitude = gps.altitude.meters();
      Serial.println(Altitude);
    }
  }


  Make_Twelite_Log(Latitude);
  Make_Twelite_Log(Longitude);
  Make_Twelite_Log(Altitude);


}
