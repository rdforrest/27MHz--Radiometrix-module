/*
  Arduino Nano Every - UART Transmitter
  Sends fixed payload every second with continuous preamble
  Using ChatGPT
  TX    Arduino Nano Every https://docs.arduino.cc/tutorials/nano-every/uart/
Please provide code for an Arduino Nano Every. One operating as a Transmitter and one as a receiver.
Output test data from an 8 by 1 array with random values initially chosen from 0 to 255 to serial monitor at 9600 Baud.  
Please print test data using the Serial USB port.
TX send a fresh random 8-byte packet every second.
USB debug output (Serial.print) to display all values in hexadecimal
Please provide code for an Arduino Nano Every acting as a transmitter using Radiometrix LMT0 modules. 
Output test data to serial monitor at 9600 Baud. 
Use a hardware UART (rs232 type asynchronous interface) and fulfil all the requirements of the link. 
The data is sent in a burst, or packet, comprising of: [preamble] [uart ‘start’ byte] [framing bytes] [data ‘payload’] [checksum] Characters must be sent continuously, start bit to stop bit without gaps (or the mark:space ratio will become unpredictable) 
Assuming 1+8+1 (one start, 8 data, 1 stop) format, a sequence of 55h (ascii U) characters provides a square wave preamble. 
After the transmitter is turned on, a stream of these preamble bytes must be sent for 20mS, An FFh byte must immediately follow the preamble, so the UART can frame on an identifiable start bit.
To maintain DC balance, a 00h byte must follow the FFh. Both these bytes can form part of the decoded ‘framing’ sequence of the burst. 
Then, to maintain mark:space balance, data can be sent by using only those characters with an equal number of ones and zeros in them. 
Of the 256 possible 8 bit codes, 70 contain 4 ones & 4 zeros. Omitting 0Fh, F0h, 3Ch and C3h (worst case ‘four ones in a row’ bit sequences) still leaves 66 usable codes per byte, which allows six bits of actual data to be coded into each transmitted byte.
(A full explanation of this method can be found here: http://www.radiometrix.co.uk/products/bimsheet.htm#rs232) 
It is necessary to use a number of fixed value bytes preceding the actual data ‘payload’ as packet identification (or ‘framing’ ), to allow the decoder to tell a valid data burst from random channel noise. 
To avoid false triggering of the decoder, at lease 3-4 bytes of framing data will be necessary. 
Additionally, one or more ‘address’ bytes may be added (to allow co-located operation of multiple systems, or polled access of multiple receivers by a single transmitter), and ideally, a checksum of some kind should be added to the data packet, as an extra precaution against spurious triggering.

Chat- GPT summary:
[preamble] [start byte] [framing bytes] [encoded payload] [encoded checksum]
Preamble ? 0x55 repeated for ~20 ms (ensures receiver locks to bitstream).
Start byte ? 0xFF (unique framing marker).
Framing byte ? 0x00 (maintains DC balance, part of sync sequence).
Encoded payload ? 8 raw data bytes ? encoded into ~11 balanced bytes.
Checksum ? XOR of payload bytes, encoded as 1 balanced byte.
2. Balanced Code Rule
Only 8-bit codes with exactly 4 ones and 4 zeros are valid.
Total valid codes: 70.
Exclude: 0x0F, 0xF0, 0x3C, 0xC3 (bad run-length patterns).
Leaves 66 usable codes ? enough to encode 6 bits each.
3. Encoding / Decoding
TX: split data into 6-bit chunks ? lookup ? send encoded 8-bit code.
RX: read 8-bit code ? lookup ? recover 6-bit chunk ? reassemble original payload.
Function	Purpose
encode6to8(x)	Maps 6-bit value ? 8-bit balanced code
decode8to6(b)	Maps balanced code ? 6-bit value (or -1 if invalid)
4. Checksum
Simple XOR of all original payload bytes.
Encoded before transmission to maintain balance.
RX recalculates ? must match decoded checksum.
5. Transmission Timing
UART format: 1 start, 8 data, 1 stop (8N1).
9600 baud default.
Data sent continuously, no gaps, to preserve mark:space ratio.
? Summary:
20 ms preamble of 0x55 ? 0xFF start ? 0x00 frame ? encoded payload ? encoded checksum.
Balanced codes ensure DC balance + “encryption”.
Receiver locks on preamble, detects framing, then decodes payload safely.

*/

// Trying libraries needed later
#include <SPI.h>
#include <Wire.h>            // For i2c coms
#include <SoftwareSerial.h>  // For i2c comms etc
// #include <Manchester.h>  // Doesn't compile

/*
  Balanced-Coded UART Transmitter
  Arduino Nano Every
  Sends packets with preamble, start, framing, payload, checksum
*/

const uint8_t PREAMBLE_BYTE = 0x55;  // UART square wave
const uint16_t PREAMBLE_MS = 20;
const uint8_t START_BYTE   = 0xFF;
const uint8_t FRAME_BYTE   = 0x00;

uint8_t payload[8];
uint32_t packetCount = 0;

// --- Balanced Code Table (66 entries) ---
const uint8_t balancedTable[66] = {
  0x33,0xCC,0x55,0xAA,0x66,0x99,0x78,0x87,
  0x5A,0xA5,0x69,0x96,0x6C,0x93,0x39,0xC6,
  0x72,0x8D,0x4E,0xB1,0x1E,0xE1,0x2D,0xD2,
  0x27,0xD8,0x18,0xE7,0x36,0xC9,0x63,0x9C,
  0x45,0xBA,0x54,0xAB,0x15,0xEA,0x51,0xAE,
  0x4A,0xB5,0x25,0xDA,0x5C,0xA3,0x3A,0xC5,
  0x2E,0xD1,0x1D,0xE2,0x6A,0x95,0x59,0xA6,
  0x46,0xB9,0x64,0x9B,0x29,0xD6,0x16,0xE9,
  0x92,0x6D
};
const int BALANCED_COUNT = 66;

// --- Lookup Functions ---
uint8_t encode6to8(uint8_t sixBits) {
  return balancedTable[sixBits % BALANCED_COUNT];
}

void setup() {
  Serial.begin(9600);     // USB debug
  Serial1.begin(9600);    // UART link
  randomSeed(analogRead(0));

  // Generate fixed random payload once
  for (int i=0; i<8; i++) {
    payload[i] = random(0, 256);
  }

  Serial.println("TX Ready");
}

void loop() {
  sendPacket();
  delay(100); // Send 10 packets every second
}

void sendPacket() {
  packetCount++;

  // --- Send preamble (20ms of 0x55) ---
  unsigned long start = millis();
  while (millis() - start < PREAMBLE_MS) {
    Serial1.write(PREAMBLE_BYTE);
  }

  // --- Start + framing bytes ---
  Serial1.write(START_BYTE);
  Serial1.write(FRAME_BYTE);

  // --- Encode payload into balanced codes ---
  uint8_t bitBuffer[64];
  int bitCount = 0;
  for (int i=0; i<8; i++) {
    for (int b=0; b<8; b++) {
      bitBuffer[bitCount++] = (payload[i] >> b) & 1;
    }
  }

  uint8_t checksum = 0;
  for (int i=0; i<bitCount; i += 6) {
    uint8_t sixBits = 0;
    for (int b=0; b<6 && (i+b)<bitCount; b++) {
      sixBits |= (bitBuffer[i+b] << b);
    }
    uint8_t encoded = encode6to8(sixBits);
    Serial1.write(encoded);
    checksum ^= sixBits;
  }

  // --- Encode checksum (6 bits only) ---
  uint8_t chkEncoded = encode6to8(checksum & 0x3F);
  Serial1.write(chkEncoded);

  // --- Debug output ---
  Serial.print("TX Packet #");
  Serial.print(packetCount);
  Serial.print(" Data=[");
  for (int i=0; i<8; i++) {
    Serial.print("0x");
    if (payload[i] < 0x10) Serial.print("0");
    Serial.print(payload[i], HEX);
    if (i<7) Serial.print(",");
  }
  Serial.println("]");
}
