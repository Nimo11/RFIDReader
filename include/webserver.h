//--------------------------------------------------------------------------------------
//  Webserver
//--------------------------------------------------------------------------------------
#ifndef webserver_h
#define webserver_h

#include <Arduino.h>

extern void handleWebRequests();

// get file from spiffs
extern bool loadFromFlash(String path);

extern bool upDateRequest();

extern bool checkRequest();

void updateRun();

void sendHeader();

#endif
