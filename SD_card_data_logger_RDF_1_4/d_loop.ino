
void loop(void) {

  // read temperature
  pickUpTemperature();
  //read the time
  now = RTC.now();

  //open file to log data in.
  dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  // log the temperature and time.
  if (dataFile) {
    // Test Data store RDF
    int datastore = 16;
    int datastore1 = 17;
    // read ADC data
    datastore = analogRead(A3);
    datastore1 = analogRead(A2);
    dataFile.print(datastore);
    dataFile.print(",");  // To get a CSV
    dataFile.print(datastore1);
    dataFile.print(",");  // To get a CSV
    // dataFile.print(celsius);
    dataFile.print("                 ");

    dataFile.print(now.hour(), DEC);
    dataFile.print(":");
    dataFile.print(now.minute(), DEC);
    dataFile.print(":");
    dataFile.println(now.second(), DEC);

    dataFile.close();
    // print to the serial port too:
    // Serial.println("data stored");
    Serial.print(datastore);
    Serial.print("\t");  //tab
    Serial.print(datastore1);
    Serial.print("\t");  //tab
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.println(now.second(), DEC);
    Serial.print("\n");  //newline

  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  //delay(60000); // this will log the temperature every minute.
  delay(5000);  // Every 5 seconds

  /*
  // Convert integer to character for oLED display ( https://www.instructables.com/id/Converting-integer-to-character/)
  int a = r.getPosition();
  float value1 = dBBin[a];
  char b[5];        // define character variable
  String str;       // define string variable
  str = String(a);  // convert to string
  str.toCharArray(b, 5);
*/

  int value1 = 1.42;  // ADC1 value

  // Convert float to character for oLED display https://www.programmingelectronics.com/dtostrf/#:~:text=dtostrf()%20%E2%80%93%20turn%20your%20floats,dtostrf%20in%20your%20Arduino%20code.
  char b[7];                 // define character variable
  dtostrf(value1, 6, 2, b);  //dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
  String str;                // define string variable
  str = String(b);           // convert to string
  str.toCharArray(b, 7);

  // Print to oled
  oled.clear();
  oled.setFont(Cooper26);
  // Display double height and width characters.
  // oled.set2X();
  oled.print(b);
  oled.print(" data");

}


// function will check the temperature sensor and update the temperature.
void pickUpTemperature() {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];


  if (!ds.search(addr)) {
    //Serial.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }

  //Serial.print("ROM =");
  for (i = 0; i < 8; i++) {
    // Serial.write(' ');
    // Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return;
  }
  Serial.println();

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      // Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      //  Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      //  Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      //   Serial.println("Device is not a DS18x20 family device.");
      return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);  // start conversion, with parasite power on at the end

  delay(1000);  // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);  // Read Scratchpad

  //Serial.print("  Data = ");
  //Serial.print(present, HEX);
  //Serial.print(" ");
  for (i = 0; i < 9; i++) {  // we need 9 bytes
    data[i] = ds.read();
    // Serial.print(data[i], HEX);
    // Serial.print(" ");
  }
  // Serial.print(" CRC=");
  // Serial.print(OneWire::crc8(data, 8), HEX);
  // Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3;  // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;       // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3;  // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1;  // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
}
