void loop()
{
  for (int i = 0; i < 6; i++)   //display all channels
  {
    // This section scales analogue pulse value in milli secs to 0 to 255 for radio control.
    // Values need adjusting for different TXs. A better, but more complicated system, would be to do an initial calibration of sticks, saved to memory.
    ppmValues[i] = map(ppmValues[i], 1800, 4140, 0, 255);
    ppmValues[i] = constrain (ppmValues[i], 1, 255);
    // byteArr[i] = ppmValues[i]; // Store in Byte array
    data[i] = ppmValues[i]; // Store in Byte array

  }

  // Stick output
  Serial.print("Stick output ");
  Serial.print("\n"); //newline
  for (int i = 0; i < 6; i++)   //display all channels
  {
    Serial.print(byteArr[i]);
    Serial.print("\t"); //tab
  }

  data[0] = datalength;

  man.transmitArray(datalength, data);
  moo = ++moo % 2;
  digitalWrite(LED_PIN, moo);


  delay(10);
  datalength++;
  if (datalength > 18) datalength = 2;

}
