#include <SPI.h>
#include <Wire.h>  // For i2c coms
#include <SoftwareSerial.h>  // For i2c comms etc
#include <Manchester.h>

// TX transmit LED
#define LED 13

// TX enable pin
#define  Enable 10

#define TX_PIN  6  //pin where your transmitter is connected
byte data[7] = {8, 20, 75, 100, 150, 200, 127}; // Test values for servo outputs

void setup()
{
  pinMode(6, OUTPUT);// Data to radio
  pinMode(8, INPUT); //ppm input
  pinMode(LED, OUTPUT);

  setup_timer1();

  Serial.begin(9600);  // RDF was 115200

  Serial.println("TX start");
  delay(120);

  // Switch TX LED on
  digitalWrite(LED, HIGH);

  man.setupTransmit(TX_PIN, MAN_300);
}

uint8_t datalength = 7; // Trial rdf
