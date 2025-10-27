

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);




void Setup_BNO055() {

  Wire.setSDA(8);
  Wire.setSCL(9);
  Wire.begin();

  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    LED_BNO055();
  } else {
    Serial.println("BNO055 success.");
  }

}



double Gravity_Z;


void Read_BNO055() {

  Gravity_Z = 0;

  if (error_BNO055) {
    // 各種データ取得
    sensors_event_t orientationData, angVelocityData, linearAccelData;
    sensors_event_t magnetometerData, accelerometerData, gravityData;

    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
    bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);
    // 温度
    int8_t boardTemp = bno.getTemp();
    // キャリブレーション
    uint8_t system, gyro, accel, mag;
    bno.getCalibration(&system, &gyro, &accel, &mag);



    if (debug) {
      Serial.print("Orientation (Euler): "); Serial.print("\t");
      Serial.print("X=");  Serial.print(orientationData.orientation.x);   Serial.print("\t");
      Serial.print("Y="); Serial.print(orientationData.orientation.y);   Serial.print("\t");
      Serial.print("Z="); Serial.print(orientationData.orientation.z);   Serial.println("\t");

      Serial.print("Gyroscope: "); Serial.print("\t");
      Serial.print("X=");  Serial.print(angVelocityData.gyro.x);   Serial.print("\t");
      Serial.print("Y="); Serial.print(angVelocityData.gyro.y);   Serial.print("\t");
      Serial.print("Z="); Serial.print(angVelocityData.gyro.z);   Serial.println("\t");

      Serial.print("Linear Acceleration: "); Serial.print("\t");
      Serial.print("X=");  Serial.print(linearAccelData.acceleration.x);   Serial.print("\t");
      Serial.print("Y="); Serial.print(linearAccelData.acceleration.y);   Serial.print("\t");
      Serial.print("Z="); Serial.print(linearAccelData.acceleration.z);   Serial.println("\t");

      Serial.print("Magnetometer: "); Serial.print("\t");
      Serial.print("X=");  Serial.print(magnetometerData.magnetic.x);   Serial.print("\t");
      Serial.print("Y="); Serial.print(magnetometerData.magnetic.y);   Serial.print("\t");
      Serial.print("Z="); Serial.print(magnetometerData.magnetic.z);   Serial.println("\t");

      Serial.print("Accelerometer: "); Serial.print("\t");
      Serial.print("X=");  Serial.print(accelerometerData.acceleration.x);   Serial.print("\t");
      Serial.print("Y="); Serial.print(accelerometerData.acceleration.y);   Serial.print("\t");
      Serial.print("Z="); Serial.print(accelerometerData.acceleration.z);   Serial.println("\t");

      Serial.print("Gravity: "); Serial.print("\t");
      Serial.print("X=");   Serial.print(gravityData.acceleration.x);   Serial.print("\t");
      Serial.print("Y=");  Serial.print(gravityData.acceleration.y);   Serial.print("\t");
      Serial.print("Z=");  Serial.print(gravityData.acceleration.z);   Serial.println("\t");
      Serial.print("Temperature: "); Serial.println(boardTemp);

      Serial.print("Calibration: Sys="); Serial.print(system); Serial.println("\t");
      Serial.print("Gyro=");  Serial.print(gyro);   Serial.print("\t");
      Serial.print("Accel="); Serial.print(accel);  Serial.print("\t");
      Serial.print("Mag=");   Serial.println(mag);  Serial.println("\t");
    }
    //delay(BNO055_SAMPLERATE_DELAY_MS);
    Gravity_Z = accelerometerData.acceleration.z;

    //double val_BNO055[22] = {orientationData.orientation.x, orientationData.orientation.y, orientationData.orientation.z, angVelocityData.gyro.x, angVelocityData.gyro.y, angVelocityData.gyro.z, linearAccelData.acceleration.x, linearAccelData.acceleration.y, linearAccelData.acceleration.z, magnetometerData.magnetic.x, magnetometerData.magnetic.y, magnetometerData.magnetic.z, accelerometerData.acceleration.x, accelerometerData.acceleration.y, accelerometerData.acceleration.z, gravityData.acceleration.x, gravityData.acceleration.y, gravityData.acceleration.z, boardTemp, gyro, accel, mag};
    double val_BNO055[10] = {
      accelerometerData.acceleration.x,
      accelerometerData.acceleration.y,
      accelerometerData.acceleration.z,
      gravityData.acceleration.x,
      gravityData.acceleration.y,
      gravityData.acceleration.z,
      static_cast<double>(boardTemp),  // 明示的に double に変換
      static_cast<double>(gyro),
      static_cast<double>(accel),
      static_cast<double>(mag)
    };

    
    //storeData(val_BNO055, 10);




  } else {
    if (debug) {
      Serial.println("NO BNO055__________________________________________");
    }
  }


}

double Return_AccelZ() {
  Serial.println(Gravity_Z);
  return Gravity_Z;
}
