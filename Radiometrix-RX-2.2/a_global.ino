unsigned int ppmValues[8];
int pulse = 0;
int rssi;
int incomingData = 0; // for incoming serial data
int i = 0; // loop counter
byte byteArr[8];
int counter = 0;

const byte numBytes = 32;
byte receivedBytes[numBytes];
byte numReceived = 0;

boolean newData = false;

// Test values for servo outputs
int buf[6] = {20, 75, 100, 150, 200, 255}; // Test values for servo outputs
