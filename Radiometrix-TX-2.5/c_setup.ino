#include <SPI.h>
#include <Wire.h>  // For i2c coms
#include <SoftwareSerial.h>  // For i2c comms etc


// TX transmit LED
#define LED 13

// TX enable pin
#define  Enable 10

SoftwareSerial mySerial(5, 6); // Pin D5 RX, Pin D6 TX Use this one, not the TX output!

void setup()
{
  pinMode(5, INPUT); //Telemetry input- not used
  pinMode(6, OUTPUT);// Data to radio
  pinMode(8, INPUT); //ppm input

  setup_timer1();

  Serial.begin(9600);  // RDF was 115200
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("TX start");


  // set the data rate for the SoftwareSerial port
  mySerial.begin(2400);

  pinMode(LED, OUTPUT);
  pinMode(Enable, OUTPUT);

  // Switch TX LED on
  digitalWrite(LED, HIGH);

  // Switch TX on, pull down to enable
  digitalWrite(Enable, LOW);

}
