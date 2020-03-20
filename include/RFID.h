#ifndef RFID_h
#define RFID_h

#include <Arduino.h>

extern uint32 RFIDRead();
uint32 toInt(byte *buffer, byte bufferSize);

#endif