// TX_Radio_27MHz_Balanced.ino
// Arduino Nano Every transmitter sketch with 4-of-8 balanced encoding.
// PPM input on D2, hardware UART Serial1 -> Radiometrix TX
// USB Serial at 2400 for debug

/*

***********************************************************************************************
  Revisions:
  Date        rev,    who      what
  Jan 23      1.3     DF        Reduce baud rate to 300
  Jan 23      1.7     DF        Single servo channel and data filter for testing
  Feb 13      2.7     DF        Use test data
  Feb 15      2.8     DF        Manchester encoding used
  Feb 21      3.0     DF        Now working on the bench with 6 servo channels and failsafes.
  Mar 3       3.1     DF        Put in timer and debug at Jonathan's suggestion.
  Mar 7       3.2     DF        Cleaned up code and put Failsafe in based on TX ID code
  Jun 3       4.0     DF        Revisited stick mapping. (Had problems with no stick output.) Not very satisfactory still - glitches!
  Oct 10      AI-4.2  DF        Successfully used Chat-GPT to replace Manchester Encoding with Radiometrix recommended methods via UART
  Oct 25      AI-5.4  DF        Working version with failsafe and servos. Range just about OK. (15m collapsed telescopic. 50m extended telescopic.Both TX and RX use Schuckman antennae matching units.)
  Oct 26      AI-5.4  DF        Baud rate dropped to 2400 - this doubled the range. (50m collapsed, 100m+ extended)
  Nov  1      AI-5.4  DF        Too much latency. Trying 4800 Baud
  Nov 18      AI-5.4  DF        Tested in Pioneer model sub at Windermere MBC pond. Worked well at about 20m range. Latency OK.

  Chat GPT Brief:      Radio-control at 27 MHz using Radiometrix modules

Please provide code for an Arduino Nano Every. One operating as a Transmitter(TX) and one as a receiver(RX).

The input data is from a transmitter PPM stream for 8 channels on pin D2.
The RX should output standard PWM servo outputs for 8 servo channels on pins D5 to D12
Nano Every Serial and Serial1 USB data at 2400 Baud.  

Use a hardware UART (rs232 type asynchronous interface) and fulfil all the requirements of the link. 
The data is sent in a burst, or packet, comprising of: [preamble] [uart ‘start’ byte] [framing bytes] [data ‘payload’] [checksum] Characters must be sent continuously, start bit to stop bit without gaps (or the mark:space ratio will become unpredictable) 
Assuming 1+8+1 (one start, 8 data, 1 stop) format, a sequence of 55 hex (ascii U) characters provides a square wave preamble. 
After the transmitter is turned on, a stream of these preamble bytes must be sent for 20mS, An FF hex byte must immediately follow the preamble, so the UART can frame on an identifiable start bit.
To maintain DC balance, a 00h byte must follow the FF hex. 

Framing data for transmitter and  receiver waits for are AA hex 99 hex and  CC hex.
To avoid false triggering of the decoder, at lease 3-4 bytes of framing data will be necessary. 

A checksum should be added to the data packet, as an extra precaution against spurious triggering.


Data sent continuously, no gaps, to preserve mark:space ratio.


Flashes onboard LED on TX and RX when a valid packet is encoded or decoded.
Print TX input data and RX output data. Print RSSI (from pin A0 )once every second for RX.

To maintain mark:space balance, please use data encoding for RX and TX, by using only those characters with an equal number of ones and zeros in them. 
Of the 256 possible 8 bit codes, 70 contain 4 ones & 4 zeros. Omitting 0Fh, F0h, 3Ch and C3h (worst case ‘four ones in a row’ bit sequences) leaves 66 usable codes per byte, which allows six bits of actual data to be coded into each transmitted byte.

  Hardware connections#
  - For TX , connect TX power at 9 volts to VIN on Arduino
  - For TX , use pin D2 as ppm input from TX
  - For TX , use pin TX for the serial output to the Radiometrix module through a 120 k ohm resistor to protect input
  - For TX , 5v sounder on pin D7 - if used
  - For TX , Transmitter output voltage to VIN on Nano
  - For TX , GND from Transmitter to GND on Nano
  - For RX , Servo outputs pins D5 to D12
  - For RX , RX connected to module data input (open collector) with 4.7K ohm pullup to +5v if used
  - For RX , A0 connected to module RSSI output
  - Both TX and RX use Schuckman antennae matching units.)
  
REMOVED:
Please provide RX and TX code as a zipped file.
The input data is from a transmitter PPM stream for 8 channels but provide a Debug mode where the hex code 5E is on all the channels.
For debug mode, please print Raw data and Encoded data from the TX in hex. Also in Debug mode Encoded and Decoded data from the RX in hex. Both use the Serial USB port.

Radiometrix modules are easily available and CE approved (They are not very cheap though!).
I am using antenna matching circuits developed by Rob Schuckman in the Netherlands.  
You can see details of his extensive work in 27MHz r/c work over here: 
https://robschuckman.wixsite.com/robschuckman/27mhz-remote-control

*/

#include <Arduino.h>

const uint8_t PPM_PIN = 2;
const uint8_t SOUND_PIN = 7;
const uint8_t LED_PIN = LED_BUILTIN;

volatile unsigned long lastEdge = 0;
volatile unsigned long pulseLen = 1500;
volatile uint8_t chanIndex = 0;
volatile bool frameReady = false;
volatile uint16_t chanRawMicros[8];  // measured pulse widths

uint8_t channels8[8];  // scaled 0..255

const uint32_t BAUD = 4800;

// framing constants
const uint8_t PREAMBLE_BYTE = 0x55;
const int PREAMBLE_COUNT = 55;  // preamble bytes (kept as before)
const uint8_t PREAMBLE_TRAIL1 = 0xFF;
const uint8_t PREAMBLE_TRAIL2 = 0x00;
const uint8_t FR_AA = 0xAA;
const uint8_t FR_99 = 0x99;
const uint8_t FR_CC = 0xCC;
const uint8_t FR_REPEAT = 4;  // repeat framing bytes

// Balanced symbol tables (generated at runtime)
uint8_t balancedTable[64];
uint8_t decodeTable[256];

void initBalancedSymbols() {
  // Exclude these four worst-case sequences
  const bool excludeMap[256] = { false };
  // Build an exclude set manually:
  // We'll create a small boolean array for excluded values
  static bool excluded[256];
  for (int i = 0; i < 256; i++) excluded[i] = false;
  excluded[0x0F] = true;
  excluded[0xF0] = true;
  excluded[0x3C] = true;
  excluded[0xC3] = true;

  // collect bytes with popcount==4 and not excluded
  int idx = 0;
  for (int b = 0; b < 256 && idx < 64; ++b) {
    byte x = (byte)b;
    // count ones
    uint8_t pc = __builtin_popcount((unsigned int)x);
    if (pc == 4 && !excluded[b]) {
      balancedTable[idx++] = x;
    }
  }
  // if we didn't find 64 (very unlikely), fall back to gather popcount==4 including excluded
  if (idx < 64) {
    for (int b = 0; b < 256 && idx < 64; ++b) {
      byte x = (byte)b;
      if (__builtin_popcount((unsigned int)x) == 4) {
        // allow even excluded if needed
        balancedTable[idx++] = x;
      }
    }
  }

  // Init decode table to invalid
  for (int i = 0; i < 256; i++) decodeTable[i] = 0xFF;
  for (int i = 0; i < 64; i++) {
    decodeTable[balancedTable[i]] = (uint8_t)i;
  }
}

// Bitstream packer: take raw bytes, emit 6-bit groups mapped to balancedTable
// Returns number of encoded bytes produced
int encodeBalanced(const uint8_t *in, int len, uint8_t *out) {
  uint32_t buffer = 0;
  int bitsIn = 0;
  int outIdx = 0;
  for (int i = 0; i < len; i++) {
    buffer = (buffer << 8) | in[i];
    bitsIn += 8;
    while (bitsIn >= 6) {
      uint8_t six = (buffer >> (bitsIn - 6)) & 0x3F;
      bitsIn -= 6;
      out[outIdx++] = balancedTable[six];
    }
  }
  if (bitsIn > 0) {
    // pad remaining bits with zeros (MSB-side)
    uint8_t six = (buffer << (6 - bitsIn)) & 0x3F;
    out[outIdx++] = balancedTable[six];
  }
  return outIdx;
}

// PPM ISR - rising edges
void ppmISR() {
  unsigned long now = micros();
  unsigned long len = now - lastEdge;
  lastEdge = now;

  // PPM sync gap (long)
  if (len > 3000UL) {
    chanIndex = 0;
    frameReady = true;
  } else {
    if (chanIndex < 8) {
      if (len < 500) len = 500;
      if (len > 3000) len = 3000;
      chanRawMicros[chanIndex++] = (uint16_t)len;
    }
  }
}

void sendPacket() {
  // build raw payload: 8 bytes channels (0..255), 1 checksum
  uint8_t raw[9];
  for (uint8_t i = 0; i < 8; i++) {
    raw[i] = channels8[i];
  }
  uint8_t checksum = 0;
  for (int i = 0; i < 8; i++) checksum += raw[i];
  raw[8] = checksum;

  // encode: 9 bytes -> should produce exactly 12 encoded bytes
  uint8_t encoded[16];
  int encLen = encodeBalanced(raw, 9, encoded);  // expected 12

  // send preamble & framing raw (not encoded)
  for (int i = 0; i < PREAMBLE_COUNT; i++) Serial1.write(PREAMBLE_BYTE);
  Serial1.write(PREAMBLE_TRAIL1);
  Serial1.write(PREAMBLE_TRAIL2);

  for (int r = 0; r < FR_REPEAT; r++) Serial1.write(FR_AA);
  for (int r = 0; r < FR_REPEAT; r++) Serial1.write(FR_99);
  for (int r = 0; r < FR_REPEAT; r++) Serial1.write(FR_CC);

  // send encoded payload (no gaps)
  Serial1.write(encoded, encLen);

  // visual indication
  digitalWrite(LED_PIN, HIGH);
  delay(30);
  digitalWrite(LED_PIN, LOW);

  // Debug print
  Serial.print("TX raw bytes: ");
  for (int i = 0; i < 9; i++) {
    Serial.print(raw[i]);
    if (i < 8) Serial.print(", ");
  }
  Serial.println();
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SOUND_PIN, OUTPUT);
  pinMode(PPM_PIN, INPUT_PULLUP);

  Serial.begin(BAUD);
  Serial1.begin(BAUD);

  initBalancedSymbols();

  Serial.println("TX Version 5.4");

  // init channels default 1500us -> 128
  for (int i = 0; i < 8; i++) {
    chanRawMicros[i] = 1500;
    channels8[i] = map(chanRawMicros[i], 500, 2500, 0, 255);
  }

  attachInterrupt(digitalPinToInterrupt(PPM_PIN), ppmISR, RISING);

  // initial preamble burst as requested (send same preamble / framing once)
  for (int i = 0; i < PREAMBLE_COUNT; i++) Serial1.write(PREAMBLE_BYTE);
  Serial1.write(PREAMBLE_TRAIL1);
  Serial1.write(PREAMBLE_TRAIL2);
  for (int r = 0; r < FR_REPEAT; r++) Serial1.write(FR_AA);
  for (int r = 0; r < FR_REPEAT; r++) Serial1.write(FR_99);
  for (int r = 0; r < FR_REPEAT; r++) Serial1.write(FR_CC);

  digitalWrite(LED_PIN, HIGH);
  delay(80);
  digitalWrite(LED_PIN, LOW);
}

unsigned long lastPrint = 0;

void loop() {
  if (frameReady) {
    noInterrupts();
    frameReady = false;
    interrupts();

    // scale micros -> 0..255
    for (uint8_t i = 0; i < 8; i++) {
      uint16_t m = chanRawMicros[i];
      // map typical RC range 1000..2000 to 0..255 (clamp for safety)
      if (m < 700) m = 700;
      if (m > 2300) m = 2300;
      channels8[i] = (uint8_t)map(m, 700, 2300, 0, 255);
    }
    sendPacket();
  }

  // also print periodically if no PPM
  if (millis() - lastPrint > 1000) {
    Serial.print("TX channels scaled: ");
    for (int i = 0; i < 8; i++) {
      Serial.print(channels8[i]);
      if (i < 7) Serial.print(", ");
    }
    Serial.println();
    lastPrint = millis();
  }

  delay(5);
}
