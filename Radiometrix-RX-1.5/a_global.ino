unsigned int ppmValues[8];
int pulse = 0;
int rssi;
int incomingData = 0; // for incoming serial data
int i = 0; // loop counter
byte byteArr[8];

// Test values for servo outputs
int buf[6] = {20, 75, 100, 150, 200, 255}; // Test values for servo outputs
