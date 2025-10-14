
const int _MISO = 16;
const int _MOSI = 19;
const int _CS = 17;
const int _SCK = 18;

const int RP_CLK_GPIO = -1; // Set to CLK GPIO
const int RP_CMD_GPIO = -1; // Set to CMD GPIO
const int RP_DAT0_GPIO = -1; // Set to DAT0 GPIO. DAT1..3 must be consecutively connected.






void Setup_SD() {

  bool sdInitialized = false;
  if (RP_CLK_GPIO >= 0) {
    sdInitialized = SD.begin(RP_CLK_GPIO, RP_CMD_GPIO, RP_DAT0_GPIO);
  } else {
    if (_MISO == 0 || _MISO == 4 || _MISO == 16) {
      SPI.setRX(_MISO);
      SPI.setTX(_MOSI);
      SPI.setSCK(_SCK);
      sdInitialized = SD.begin(_CS);
    } else if (_MISO == 8 || _MISO == 12) {
      SPI1.setRX(_MISO);
      SPI1.setTX(_MOSI);
      SPI1.setSCK(_SCK);
      sdInitialized = SD.begin(_CS, SPI1);
    } else {
      Serial.println(F("ERROR: Unknown SPI Configuration"));
      return;
    }
  }

  if (!sdInitialized) {
    Serial.println("Card failed, or not present");
    LED_SD();
  } else {
    Serial.println("SDcard success.");
  }



  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.print("start_write");
    dataFile.println();
    dataFile.flush();
    dataFile.close();
    Serial.println("write_success");
  } else {
    Serial.println("datalog.txt do not open");
  }
}



void Write_SD(double datas[], int count) {
  for (int i = 0; i < count; i++) {
    writeData[i] = datas[i];
  }
  writeCount = count;
  writeRequest = true;
}
