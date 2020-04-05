#include "Global.h"

LogObject _Log;
HTTPClient _httpClient;
WiFiClient _client;
WiFiManager _wm;
WiFiUDP _Udp;
String _Cookie="";
Config _config;
GITUpdater _updater;

Session _session;

const char *WM_INPUT_SERVER_LABEL = "Server URL";
const char *WM_INPUT_NODE_LABEL = "IP";
const char *WM_INPUT_USER = "User";
const char *WM_INPUT_PASSWORD = "Password";
const char *WM_INPUT_OK_RESPONSE_LABEL = "OK string";
const char *WM_INPUT_UID_NODE_LABEL = "UID card node";

WiFiManagerParameter _url_server_param("server",WM_INPUT_SERVER_LABEL , _config.url_server, 150);
WiFiManagerParameter _IP_param("ip",WM_INPUT_NODE_LABEL , _config.IP, 10);
WiFiManagerParameter _response_Ok_param("rok",WM_INPUT_OK_RESPONSE_LABEL , _config.response_Ok, 20);
WiFiManagerParameter _UidNode_param("uidnode",WM_INPUT_UID_NODE_LABEL , _config.uidNode, 20);
WiFiManagerParameter _User_param("user",WM_INPUT_USER , _config.user, 20);
WiFiManagerParameter _Password_param("password",WM_INPUT_PASSWORD , _config.password, 20);

const int _pinRST = 5;  // pin RST du module RC522
const int _pinSDA = 4; // pin SDA du module RC522
const int _pinBuz = 16; // Buzzer

MFRC522 _rfid(_pinSDA);//, _pinRST

//MFRC522::MIFARE_Key key; 
byte _nuidPICC[4];

unsigned long _lastRead;



