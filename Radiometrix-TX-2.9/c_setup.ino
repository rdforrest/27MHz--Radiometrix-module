#include <SPI.h>
#include <Wire.h>  // For i2c coms
#include <SoftwareSerial.h>  // For i2c comms etc
#include <Manchester.h>

// TX transmit LED
#define LED 13

// TX enable pin
#define  Enable 10


#define TX_PIN  6  //pin where your transmitter is connected
#define LED_PIN 13 //pin for blinking LED
uint8_t moo = 1; //last led status
byte data[8] = {8, 20, 75, 100, 150, 200, 255}; // Test values for servo outputs

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
  delay(120);

  pinMode(LED, OUTPUT);
  pinMode(Enable, OUTPUT);

  // Switch TX LED on
  digitalWrite(LED, HIGH);

  // Switch TX on, pull down to enable
  digitalWrite(Enable, LOW);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, moo);
  // man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_300);
}

uint8_t datalength = 2; //at least two data
