//-------------------------------------------------------------------------------------------
// Wifimanager
//--------------------------------------------------------------------------------------------
#include "mw_wifimanager.h"
#include <WiFiManager.h>
#include "config.h"
#include "Global.h"

void setWifiManagerMenu()
{
  std::vector<const char *> menu = {"wifi", "info", "param","test", "sep", "restart"};
  _wm.setMenu(menu);
  _wm.setDebugOutput(_Log.level==LogObject::DebugLevels::Verbose);
  _wm.setSaveParamsCallback(saveParam);
  _wm.setConfigPortalBlocking(true);
  //_wm.setCountry("FR");

  _wm.addParameter(&_url_server_param);
  _wm.addParameter(&_IP_param);
  _wm.addParameter(&_response_Ok_param);
  _wm.addParameter(&_User_param);
  _wm.addParameter(&_Password_param);
  _wm.addParameter(&_UidNode_param);
}
