// RX_Radio_27MHz_Balanced_Failsafe_AutoReset.ino
// Balanced 4-of-8 encoded 27MHz Radiometrix receiver with auto-resetting failsafe.

#include <Arduino.h>
#include <Servo.h>

const uint32_t BAUD = 2400;
const uint8_t LED_PIN = LED_BUILTIN;
const uint8_t SERVO_PINS[8] = { 5, 6, 7, 8, 9, 10, 11, 12 };
Servo servos[8];
uint16_t servoUs[8];

// framing bytes
const uint8_t PREAMBLE_TRAIL1 = 0xFF;
const uint8_t PREAMBLE_TRAIL2 = 0x00;
const uint8_t FR_AA = 0xAA;
const uint8_t FR_99 = 0x99;
const uint8_t FR_CC = 0xCC;
const uint8_t FR_REPEAT = 4;

// balanced code tables
uint8_t balancedTable[64];
uint8_t decodeTable[256];

void initBalancedSymbols() {
  static bool excluded[256];
  for (int i = 0; i < 256; i++) excluded[i] = false;
  excluded[0x0F] = true;
  excluded[0xF0] = true;
  excluded[0x3C] = true;
  excluded[0xC3] = true;

  int idx = 0;
  for (int b = 0; b < 256 && idx < 64; ++b) {
    if (__builtin_popcount((unsigned int)b) == 4 && !excluded[b])
      balancedTable[idx++] = (uint8_t)b;
  }
  if (idx < 64) {
    for (int b = 0; b < 256 && idx < 64; ++b)
      if (__builtin_popcount((unsigned int)b) == 4)
        balancedTable[idx++] = (uint8_t)b;
  }

  for (int i = 0; i < 256; i++) decodeTable[i] = 0xFF;
  for (int i = 0; i < 64; i++) decodeTable[balancedTable[i]] = (uint8_t)i;
}

int decodeBalanced(const uint8_t *in, int inLen, uint8_t *out) {
  uint32_t buf = 0;
  int bits = 0, o = 0;
  for (int i = 0; i < inLen; i++) {
    uint8_t sym = in[i];
    uint8_t six = decodeTable[sym];
    if (six == 0xFF) return -1;
    buf = (buf << 6) | six;
    bits += 6;
    while (bits >= 8) {
      out[o++] = (buf >> (bits - 8)) & 0xFF;
      bits -= 8;
    }
  }
  return o;
}

// state machine
enum RxState { SEARCH_PREAMBLE,
               GOT_0xFF,
               GOT_0xFF_00,
               GOT_AA_SEQ,
               GOT_99_SEQ,
               GOT_CC_SEQ };
RxState state = SEARCH_PREAMBLE;
int aaCount = 0, n99 = 0, nCC = 0;

const int EXPECT_ENCODED_PAYLOAD_LEN = 12;  // 9 raw -> 12 encoded
uint8_t encPayload[32];
int encIndex = 0;

unsigned long lastPacketTime = 0;
unsigned long lastRssiPrint = 0;
unsigned long lastBlink = 0;
const unsigned long FAILSAFE_TIMEOUT = 5000;  // 5s
bool inFailsafe = false;

// --- helper: set all servos to failsafe (1000 µs) ---
void activateFailsafe() {
  for (int i = 0; i < 8; i++) {
    servos[i].writeMicroseconds(1000);
    servoUs[i] = 1000;
  }
  Serial.println("** FAILSAFE ACTIVATED **");
  inFailsafe = true;
}

// --- helper: leave failsafe, blink LED once ---
void exitFailsafe() {
  if (inFailsafe) {
    Serial.println("** FAILSAFE CLEARED **");
    inFailsafe = false;
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
  }
}

// --- called when valid packet decoded ---
void handleValidPacket(uint8_t *raw, int len) {
  uint8_t checksum = 0;
  for (int i = 0; i < 8; i++) checksum += raw[i];
  if (checksum != raw[8]) {
    Serial.println("RX: bad checksum");
    return;
  }

  // reset timer and clear failsafe if active
  lastPacketTime = millis();
  exitFailsafe();

  // apply channel data
  for (int i = 0; i < 8; i++) {
    uint8_t v = raw[i];
    uint16_t us = map(v, 0, 255, 1000, 2000);
    servos[i].writeMicroseconds(us);
    servoUs[i] = us;
  }

  // blink once
  digitalWrite(LED_PIN, HIGH);
  delay(30);
  digitalWrite(LED_PIN, LOW);

  Serial.print("RX servos (us): ");
  for (int i = 0; i < 8; i++) {
    Serial.print(servoUs[i]);
    if (i < 7) Serial.print(", ");
  }
  Serial.println();
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(BAUD);
  Serial1.begin(BAUD);
  initBalancedSymbols();

  Serial.println("RX Version 5.4");

  for (int i = 0; i < 8; i++) {
    servos[i].attach(SERVO_PINS[i]);
    servos[i].writeMicroseconds(1500);
    servoUs[i] = 1500;
  }

  state = SEARCH_PREAMBLE;
  lastPacketTime = millis();
  lastRssiPrint = millis();
  lastBlink = 0;
}

void loop() {
  while (Serial1.available()) {
    uint8_t b = Serial1.read();

    switch (state) {
      case SEARCH_PREAMBLE:
        if (b == PREAMBLE_TRAIL1) state = GOT_0xFF;
        break;
      case GOT_0xFF:
        if (b == PREAMBLE_TRAIL2) {
          state = GOT_0xFF_00;
          aaCount = n99 = nCC = 0;
        } else state = SEARCH_PREAMBLE;
        break;
      case GOT_0xFF_00:
        if (b == FR_AA) {
          if (++aaCount >= FR_REPEAT) state = GOT_AA_SEQ;
        } else state = SEARCH_PREAMBLE;
        break;
      case GOT_AA_SEQ:
        if (b == FR_99) {
          if (++n99 >= FR_REPEAT) state = GOT_99_SEQ;
        } else state = SEARCH_PREAMBLE;
        break;
      case GOT_99_SEQ:
        if (b == FR_CC) {
          if (++nCC >= FR_REPEAT) {
            state = GOT_CC_SEQ;
            encIndex = 0;
          }
        } else state = SEARCH_PREAMBLE;
        break;
      case GOT_CC_SEQ:
        encPayload[encIndex++] = b;
        if (encIndex >= EXPECT_ENCODED_PAYLOAD_LEN) {
          uint8_t raw[16];
          int rawLen = decodeBalanced(encPayload, encIndex, raw);
          if (rawLen == 9) handleValidPacket(raw, rawLen);
          else Serial.println("RX: decode fail");

          state = SEARCH_PREAMBLE;
          encIndex = 0;
          aaCount = n99 = nCC = 0;
        }
        break;
    }
  }

  // --- failsafe check ---
  if (millis() - lastPacketTime > FAILSAFE_TIMEOUT) {
    if (!inFailsafe) activateFailsafe();
    // blink while failsafe
    if (millis() - lastBlink > 200) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      lastBlink = millis();
    }
  }

  // --- RSSI print once per second ---
  if (millis() - lastRssiPrint >= 1000) {
    int r = analogRead(A0);
    Serial.print("RSSI: ");
    Serial.println(r);
    lastRssiPrint = millis();
  }
}
