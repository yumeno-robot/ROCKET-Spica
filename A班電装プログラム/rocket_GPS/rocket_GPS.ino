#include <TinyGPS++.h>
#include <TinyGPSPlus.h>
TinyGPSPlus gps;


void setup() {

  delay(4000);
  Serial.begin(115200);
  Serial.println("__");
  Serial1.begin(9600);    // Serial monitor       // GPS module
}

void loop() {
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());

    if (gps.location.isUpdated()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
    }

    if (gps.altitude.isUpdated()) {
      Serial.print("Altitude (m): ");
      Serial.println(gps.altitude.meters());
    }
  }
}
