#include <SPI.h>
#include <Wire.h>  // For i2c coms
#include <SoftwareSerial.h>  // For i2c comms etc
#include <ServoTimer2.h> // Works fine
#include <avr/wdt.h>
#include <Wire.h>  // For i2c coms

SoftwareSerial mySerial(11, 12); // Pin D11 RX, Pin D12 TX Use this one, not the TX output!

// define the pins for the servos
#define servo1 5
#define servo2 6
#define servo3 7
#define servo4 8
#define servo5 9
#define servo6 10

// Declare variables for the servos
ServoTimer2 ch1;
ServoTimer2 ch2;
ServoTimer2 ch3;
ServoTimer2 ch4;
ServoTimer2 ch5;
ServoTimer2 ch6;

// FAILSAFE values
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;
int val5 = 0;
int val6 = 0;

// Blinky on receipt
#define LED 13

unsigned long previousMillis = 0;        // for time delay used in Failsafe
const long interval = 10000;           // set time interval (milliseconds)
unsigned long currentMillis = millis();

void setup()
{
  pinMode(5, INPUT); // Data input
  pinMode(6, OUTPUT);// Not used
  pinMode(8, INPUT); // Not used

  Serial.begin(9600);  // RDF was 115200
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("RX start");

  mySerial.begin(1200);

  ch1.attach(servo1);     // attach a pin to the servos and they will start pulsing
  ch2.attach(servo2);
  ch3.attach(servo3);
  ch4.attach(servo4);
  ch5.attach(servo5);
  ch6.attach(servo6);

  // servo.writes to position
  ch1.write(val1);
  ch2.write(val2);
  ch3.write(val3);
  ch4.write(val4);
  ch5.write(val5);
  ch6.write(val6);

  pinMode(LED, OUTPUT);

}
