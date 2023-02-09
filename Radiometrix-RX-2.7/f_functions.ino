// Reset function in software to kick start
void resetFunc() {
}

void beep() {   // Sounder output
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);   // Test. Turn the sounder on D7 on
  delay(500);              // wait
  digitalWrite(7, LOW);   // turn off
}
// Serial procedures from:
//  https://forum.arduino.cc/t/serial-input-basics-updated/382007/2

void recvBytesWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  byte startMarker = 0x11; // start marker
  byte endMarker = 0x18; // end marker
  byte rb;


  while (mySerial.available() > 0 && newData == false) {
    rb = mySerial.read();

    if (recvInProgress == true) {
      if (rb != endMarker) {
        receivedBytes[ndx] = rb;
        ndx++;
        if (ndx >= numBytes) {
          ndx = numBytes - 1;
        }
      }
      else {
        receivedBytes[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        numReceived = ndx;  // save the number for use when printing
        ndx = 0;
        newData = true;
      }
    }

    else if (rb == startMarker) {
      recvInProgress = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    //  Serial.print("Data in(DEC)");
    for (byte n = 0; n < numReceived; n++) {
      Serial.print(receivedBytes[n], DEC);
      buf[n] = receivedBytes[n]; // Capture serial data
      Serial.print(' ');
    }
    Serial.println();
    newData = false;
  }
}
