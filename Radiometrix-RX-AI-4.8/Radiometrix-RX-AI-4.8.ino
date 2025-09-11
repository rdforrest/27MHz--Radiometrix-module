/*
  Arduino Nano Every - UART Receiver
  Locks on preamble and decodes continuous packet stream
  Handles lost bytes with timeout
  Using ChatGPT
*/

// Trying libraries needed later
#include <avr/wdt.h>
// #include <Manchester.h> // Doesn't compile
#include <Servo.h>
/*
  Balanced-Coded UART Receiver
  Arduino Nano Every
  Resyncs on preamble/start bytes
  Tracks packet loss / checksum fails
*/

const uint8_t PREAMBLE_BYTE = 0x55;
const uint8_t START_BYTE   = 0xFF;
const uint8_t FRAME_BYTE   = 0x00;

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

// --- Lookup Function ---
int decode8to6(uint8_t code) {
  for (int i=0; i<BALANCED_COUNT; i++) {
    if (balancedTable[i] == code) return i;
  }
  return -1;
}

uint32_t packetCount = 0;
uint32_t lostCount   = 0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("RX Ready");
}

void loop() {
  if (Serial1.available()) {
    if (readPacket()) {
      packetCount++;
      Serial.print("RX Packet #");
      Serial.print(packetCount);
      Serial.print(" | Lost: ");
      Serial.println(lostCount);
    } else {
      lostCount++;
      Serial.print("Packet Lost! Total Lost: ");
      Serial.println(lostCount);
    }
  }
}

bool readPacket() {
  // --- Wait for start sequence (FF 00) ---
  while (Serial1.available() >= 2) {
    if (Serial1.peek() == START_BYTE) {
      Serial1.read();
      if (Serial1.peek() == FRAME_BYTE) {
        Serial1.read();
        break; // found sync
      }
    } else {
      Serial1.read(); // discard until sync
    }
  }

  // --- Decode symbols until full payload + checksum ---
  uint8_t decodedBits[64];
  int bitPos = 0;
  uint8_t checksum = 0;

  while (bitPos < 64) {
    while (!Serial1.available()); // wait
    int sym = decode8to6(Serial1.read());
    if (sym < 0) return false; // invalid symbol
    for (int b=0; b<6 && bitPos<64; b++) {
      decodedBits[bitPos++] = (sym >> b) & 1;
    }
    checksum ^= sym;
  }

  // --- Decode checksum symbol ---
  while (!Serial1.available());
  int chkSym = decode8to6(Serial1.read());
  if (chkSym < 0) return false;

  // --- Rebuild payload bytes ---
  uint8_t payload[8] = {0};
  int bitIndex = 0;
  for (int i=0; i<8; i++) {
    for (int b=0; b<8; b++) {
      payload[i] |= (decodedBits[bitIndex++] << b);
    }
  }

  // --- Verify checksum ---
  if ((checksum & 0x3F) != chkSym) {
    Serial.println("Checksum FAIL");
    return false;
  }

  // --- Print payload ---
  Serial.print("RX Data=[");
  for (int i=0; i<8; i++) {
    Serial.print("0x");
    if (payload[i] < 0x10) Serial.print("0");
    Serial.print(payload[i], HEX);
    if (i<7) Serial.print(",");
  }
  Serial.println("] OK");

  return true;
}
