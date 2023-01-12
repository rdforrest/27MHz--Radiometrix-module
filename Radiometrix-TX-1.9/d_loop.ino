void loop()
{
  for (int i = 0; i < 6; i++)   //display all channels
  {

    // This section scales analogue pulse value in milli secs to 0 to 255 for radio control.
    // Values need adjusting for different TXs. A better, but more complicated system, would be to do an initial calibration of sticks, saved to memory.
    ppmValues[i] = map(ppmValues[i], 1800, 4140, 0, 255);
    ppmValues[i] = constrain (ppmValues[i], 1, 255);
    byteArr[i] = ppmValues[i]; // Store in Byte array

    // Test - make all data one channel
    // byteArr[i] = ppmValues[1]; // Store in Byte array

    // Serial.print(ppmValues[i]); // the array value is divided by 2 to bring it back to real world value
    // Serial.print("  ");
  }

  // set the data rate for the SoftwareSerial port
  mySerial.begin(300);

  // Switch TX LED on
  digitalWrite(LED, HIGH);

  // Switch TX on, pull down to enable
  digitalWrite(Enable, LOW);

  // for (int i = 0; i <= 10; i++) {
  for (int i = 0; i <= 1; i++) {
    mySerial.write(0x55); // 55h preamble routine for wifi to settle down
    //    delay(10);
  }

  mySerial.write(0xFF); // FFh start framing bit

  mySerial.write((byte) 0x00);  // 00h to maintain DC balance

  // Data payload
  // mySerial.write(byteArr, sizeof(byteArr));
  mySerial.write(0x21); // ascii ! start bit
  mySerial.write(byteArr, 6);
  mySerial.write(0x23); // ascii #  stop bit

  // Switch TX LED off
  digitalWrite(LED, LOW);

  // Switch TX off
  digitalWrite(Enable, HIGH);

  // Stick output
  Serial.print("Stick output  ");
  Serial.print("\n"); //newline
  for (int i = 0; i < 6; i++)   //display all channels
  {
    Serial.print(byteArr[i]);
    Serial.print("\t"); //tab
  }

  // Pause for 1000 milliseconds before the next reading
  delay(1000);

}
