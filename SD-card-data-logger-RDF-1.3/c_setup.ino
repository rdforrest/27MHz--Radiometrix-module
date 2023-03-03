
OneWire  ds(8);  // temperature sensor on pin 8 (a 4.7K resistor is necessary) //

File dataFile;
DateTime now;

void setup(void) {
  Serial.begin(9600);
  //setup clock
  Wire.begin();
  RTC.begin();
  //check if the Real Time Clock is on
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    // RDF comment - set set time using DS1307RTC - SeTime
    //  RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  //setup SD card
  Serial.print("Initializing SD card...");

  // see if the SD card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  //write down the date (year / month / day - prints only the start, so if the logger runs for several days you only find the start back at the begin.
  now = RTC.now();
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.print("Start logging on: ");
  dataFile.print(now.year(), DEC);
  dataFile.print('/');
  dataFile.print(now.month(), DEC);
  dataFile.print('/');
  dataFile.print(now.day(), DEC);
  dataFile.println(" ");
  dataFile.println("ADC1 voltage   ADC2 voltage   Time");
  dataFile.close();
}
