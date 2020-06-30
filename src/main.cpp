/**************************************************************

   
*****************************************************************/

#include <SPI.h>
#include <iostream>
#include <cstring>
#include "ESP8266WiFi.h"
#include "Global.h"
#include "config.h"
#include "LogObject.h"
#include "mw_wifimanager.h"
#include "RFID.h"
#include "api.h"
#include "version.h"
#include "Buzz.h"
#include <ESP8266WebServerSecure.h>

#define BUZZ_PIN 16 // Buzzer

const char *www_username = "admin";
const char *www_password = "esp8266";

Buzz _buzz(BUZZ_PIN);

void setup()
{
	Serial.begin(115200);



	_Log.debugType = LogObject::DebugType::SerialType;
	_Log.level = LogObject::DebugLevels::Verbose;

	_updater.SetCurrentVersion(BUILD_NUMBER);
	_updater.SetGITProjectURL("https://github.com/Nimo11/RFIDReader/master");

	SPI.begin();
	_rfid.PCD_Init(); // Init MFRC522

	_rfid.PCD_DumpVersionToSerial();
	_rfid.PCD_AntennaOff();
	delay(2000);

	startSPIFFS();

	loadConfig();

	WiFi.hostname(F("RFIDReader"));
	setWifiManagerMenu();
	_lastRead = millis();

	pinMode(BUZZ_PIN, OUTPUT);
	_buzz.buzz(1);

	if (_wm->autoConnect("RFIDReader"))
	{
		_Log.print(LogObject::DebugLevels::Normal, F("IP Address "));
		_Log.println(LogObject::DebugLevels::Normal, WiFi.localIP().toString());

		//setOTA();

		_wm->startWebPortal();

		//if(_updater.CheckUpdate())
		//	{
		//_updater.Updates();
		//	}
		_Log.println(LogObject::DebugLevels::Normal, F("Starting work."));
		_buzz.buzz(3);
	}
	else
	{
		_Log.println(LogObject::DebugLevels::Normal, F("Wifi not found wait 15s!"));
		_buzz.continuousBuzz(4);
		delay(15000);
		//reset and try again, or maybe put it to deep sleep
		ESP.reset();
		delay(1000);
	}
}

void loop()
{

	_wm->server->handleClient();

	_rfid.PCD_AntennaOn();
	uint32 id = RFIDRead();
	_rfid.PCD_AntennaOff();

	if (id > 0)
	{
		_buzz.buzz(1);

		if (!sendAPIRequest(
				_config.url_server,
				String(id),
				_config.user,
				_config.password,
				_config.response_Ok,
				_config.uidNode))
		{
			_buzz.continuousBuzz(4);
		}
		else
		{
			_buzz.buzz(3);
		}
	}
}
