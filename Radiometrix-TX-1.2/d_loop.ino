void loop()
{
  for (int i = 0; i < 6; i++)   //display all channels
  {

    // This section scales analogue pulse value in milli secs to 0 to 255 for radio control.
    // Values need adjusting for different TXs. A better, but more complicated system, would be to do an initial calibration of sticks, saved to memory.
    ppmValues[i] = map(ppmValues[i], 1800, 4140, 0, 255);
    ppmValues[i] = constrain (ppmValues[i], 1, 255);
    byteArr[i] = ppmValues[i]; // Store in Byte array
    Serial.print(ppmValues[i]); // the array value is divided by 2 to bring it back to real world value
    Serial.print("  ");
  }

  // Stick output
  Serial.print("Stick output  ");
  Serial.print("\n"); //newline
  for (int i = 0; i < 6; i++)   //display all channels
  {
    //  Serial.print(byteArr[i]);
    Serial.print("\t"); //tab
  }

  for (int i = 0; i <= 10; i++) {
    mySerial.println(0x55); // 55h preamble routine for wifi to settle down
    //    delay(10);
  }

  mySerial.println(0xFF); // FFh start framing bit

  mySerial.println(0x00); // 00h to maintain DC balance

  // Data payload
  //mySerial.println(111, 222, 333, 444, 555, 666, 777, 888);
  mySerial.println("Hello from TX 1234");

  /* // if (mySerial.available()) {
    // Serial.write(mySerial.read());
    mySerial.print(123, DEC);
    mySerial.print("\t");
    mySerial.println();                  // Print a line feed character
    // }

    // if (Serial.available()) {
    // mySerial.write(Serial.read());
    Serial.print(567, DEC);
    Serial.print("\t");
    Serial.println();                  // Print a line feed character
    // }
  */

  // Pause for 10 milliseconds before the next reading
  //delay(10);

}
