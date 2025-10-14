

#include <SPI.h>
#include <SD.h>



volatile double writeData[10];
volatile int writeCount = 0;
volatile bool writeRequest = false;


bool error_SD = true;
bool error_BNO055 = true;
bool error_BNE280 = true;
bool error_GPS = true;


void setup() {

  delay(5000);

  Serial.begin(115200);

  Setup_other_pin();
  Setup_SD();
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.print("BME280 ::温度　気圧　高さ　湿度");
  dataFile.println();
  dataFile.flush();
  dataFile.close();

  Setup_BNO055();
  Setup_BME280();



  buzz_start();
}



void setup1() {

}



void loop() {

  Read_BNO055();
  Read_BME280();



}




void loop1() {

  if (error_SD) {
    if (writeRequest) {
      writeRequest = false;
      File dataFile = SD.open("datalog.txt", FILE_WRITE);


      if (dataFile) {
        dataFile.print(millis());
        dataFile.print(",");
        dataFile.print("   ");
        for (int i = 0; i < writeCount; i++) {
          dataFile.print(writeData[i]);
          dataFile.print(",");
          dataFile.print("   ");
        }
        dataFile.println();
        dataFile.flush();
        dataFile.close();
      }
    }
  } else {
    Serial.println("NO SD______________________________________________");
  }

}
