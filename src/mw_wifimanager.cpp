//-------------------------------------------------------------------------------------------
// Wifimanager
//--------------------------------------------------------------------------------------------
#include "mw_wifimanager.h"
#include <WiFiManager.h>
#include "config.h"
#include "Global.h"

WiFiManagerParameter _DIV_END("</DIV>");

void setWifiManagerMenu()
{
  _wm = new WiFiManager();

  IPAddress addr;
  if (addr.fromString(_config.IP))
  {
    Serial.println(addr);
    _wm->setSTAStaticIPConfig(addr, addr, IPAddress(255, 255, 255, 0));
  }
  else
  {
    Serial.print("Wrong IP ");
    Serial.println(_config.IP);
  }

  _wm->AuthenticationRequired = true;
  _wm->AuthUser = "admin";
  _wm->AuthPassword = _config.admin_pass;
  
  std::vector<const char *> menu = {"param", "sep", "wifi", "info", "sep", "restart"};
  _wm->setMenu(menu);
  _wm->setDebugOutput(_Log.level == LogObject::DebugLevels::Verbose);
  _wm->setSaveParamsCallback(saveParam);
  _wm->setConfigPortalBlocking(true);

  for (ConfigRef &p : _paramConfig)
  {
    _wm->addParameter(&p.WmParam);
  }

  _wm->addParameter(&_DIV_END);
}
