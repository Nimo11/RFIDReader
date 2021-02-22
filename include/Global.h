#ifndef Global_h
#define Global_h

#include "LogObject.h"
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include "config.h"
#include <MFRC522.h>
#include "GITUpdater.h"
#include "LanguageManager.h"

enum NtpStates
{
  waiting,
  Ok,
  Error
};

extern LanguageManager _Lm;
extern LogObject _Log;
extern HTTPClient _httpClient;
extern WiFiClient _client;
extern WiFiManager* _wm;
extern GITUpdater _updater;

extern WiFiUDP _Udp;
extern String _Cookie;

extern Session _session;

extern Config _config;

class ConfigRef
{
public:
    ConfigRef(char* param,const char* node,const char* label,int size)
    {
        CfgPtr = param;
        Size =size;
        WmParam=WiFiManagerParameter(node, label, param, Size);
    }
    ConfigRef(const char* label)
    {
        WmParam=WiFiManagerParameter(label);
    }
    char * CfgPtr;
    int Size;
    WiFiManagerParameter WmParam;
};

extern ConfigRef _paramConfig[11];

extern const int _pinBuz;
extern const int _greenLed;
extern const int _redLed;

extern MFRC522 _rfid;

//extern MFRC522::MIFARE_Key key; 
extern byte _nuidPICC[4];

// store last card uid readed
extern unsigned long _lastRead;



#endif