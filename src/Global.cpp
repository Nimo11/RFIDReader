#include "Global.h"
#include "LanguageManager.h"

LanguageManager _Lm;
LogObject _Log;
HTTPClient _httpClient;
WiFiClient _client;
WiFiManager *_wm;
WiFiUDP _Udp;
String _Cookie = "";
Config _config;
GITUpdater _updater(GITUpdater::Languages::LANG_FR);

Session _session;

const char *WM_INPUT_ADMIN_LABEL PROGMEM = "<div class=\"msg P\">Admin password";
const char *WM_INPUT_IP_LABEL PROGMEM = "Static IP";

const char *WM_INPUT_SERVER_LABEL PROGMEM = "</div><div class=\"msg P\">API Server URL";
const char *WM_INPUT_USER PROGMEM = "API User";
const char *WM_INPUT_PASSWORD PROGMEM = "API Password";
const char *WM_INPUT_OK_RESPONSE_LABEL PROGMEM = "OK string";
const char *WM_INPUT_UID_NODE_LABEL PROGMEM = "UID card node";

ConfigRef _paramConfig[7] PROGMEM = {
    ConfigRef(_config.admin_pass, "adminPassword", WM_INPUT_ADMIN_LABEL, sizeof(_config.admin_pass)),
    ConfigRef(_config.url_server, "server", WM_INPUT_SERVER_LABEL, sizeof(_config.url_server)),
    ConfigRef(_config.IP, "ip", WM_INPUT_IP_LABEL, sizeof(_config.IP)),
    ConfigRef(_config.response_Ok, "rok", WM_INPUT_OK_RESPONSE_LABEL, sizeof(_config.response_Ok)),
    ConfigRef(_config.uidNode, "uidnode", WM_INPUT_UID_NODE_LABEL, sizeof(_config.uidNode)),
    ConfigRef(_config.user, "user", WM_INPUT_USER, sizeof(_config.user)),
    ConfigRef(_config.password, "password", WM_INPUT_PASSWORD, sizeof(_config.password)),
};



#define SS_PIN 4
#define RST_PIN 5

// warning mfrc522(ss_pin) not working anymore
MFRC522 _rfid(SS_PIN, RST_PIN); //

//MFRC522::MIFARE_Key key;
byte _nuidPICC[4];

unsigned long _lastRead;
