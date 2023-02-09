

// Functions:

/*
  Serial connection with String to Integer conversion
  Reads a serial input string until it sees a newline, then converts the string
  to a number if the characters are digits.
  The circuit:
  - No external components needed.
  created 29 Nov 2010
  by Tom Igoe
  This example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/StringToInt
*/

void recvWithEndMarker() {

  inString = "";    // string to hold input

  // Open serial communications and wait for ports to open:
  Serial.begin(9600);
  // while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  // }

  // send an intro:
  Serial.println("Serial data():");
  Serial.println("\n"); //newline

  // Read serial1 input:
  Serial1.begin(9600);
  while (Serial1.available() > 0) {
    int inChar = Serial1.read();
    Serial.println("\n"); //newline
    Serial.println("inChar");

    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
      Serial.println("inString");
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      // Save value as a long integer.RDF
      bitt = inString.toInt();
      Serial.print("Value:");
      Serial.println(inString.toInt());
      Serial.print("String: ");
      Serial.println(inString);
      // clear the string for new input:
      inString = "";

      // Convert integer to character for display ( https://www.instructables.com/id/Converting-integer-to-character/)

      long a = bitt;

      char b[3]; // define character variable

      String str; // define string variable

      str = String(a); // convert to string

      str.toCharArray(b, 3);
      Serial.print(b[3]);
      // Print to oLED
      u8g2.firstPage();
      do {
        // 62 Pixel Height
        u8g2.setFont(u8g2_font_logisoso62_tn);
        u8g2.drawStr(20, 65, b);
        Serial.print(b);
      } while ( u8g2.nextPage() );
      delay(1000);
    }

  }
}
