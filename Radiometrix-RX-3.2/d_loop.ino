void loop()
{
  wdt_reset(); //reset Watchdog in main loop for Failsafe
  currentMillis = millis(); // capture time for Failsafe function

  // read and print RSSI
  rssi = analogRead(A4);
  counter = counter + 1; // reduces amount of printing
  if (counter > 10) {
    Serial.print("RSSI=");
    Serial.print(rssi);
    Serial.print("\n"); //newline
    counter = 0; // reset counter
  }

  if (rssi >= 400) {
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

    // Serial.println();


    // Print stick outputs
    for (int i = 0; i < 6; i++)   //display all channels
    {
      if (buffer[i] != -1 && buffer[i] != 255) { // Screen out common data values
        Serial.print(buffer[i]);
        Serial.print("\t"); //tab
      }

      man.beginReceiveArray(BUFFER_SIZE, buffer);
      moo = ++moo % 2;
      digitalWrite(LED_PIN, moo);
    }
  }

  // delay(200);

  myservo.write(buffer[1]);              // tell servo to go to position in variable 'pos'
  delay(15);                       // waits 15 ms for the servo to reach the position


  int  val1 = buffer[0] * 4 + 1000;
  int  val2 = buffer[1] * 4 + 1000;
  int  val3 = buffer[2] * 4 + 1000;
  int  val4 = buffer[3] * 4 + 1000;
  int  val5 = buffer[4] * 4 + 1000;
  int  val6 = buffer[5] * 4 + 1000;

  // Test
  // val2 = 1500;
  // Serial.print(val2);

  Serial.print("\n"); //newline

  /*
    // Servo.writes to position
    ch1.write(val1);
    ch2.write(val2);
    ch3.write(val3);
    ch4.write(val4);
    ch5.write(val5);
    ch6.write(val6);

    delay(10);

  */

  // Failsafe section
  // check time interval (milliseconds)
  if (currentMillis - previousMillis >= interval) {

    Serial.println("Failsafe values");
    // FAILSAFE values (1000 to 2000)
    val1 = 1600; // This is often for steering, so slight turn
    val2 = 1130; // Planes to surface
    val3 = 1750;
    val4 = 1750;
    val5 = 1005;
    val6 = 1005;

    Serial.print((val1 - 1000) / 4);
    Serial.print("\t"); //tab
    Serial.print((val2 - 1000) / 4);
    Serial.print("\t"); //tab
    Serial.print((val3 - 1000) / 4);
    Serial.print("\t"); //tab
    Serial.print((val4 - 1000) / 4);
    Serial.print("\t"); //tab
    Serial.print((val5 - 1000) / 4);
    Serial.print("\t"); //tab
    Serial.print((val6 - 1000) / 4);
    Serial.print("\t"); //tab

    /*
        // servo.writes to position
        ch1.write(val1);
        ch2.write(val2);
        ch3.write(val3);
        ch4.write(val4);
        ch5.write(val5);
        ch6.write(val6);
    */

    delay(200);

    // Reset the processor
    resetFunc();
  }

}
