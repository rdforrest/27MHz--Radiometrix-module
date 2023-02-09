void loop(void) {

  // read and print RSSI
  rssi = analogRead(A7);
  rssi = ((((rssi * (5.07 / 1024)) - 0.4) * 37.5) - 125); // Convert ADV value to dBm
  counter = counter + 1; // reduces amount of printing
  if (counter > 10) {
    Serial.print("RSSI=");
    Serial.print(rssi);
    Serial.print("\n"); //newline
    counter = 0; // reset counter
  }

  // clear the string for new input:
  inString = "";

  // Convert integer to character for display ( https://www.instructables.com/id/Converting-integer-to-character/)

  long a = rssi;
  char b[5]; // define character variable
  String str; // define string variable
  str = String(a); // convert to string
  str.toCharArray(b, 5);
  Serial.print(b[5]);
  // Print to oled
  u8g2.begin();
  u8g2.firstPage();
  do {
    // u8g2.setFont(u8g2_font_ncenB14_tr);
    // u8g2.drawStr(0, 15, b);
    //u8g2.setFont(u8g2_font_logisoso62_tn);
    //u8g2.drawStr(15, 65, b);
    //u8g2.setFont(u8g2_font_logisoso28_tf);
    //u8g2.drawStr(15, 55, b);
    u8g2.setFont(u8g2_font_logisoso42_tf);
    u8g2.drawStr(15, 40, b);
    Serial.print(b);
  } while ( u8g2.nextPage() );
  delay(1000);

  // This is the function which receives the serial data:

  recvWithEndMarker();

  delay(200); // We wait 200 mS to print the next value

}
