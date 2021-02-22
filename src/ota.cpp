//--------------------------------------------------------------------------------------
//  OTA
//--------------------------------------------------------------------------------------
#include <ota.h>
#include <Global.h>
#include "ArduinoOTA.h"

void setOTA(const char* hostname,const char* password) {
  ArduinoOTA.setHostname(hostname);//undercscore not allowed
  ArduinoOTA.setPassword(password);

  ArduinoOTA.onStart([]() {
    _Log.println(LogObject::DebugLevels::Normal,"Start OTA update");
  });
  ArduinoOTA.onEnd([]() {
    _Log.println(LogObject::DebugLevels::Verbose,"\nEnd of OTA");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    _Log.printf(LogObject::DebugLevels::Verbose,"Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    _Log.printf(LogObject::DebugLevels::ErrorOnly,"Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) _Log.println(LogObject::DebugLevels::ErrorOnly,"Auth Failed");
    else if (error == OTA_BEGIN_ERROR) _Log.println(LogObject::DebugLevels::ErrorOnly,"Begin Failed");
    else if (error == OTA_CONNECT_ERROR) _Log.println(LogObject::DebugLevels::ErrorOnly,"Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) _Log.println(LogObject::DebugLevels::ErrorOnly,"Receive Failed");
    else if (error == OTA_END_ERROR) _Log.println(LogObject::DebugLevels::ErrorOnly,"End Failed");
  });
  
  ArduinoOTA.begin();
  _Log.println(LogObject::DebugLevels::Normal,"OTA ready");

}
