void loop()
{
  wdt_reset(); //reset Watchdog in main loop for Failsafe
  currentMillis = millis(); // capture time for Failsafe function

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

  if (rssi >= 120) { // 120 corresponds to -118 dBm
    // reset the failsafe timer if rssi is OK
    previousMillis = currentMillis;
    digitalWrite(LED, HIGH); // Turn receive LED On
  }
  else {
    digitalWrite(LED, LOW); // Turn receive LED Off
  }

  if (man.receiveComplete())
  {
    uint8_t receivedSize = 0;

    //do something with the data in 'buffer' here before you start receiving to the same buffer again
    receivedSize = buffer[0];

    for (uint8_t i = 1; i < receivedSize; i++)
      Serial.write(buffer[i]);

    man.beginReceiveArray(BUFFER_SIZE, buffer);
    moo = ++moo % 2;
    digitalWrite(LED_PIN, moo);
  }

#ifdef DEBUG
  // Print stick outputs
  for (int i = 0; i < 6; i++)   //display all channels
  {
    Serial.print(buffer[i]);
    Serial.print("\t"); //tab
    // }
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

  servo1.writeMicroseconds(buffer[0] * 4 + 1000);
  servo2.writeMicroseconds(buffer[1] * 4 + 1000);
  servo3.writeMicroseconds(buffer[2] * 4 + 1000);
  servo4.writeMicroseconds(buffer[3] * 4 + 1000);
  servo5.writeMicroseconds(buffer[4] * 4 + 1000);
  servo6.writeMicroseconds(buffer[5] * 4 + 1000);

  // Failsafe section
  // check time interval (milliseconds)
  if (currentMillis - previousMillis >= interval) {

    Serial.println("Failsafe on");
    Serial.print("\n"); //newline
    // Move servos to position
    servo1.writeMicroseconds(140 * 4 + 1000 );
    servo2.writeMicroseconds( 20 * 4 + 1000 );
    servo3.writeMicroseconds(140 * 4 + 1000 );
    servo4.writeMicroseconds(140 * 4 + 1000 );
    servo5.writeMicroseconds( 10 * 4 + 1000 );
    servo6.writeMicroseconds(140 * 4 + 1000 );

    delay(200);

    // Reset the processor
    resetFunc();
  }

}
