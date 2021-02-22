//--------------------------------------------------------------------------------------
//  Webserver
//--------------------------------------------------------------------------------------
#include "webserver.h"
#include "Global.h"
#include "config.h"
#include "LittleFS.h"

const char *RESP_CODE_OK  PROGMEM="HTTP/1.1 200 OK";
const char *RESP_TYPE_HTML  PROGMEM="Content-Type: text/html";
const char *RESP_CLOSE  PROGMEM="Connection: close";

const char *BTN_CANCEL PROGMEM ="<button onclick=\"window.location.href = '/';return false;\">Cancel</button>";
const char *BTN_UPDATE PROGMEM ="<button onclick=\"window.location.href = '/update';return false;\">Update</button>";

void handleWebRequests() {
  if (_wm->AuthenticationRequired&&!_wm->server->authenticate(_wm->AuthUser,_wm->AuthPassword))
  {
    _wm->server->requestAuthentication(HTTPAuthMethod::DIGEST_AUTH);
  }

  if (loadFromFlash(_wm->server->uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += _wm->server->uri();
  message += "\nMethod: ";
  message += (_wm->server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += _wm->server->args();
  message += "\n";
  for (uint8_t i = 0; i < _wm->server->args(); i++) {
    message += " NAME:" + _wm->server->argName(i) + "\n VALUE:" + _wm->server->arg(i) + "\n";
  }
  _wm->server->send(404, "text/plain", message);
  _Log.println(LogObject::DebugLevels::ErrorOnly,message);
}
// get file from spiffs
bool loadFromFlash(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path=="/check") return checkRequest();
  if (path=="/update") return upDateRequest();


  _wm->server->sendHeader(F("Cache-Control"), F("max-age=290304000, public"));

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html")||path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".log")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";

  if (path.startsWith("/script.js")||path.startsWith("/favicon.ico"))
  {
    path+=".gz";
  };

  _Log.print(LogObject::DebugLevels::Verbose,"Page request :");
  _Log.println(LogObject::DebugLevels::Verbose,path.c_str());
  File dataFile = LittleFS.open(path.c_str(), "r");

  if (_wm->server->hasArg("download")) dataType = "application/octet-stream";
  if (_wm->server->streamFile(dataFile, dataType) != dataFile.size()) {}

  dataFile.close();
  return true;
}

bool upDateRequest()
{
  _Log.print(LogObject::DebugLevels::Verbose,"Updating");
  WiFiClient  _client = _wm->server->client();
  sendHeader();
  _client.println(FPSTR("<head>"));
  _client.println(FPSTR("<META HTTP-EQUIV='refresh' CONTENT='120;URL=/'>"));
  _client.println(FPSTR(HTTP_STYLE));
  _client.println(FPSTR("</head>"));
  _client.print(FPSTR("<div class='wrap'><div class='msg'>"));
  _client.print("Updating...");
  _client.println(FPSTR("<br/></div></div>"));
  delay(1);
  _client.stop();

  _updater.Updates(true);
  
  return true;
}

bool checkRequest()
{
  if (_updater.State=="")
  {
    _updater.State="Checking...<br>";
  }

  _Log.print(LogObject::DebugLevels::Verbose,"Send update state");
  WiFiClient  _client = _wm->server->client();
  sendHeader();
  _client.println(FPSTR("<head>"));
  _client.println(FPSTR("<META HTTP-EQUIV='refresh' CONTENT='3;URL=/check'>"));
  _client.println(FPSTR(HTTP_STYLE));
  _client.println(FPSTR("</head>"));
  _client.print(FPSTR("<div class='wrap'><div class='msg'>"));
  _client.print(_updater.State);
  
  delay(1);

  if (_updater.State=="Checking...<br>")
  {
    _updater.CheckUpdate();
  }
  else
  {
    if (_updater.GetOnLineVersion()<_updater.GetCurrentVersion())
      _client.println(BTN_CANCEL);
    else
      _client.println(BTN_UPDATE);
  }
  
  _client.println(FPSTR("<br/></div></div>"));
  
  _client.stop();
  
  return true;
}


void sendHeader()
{
  _wm->server->client().println(FPSTR(RESP_CODE_OK));
  _wm->server->client().println(FPSTR(RESP_TYPE_HTML));
  _wm->server->client().println(FPSTR(RESP_CLOSE));
  _wm->server->client().println();
}
