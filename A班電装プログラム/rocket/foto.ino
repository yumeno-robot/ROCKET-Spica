



void Read_foto() {
  int y = analogRead(26);


  if (y > 500) {
    if (!First_Release_completion) {
      Release_completion_time = millis();
      First_Release_completion = true;
    }

  }
  if (debug) {
    Serial.println(y);
  }



}
