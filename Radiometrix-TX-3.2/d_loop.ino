void loop()
{
  unsigned long currentMillis = millis(); // set timer routine (Thanks to Jonathan for the suggestion to reduce rate of data transmission.)

  for (int i = 0; i < 6; i++)   //display all channels
  {
    // This section scales analogue pulse value in milli secs to 0 to 255 for radio control.
    // Values need adjusting for different TXs. A better, but more complicated system, would be to do an initial calibration of sticks, saved to memory.
    ppmValues[i] = map(ppmValues[i], 1800, 4140, 0, 255);
    ppmValues[i] = constrain (ppmValues[i], 1, 255);
    data[i] = ppmValues[i]; // Store in Byte array

  }
#ifdef DEBUG
  // Stick output
  Serial.print("Stick output ");
  Serial.print("\n"); //newline
  for (int i = 0; i < 7; i++)   //display all channels
  {
    Serial.print(data[i]);
    Serial.print("\t"); //tab
  }
#endif

  if (currentMillis - TX_previousMillis > TX_interval)
  {
    // time TX_interval has been reached - run code here

    man.transmitArray(datalength, data);
    digitalWrite(LED, LOW);

    TX_previousMillis = currentMillis; // update the time we have run this event so we can compare with future time
  }

  delay(5); // This delay seems to be necessary for some reason?!

}
