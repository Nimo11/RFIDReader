//--------------------------------------------------------------------------------------
//  Config manager
//--------------------------------------------------------------------------------------
#include "config.h"
#include "LittleFS.h"
#include <ArduinoJson.h>
#include "LogObject.h"
#include "Global.h"

void startLittleFS()
{ // Start the SPIFFS and list all contents
  if (LittleFS.begin())
  { // Start the SPI Flash File System (SPIFFS)
    _Log.print(LogObject::DebugLevels::Normal, MSG_SPIFF_STARTED);
    _Log.println(LogObject::DebugLevels::Verbose, MSG_SPIFF_CONTENTS);
    {
      Dir dir = LittleFS.openDir("/");
      while (dir.next())
      { // List the file system contents
        String fileName = dir.fileName();
        size_t fileSize = dir.fileSize();
        _Log.printf(LogObject::DebugLevels::Verbose, MSG_SPIFF_FILE_TEMPLATE, fileName.c_str(), formatBytes(fileSize).c_str());
      }
      _Log.print(LogObject::DebugLevels::Verbose, "\n");
    }
  }
  else
  {
    _Log.print(LogObject::DebugLevels::Normal, MSG_SPIFF_ERROR);
  }
}

String formatBytes(size_t bytes)
{ // convert sizes in bytes to KB and MB
  if (bytes < 1024)
  {
    return String(bytes) + "B";
  }
  else if (bytes < (1024 * 1024))
  {
    return String(bytes / 1024.0) + "KB";
  }
  else if (bytes < (1024 * 1024 * 1024))
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }

  return String(bytes);
}

void saveParam()
{
  StaticJsonDocument<1024> doc;

  for (ConfigRef &p : _paramConfig)
  {
    if(p.WmParam.getID()!=NULL)
    {
      _Log.print(LogObject::DebugLevels::Verbose, p.WmParam.getID());
      _Log.print(LogObject::DebugLevels::Verbose," : ");
      _Log.println(LogObject::DebugLevels::Verbose, p.WmParam.getValue());
      strcpy(p.CfgPtr, p.WmParam.getValue()); //update config value from web form
      doc[p.WmParam.getID()] = p.CfgPtr;      //build json
    }
  }

  //save value
  _Log.println(LogObject::DebugLevels::Verbose, MSG_SPIFF_SAVE_CFG);

  File configFile = LittleFS.open(MSG_SPIFF_PATH_CFG, "w");
  if (!configFile)
  {
    _Log.println(LogObject::DebugLevels::ErrorOnly, MSG_SPIFF_SAVE_CFG_ERROR);
  }

  serializeJson(doc, configFile);
  configFile.close();

  configChanged();
}

void loadConfig()
{
  //read configuration from FS json
  File configFile = LittleFS.open(MSG_SPIFF_PATH_CFG, "r");

  if (configFile)
  {
    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);

    configFile.readBytes(buf.get(), size);

    DynamicJsonDocument doc(size * 2);
    DeserializationError err = deserializeJson(doc, buf.get());

    configFile.close();

    if (err == DeserializationError::Ok)
    {
      for (ConfigRef &p : _paramConfig)
      {
        if (p.WmParam.getID()!=NULL&&doc.containsKey(p.WmParam.getID()))
        {
          std::string str = "string";
          strcpy(p.CfgPtr, doc[p.WmParam.getID()]);
          p.WmParam.setValue(p.CfgPtr, p.Size);
          _Log.print(LogObject::DebugLevels::Verbose, p.WmParam.getID());
          _Log.print(LogObject::DebugLevels::Verbose, " : ");
          _Log.println(LogObject::DebugLevels::Verbose, p.WmParam.getValue());
        }
        else
        {
          _Log.print(LogObject::DebugLevels::Verbose, "Param not found");
          _Log.println(LogObject::DebugLevels::Verbose, p.WmParam.getValue());
        }
        
      }

      _Log.println(LogObject::DebugLevels::Normal, MSG_SPIFF_LOAD_CFG);
      configChanged();
    }
    else
    {
      _Log.print(LogObject::DebugLevels::ErrorOnly, MSG_SPIFF_SAVE_CFG_DESERIALIZE_ERROR);
      _Log.println(LogObject::DebugLevels::ErrorOnly, err.c_str());
    }
  }
}

void configChanged(){
  if(nullptr != _config.changeHandler){
    _config.changeHandler(0);
  }
}


void TestSpiffs()
{
  //SPIFFS.format();
  File file = LittleFS.open("/file.txt", "w");

  if (!file)
  {
    Serial.println("Error opening file for writing");
    return;
  }

  int bytesWritten = file.print("TEST LittleFS");
  if (bytesWritten > 0)
  {
    Serial.println("File was written");
    Serial.println(bytesWritten);
  }
  else
  {
    Serial.println("File write failed");
  }
  file.close();
  file = LittleFS.open("/file.txt", "r");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();
}