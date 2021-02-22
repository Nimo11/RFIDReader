#ifndef API_h
#define API_h

#include <Arduino.h>
#include "BearSSLHelpers.h"
#include "ESP8266HTTPClient.h"


extern bool sendAPIRequest(const String url,const String uid,const char* user,const char* password,const String koString,String uidNode);
extern void sendAPIRequest2(String url);

bool deserialize(String json);

String exractParam(String& authReq, const String& param, const char delimit);
String getCNonce(const int len) ;
String getDigestAuth(String& authReq, const String& username, const String& password, const String& uri, unsigned int counter) ;

#endif