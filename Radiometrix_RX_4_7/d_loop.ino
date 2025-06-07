void loop() {
  // wdt_reset();               //reset Watchdog in main loop for Failsafe
  currentMillis = millis();  // capture time for Failsafe function

  // read and print RSSI
  int rssiraw = analogRead(A4);
  rssi = 0.1609641 * rssiraw - 140.219;  //using calibration spreadsheet and Radiometrix datasheet

  /////////////////////////////////////////////////////////////////
  // Failsafe test
  if (buffer[6] == 127) {            // If transmitted code is present, reset timer
    previousMillis = currentMillis;  // reset failsafe time counter
                                     // Test rssi value routine
                                     // rssi = -65;
                                     //if (rssi > -80) {                  // Greater  than -80dBm is Ok. Was -90 then -85 then -75. Not very reliable - reduces range
                                     // if (rssi > -90 && buffer[6] != 127) {  // Greater than -90dBm is Ok, and transmitted code is not there
                                     //  wdt_reset();                         //reset Watchdog in main loop for Failsafe
                                     // buffer[6] = 0;                   // reset TX ID value
    digitalWrite(LED, HIGH);         // Turn receive LED On
                                     // previousMillis = currentMillis;  // reset failsafe time counter

    if (man.receiveComplete()) {

      uint8_t receivedSize = 0;

      //do something with the data in 'buffer' here before you start receiving to the same buffer again
      receivedSize = buffer[0];

      for (uint8_t i = 1; i < receivedSize; i++)
        Serial.write(buffer[i]);

      man.beginReceiveArray(BUFFER_SIZE, buffer);
      moo = ++moo % 2;
      digitalWrite(LED_PIN, moo);
    }

    // Print stick outputs
    for (int i = 0; i < 7; i++)  //display all channels
    {
      Serial.print(buffer[i]);
      Serial.print("\t");  //tab
      // }
    }
    Serial.print("\n");  //newline

    Serial.print("RSSI=");
    Serial.print(rssi);
    Serial.print("\n");  //newline

#ifdef DEBUG
    // Test servo
    servo1.writeMicroseconds(1500);  // set servo to mid-point
                                     // Move servos to position

    // Output to Channel 2 on/off indicator
    if (buffer[1] < 135) {
      digitalWrite(CHAN2_ONOFF, LOW);  // Turn Channel 2 on/off indicator On
    } else {
      digitalWrite(CHAN2_ONOFF, HIGH);  // Turn Channel 2 on/off indicator On
    }

#endif

    servo1.writeMicroseconds(buffer[0] * 4 + 1000);
    servo2.writeMicroseconds(buffer[1] * 4 + 1000);
    servo3.writeMicroseconds(buffer[2] * 4 + 1000);
    servo4.writeMicroseconds(buffer[3] * 4 + 1000);
    servo5.writeMicroseconds(buffer[4] * 4 + 1000);
    servo6.writeMicroseconds(buffer[5] * 4 + 1000);

    buffer[6] = 0;  // Reset this buffer fro failsafe
  }

  /////////////////////////////////////////////////////////////////
  // Failsafe section
  else {
    // check time interval (milliseconds)
    if (currentMillis - previousMillis >= interval) {
      digitalWrite(LED, LOW);  // Turn receive LED Off
      Serial.println("Failsafe on");

      // Failsafe values for servo outputs
      int buffer[7] = { 160, 140, 140, 10, 10, 140, 0 };  // Test values for servo outputs

      // Move servos to position

      servo1.writeMicroseconds(buffer[0] * 4 + 1000);
      servo2.writeMicroseconds(buffer[1] * 4 + 1000);
      servo3.writeMicroseconds(buffer[2] * 4 + 1000);
      servo4.writeMicroseconds(buffer[3] * 4 + 1000);
      servo5.writeMicroseconds(buffer[4] * 4 + 1000);
      servo6.writeMicroseconds(buffer[5] * 4 + 1000);

#ifdef DEBUG
      // Print stick outputs
      for (int i = 0; i < 7; i++)  //display all channels
      {
        Serial.print(buffer[i]);
        Serial.print("\t");  //tab
        // }
      }
      Serial.print("\n");  //newline

      Serial.print("RSSI=");
      Serial.print(rssi);
      Serial.print("\n");  //newline
#endif

      delay(200);

      // Reset the processor
      resetFunc();
    }
  }
}
