void loop()
{
  wdt_reset(); //reset Watchdog in main loop for Failsafe
  currentMillis = millis(); // capture time for Failsafe function

  // read and print RSSI
  rssi = analogRead(A4);
  Serial.print("RSSI=");
  Serial.print(rssi);
  // Serial.print("\n"); //newline
  Serial.print("\t"); //tab

  if (rssi >= 400) {
    // reset the failsafe timer if rssi is OK
    previousMillis = currentMillis;
    digitalWrite(LED, HIGH); // Turn receive LED On
  }
  else {
    digitalWrite(LED, LOW); // Turn receive LED Off
  }

  // set the data rate for the SoftwareSerial port
  mySerial.begin(300);
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  // Pause for 10 milliseconds before the next reading
  // delay(10);

  // /*
  // Test values for servo outputs
  int buf[6] = {20, 75, 100, 150, 200, 255}; // Test values for servo outputs

  // Read data
  for (int i = 0; i < 6; i++)   //display all channels
  {
    buf[i] = (mySerial.read());
  }

  // Print stick outputs
  for (int i = 0; i < 6; i++)   //display all channels
  {
    Serial.print(buf[i]);
    Serial.print("\t"); //tab
  }
  // */

  // print array and convert to val for Servo2
  // Serial.print("Stick output  ");
  // Serial.print("\n"); //newline

  int  val1 = buf[0] * 4 + 1000;
  int  val2 = buf[1] * 4 + 1000;
  int  val3 = buf[2] * 4 + 1000;
  int  val4 = buf[3] * 4 + 1000;
  int  val5 = buf[4] * 4 + 1000;
  int  val6 = buf[5] * 4 + 1000;

  Serial.print("\n"); //newline

  // Servo.writes to position
  ch1.write(val1);
  ch2.write(val2);
  ch3.write(val3);
  ch4.write(val4);
  ch5.write(val5);
  ch6.write(val6);

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

    // servo.writes to position
    ch1.write(val1);
    ch2.write(val2);
    ch3.write(val3);
    ch4.write(val4);
    ch5.write(val5);
    ch6.write(val6);

    delay(200);

    // Reset the processor
    //  resetFunc();
  }
}
