void loop()
{
  wdt_reset(); //reset Watchdog in main loop for Failsafe
  currentMillis = millis(); // capture time for Failsafe function

  // reset the failsafe timer if correct TX ID code is received (127)
  if (buffer[6] == 127) {
    previousMillis = currentMillis;
  }
  buffer[6] = 0; // Cancel TX ID value

  // read and print RSSI
  rssi = analogRead(A4);

#ifdef DEBUG
  counter = counter + 1; // reduces amount of printing
  if (counter > 5) {
    Serial.print("RSSI=");
    Serial.print(rssi);
    Serial.print("\n"); //newline
    counter = 0; // reset counter

  }
#endif

  if (man.receiveComplete())
  {
    uint8_t receivedSize = 0;

    for (uint8_t i = 1; i < receivedSize; i++)
      Serial.write(buffer[i]);

    man.beginReceiveArray(BUFFER_SIZE, buffer);

    digitalWrite(LED, HIGH);
  }

#ifdef DEBUG
  // Print stick outputs
  for (int i = 0; i < 7; i++)   //display all channels
  {
    Serial.print(buffer[i]);
    Serial.print("\t"); //tab
  }
  Serial.print("\n"); //newline

  // Test servo
  // servo1.writeMicroseconds(1500);  // set servo to mid-point

  // Move servos to position

#endif

  // Output to Channel 2 on/off indicator
  if (buffer[1] < 135) {
    digitalWrite(CHAN2_ONOFF, LOW); // Turn Channel 2 on/off indicator On
  }  else {
    digitalWrite(CHAN2_ONOFF, HIGH); // Turn Channel 2 on/off indicator On
  }
  
  // Move servos to position
  servo1.writeMicroseconds(buffer[0] * 4 + 1000);
  servo2.writeMicroseconds(buffer[1] * 4 + 1000);
  servo3.writeMicroseconds(buffer[2] * 4 + 1000);
  servo4.writeMicroseconds(buffer[3] * 4 + 1000);
  servo5.writeMicroseconds(buffer[4] * 4 + 1000);
  servo6.writeMicroseconds(buffer[5] * 4 + 1000);

  // Failsafe section
  // Check time interval (milliseconds)
  if (currentMillis - previousMillis >= interval) {
    digitalWrite(LED, LOW); // Turn receive LED Off
    Serial.println("Failsafe on");
    Serial.print("\n"); //newline

    // Put in Failsafe values
    buffer[0] = 140;
    buffer[1] = 20;
    buffer[2] = 140;
    buffer[3] = 140;
    buffer[4] = 140;
    buffer[5] = 10;
    buffer[6] = 123;

    // Move servos to position
    servo1.writeMicroseconds(buffer[0] * 4 + 1000);
    servo2.writeMicroseconds(buffer[1] * 4 + 1000);
    servo3.writeMicroseconds(buffer[2] * 4 + 1000);
    servo4.writeMicroseconds(buffer[3] * 4 + 1000);
    servo5.writeMicroseconds(buffer[4] * 4 + 1000);
    servo6.writeMicroseconds(buffer[5] * 4 + 1000);

    delay(200);

    // Reset the processor
    resetFunc();
  }

}
