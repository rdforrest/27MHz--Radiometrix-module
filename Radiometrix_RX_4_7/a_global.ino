unsigned int ppmValues[8];
int pulse = 0;
int rssi;
int incomingData = 0;  // for incoming serial data
int i = 0;             // loop counter
int counter = 0;

const byte numBytes = 8;
byte receivedBytes[numBytes];
byte numReceived = 0;

boolean newData = false;

#define DEBUG 1;  // Comment this out to blank out debug code


