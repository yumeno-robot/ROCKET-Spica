
//解放用モーター　仕様について　　右＝閉じる　左＝開ける　

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;


void Setup_BME280() {

  Wire.setSDA(8);
  Wire.setSCL(9);
  Wire.begin();

  if (! bme.begin(0x76, &Wire)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    LED_BNE280();
  } else {
    Serial.println("BME280 success.");
  }
}


double Temperature;
double Pressure;
double Approx;
double Humidity;



void Read_BME280() {

  Temperature = 0;
  Pressure = 0;
  Approx = 0;
  Humidity = 0;


  if (error_BNE280) {
    bme.takeForcedMeasurement();

    Temperature = bme.readTemperature();
    Pressure = bme.readPressure() / 100.0F;
    Approx = bme.readAltitude(SEALEVELPRESSURE_HPA);
    Humidity = bme.readHumidity();


    if (debug) {
      Serial.print("Temperature = ");
      Serial.print(Temperature);
      Serial.print(" *C");
      Serial.print("\t");

      Serial.print("Pressure = ");
      Serial.print(Pressure);
      Serial.print(" hPa");
      Serial.print("\t");

      Serial.print("Approx. Altitude = ");
      Serial.print(Approx);
      Serial.print(" m");
      Serial.print("\t");

      Serial.print("Humidity = ");
      Serial.print(Humidity);
      Serial.print(" %");
      Serial.print("\t");

      Serial.println();
    }


    if (!SD_WRITE_NOW) {
      double val_BME280[4] = {Temperature, Pressure, Approx, Humidity};
      storeData(val_BME280, 4);
    }
    
    Make_Twelite_Log(Temperature);
    Make_Twelite_Log(Pressure);
    Make_Twelite_Log(Approx);
    Make_Twelite_Log(Humidity);

  } else {
    if (debug) {
      Serial.println("NO BEM280__________________________________________");
    }
  }

}


double Return_Pressure() {
  return Pressure;
}
