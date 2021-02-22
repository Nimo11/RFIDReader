#include "Global.h"
#include "LanguageManager.h"

//LanguageManager _Lm;
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
const char *WM_INPUT_IP_LABEL PROGMEM = "</div><div class=\"msg P\">Static IP";
const char *WM_INPUT_GW_LABEL PROGMEM = "Gateway IP";
const char *WM_INPUT_MASQ_LABEL PROGMEM = "Masque";

const char *WM_INPUT_SERVER_LABEL PROGMEM = "</div><div class=\"msg P\">API Server URL";
const char *WM_INPUT_USER PROGMEM = "API User";
const char *WM_INPUT_PASSWORD PROGMEM = "API Password";
const char *WM_INPUT_OK_RESPONSE_LABEL PROGMEM = "OK string";
const char *WM_INPUT_UID_NODE_LABEL PROGMEM = "UID card node";

const char *WM_INPUT_BTN_UPDATE PROGMEM= "</div><button onclick=\"window.location.href = '/check?start';return false;\">Check update</button><br><br><div>";
const char *WM_INPUT_BTN_CONFIG PROGMEM= "</div><button onclick=\"window.location.href = '/config.json?download';return false;\">Export config</button><br><br><div>";


ConfigRef _paramConfig[11] = {
    ConfigRef(_config.admin_pass, CFG_JSON_ADMIN_PASSWORD, WM_INPUT_ADMIN_LABEL, sizeof(_config.admin_pass)),

    ConfigRef(_config.IP, CFG_JSON_IP, WM_INPUT_IP_LABEL, sizeof(_config.IP)),
    ConfigRef(_config.GW, CFG_JSON_GW, WM_INPUT_GW_LABEL, sizeof(_config.GW)),
    ConfigRef(_config.MASQ, CFG_JSON_MASQ, WM_INPUT_MASQ_LABEL, sizeof(_config.MASQ)),
    
    ConfigRef(_config.url_server, CFG_JSON_NODE_URL_SERVER, WM_INPUT_SERVER_LABEL, sizeof(_config.url_server)),
    ConfigRef(_config.response_Ok, CFG_JSON_NODE_RESPONSE_OK, WM_INPUT_OK_RESPONSE_LABEL, sizeof(_config.response_Ok)),
    ConfigRef(_config.uidNode, CFG_JSON_UID_NODE, WM_INPUT_UID_NODE_LABEL, sizeof(_config.uidNode)),
    ConfigRef(_config.user, CFG_JSON_USER, WM_INPUT_USER, sizeof(_config.user)),
    ConfigRef(_config.password, CFG_JSON_PASSWORD, WM_INPUT_PASSWORD, sizeof(_config.password)),
    ConfigRef(WM_INPUT_BTN_UPDATE),
    ConfigRef(WM_INPUT_BTN_CONFIG)};


#define SS_PIN D2
#define RST_PIN 5

// warning mfrc522(ss_pin) not working anymore
MFRC522 _rfid(SS_PIN, RST_PIN); //

//MFRC522::MIFARE_Key key;
byte _nuidPICC[4];

unsigned long _lastRead;
