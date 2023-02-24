#include <avr/wdt.h>
#include <Manchester.h>
#include <Servo.h>

// create servo object to control a servo. twelve servo objects can be created on most boards
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

#define RX_PIN 11
#define LED_PIN 13

uint8_t moo = 1;
#define BUFFER_SIZE 22
uint8_t buffer[BUFFER_SIZE];

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
  servo1.attach(5,1000,2000); // Set Servo Min and Max
  servo2.attach(6,1000,2000);
  servo3.attach(7,1000,2000);
  servo4.attach(8,1000,2000);
  servo5.attach(9,1000,2000);
  servo6.attach(10,1000,2000);

  pinMode(5, INPUT); // Data input
  pinMode(6, OUTPUT);// Not used
  pinMode(8, INPUT); // Not used

  Serial.begin(9600);  // RDF was 115200
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("RX start");
  delay(120);

  pinMode(LED, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, moo);
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_300);
  man.beginReceiveArray(BUFFER_SIZE, buffer);

}
