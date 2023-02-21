void loop()
{
  wdt_reset(); //reset Watchdog in main loop for Failsafe
  currentMillis = millis(); // capture time for Failsafe function

  // read and print RSSI
  rssi = analogRead(A4);
  counter = counter + 1; // reduces amount of printing
  if (counter > 5) {
    Serial.print("RSSI=");
    Serial.print(rssi);
    Serial.print("\n"); //newline
    counter = 0; // reset counter
  }

  if (rssi >= 250) { // 120 corresponds to -118 dBm
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

  // Print stick outputs
  for (int i = 0; i < 6; i++)   //display all channels
  {
    if (buffer[i] != -1 && buffer[i] != 255) { // Screen out common data values
      Serial.print(buffer[i]);
      Serial.print("\t"); //tab
    }
  }
  Serial.print("\n"); //newline

  // Move servos to position
  servo1.write(buffer[0]);
  servo2.write(buffer[1]);
  servo3.write(buffer[2]);
  servo4.write(buffer[3]);
  servo5.write(buffer[4]);
  servo6.write(buffer[5]);

  delay(20);

  // Failsafe section
  // check time interval (milliseconds)
  if (currentMillis - previousMillis >= interval) {

    Serial.println("Failsafe values");
    // Move servos to position
    servo1.write(50 );
    servo2.write(50 );
    servo3.write(50 );
    servo4.write(50 );
    servo5.write(50 );
    servo6.write(50 );

    // Print stick outputs
    for (int i = 0; i < 6; i++)   //display all channels
    {
      if (buffer[i] != -1 && buffer[i] != 255) { // Screen out common data values
        Serial.print(buffer[i]);
        Serial.print("\t"); //tab
      }
    }
    Serial.print("\n"); //newline

    delay(200);

    // Reset the processor
    resetFunc();
  }

}
