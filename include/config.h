//--------------------------------------------------------------------------------------
//  Config manager
//--------------------------------------------------------------------------------------
#ifndef config_h
#define config_h

#include <Arduino.h>
#include <ArduinoJson.h>

const char MSG_SPIFF_STARTED[]  ="SPIFFS started.";
const char MSG_SPIFF_CONTENTS[]  ="Contents:";
const char MSG_SPIFF_FILE_TEMPLATE[]  ="\tFS File: %s, size: %s\r\n";
const char MSG_SPIFF_ERROR[]  ="SPIFFS error.";
const char MSG_SPIFF_SAVE_CFG[]  ="Saving config";
const char MSG_SPIFF_LOAD_CFG[]  ="Loading config";
const char MSG_SPIFF_SAVE_CFG_ERROR[]  ="Failed to save config file for writing";
const char MSG_SPIFF_SAVE_CFG_DESERIALIZE_ERROR[]  ="Deserialize failed with code :";
const char MSG_SPIFF_PATH_CFG[]  ="/config.json";

const char CFG_JSON_NODE_URL_SERVER[]  ="url_server";
const char CFG_JSON_IP[]  ="ip";
const char CFG_JSON_USER[]  ="user";
const char CFG_JSON_NODE_RESPONSE_OK[]  ="ok_value";
const char CFG_JSON_PASSWORD[]="password";
const char CFG_JSON_UID_NODE[]="uidnode";

struct Config
{
  char url_server[150] = "https://";
  char IP[10] = "";
  char response_Ok[20]="ok";
  char user[20]="";
  char password[20]="";
  char uidNode[20]="";
};

extern void startSPIFFS();

String formatBytes(size_t bytes);

extern void saveParam();

extern void loadConfig();

template <typename T>
void setFromJson(T obj,JsonObject jsonObj,const char propertyName[]);

void TestSpiffs();

#endif