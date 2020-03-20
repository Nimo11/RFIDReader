#ifndef Global_h
#define Global_h

#include "LogObject.h"
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include "config.h"
#include <MFRC522.h>

enum NtpStates
{
  waiting,
  Ok,
  Error
};

// current version for auto update
extern const int _FW_VERSION;
// auto update store
extern const char* _fwUrlBase;

extern LogObject _Log;
extern HTTPClient _httpClient;
extern WiFiClient _client;
extern WiFiManager _wm;
extern WiFiUDP _Udp;
extern String _Cookie;

extern Session _session;

extern Config _config;

extern WiFiManagerParameter _url_server_param;
extern WiFiManagerParameter _IP_param;
extern WiFiManagerParameter _response_Ok_param;
extern WiFiManagerParameter _User_param;
extern WiFiManagerParameter _Password_param;
extern WiFiManagerParameter _UidNode_param;

extern const int _pinBuz;
extern MFRC522 _rfid;

//extern MFRC522::MIFARE_Key key; 
extern byte _nuidPICC[4];

// store last card uid readed
extern unsigned long _lastRead;

#endif