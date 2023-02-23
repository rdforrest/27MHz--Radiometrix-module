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

  // Print stick outputs
  for (int i = 0; i < 6; i++)   //display all channels
  {
    // if (buffer[i] != -1 && buffer[i] != 255) { // Screen out common data values
    Serial.print(buffer[i]);
    Serial.print("\t"); //tab
    // }
  }
  Serial.print("\n"); //newline

  // Move servos to position and adjust from 255 max to 180 degrees max.

  /*
    val = map(val, 0, 1023, 0, 180);

    servo1.write(buffer[0] * 180 / 255);
    servo2.write(buffer[1] * 180 / 255);
    servo3.write(buffer[2] * 180 / 255);
    servo4.write(buffer[3] * 180 / 255);
    servo5.write(buffer[4] * 180 / 255);
    servo6.write(buffer[5] * 180 / 255);
  */

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
    servo1.write(140 );
    servo2.write(20 );
    servo3.write(100 );
    servo4.write(100 );
    servo5.write(10 );
    servo6.write(10 );

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
