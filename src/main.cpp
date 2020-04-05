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

void buzz(int buzzCount)
{
	for (int i = 0; i < buzzCount; i++)
	{
		digitalWrite(_pinBuz, HIGH); // Faire du bruit
		delay(100);					// Attendre 10ms
		digitalWrite(_pinBuz, LOW);  // Silence
		delay(100);
	}
}

void setup()
{
	Serial.begin(115200);

	_Log.debugType = LogObject::DebugType::SerialType;
	_Log.level = LogObject::DebugLevels::Verbose;

//const char* _fwUrlBase = "https://raw.githubusercontent.com/Nimo11/MyWattmeter/master/.pio/build/esp12e/";
	_updater.SetCurrentVersion(BUILD_NUMBER);
	_updater.SetGITProjectURL("https://github.com/Nimo11/RFIDReader");

	WiFi.hostname(F("RFIDReader"));
	
	SPI.begin();	 // Init SPI bus
	_rfid.PCD_Init(); // Init MFRC522
	_rfid.PCD_AntennaOff();
	delay(3000);
	
	_wm.setCountry("FR");
	//_wm.WiFiManagerInit();
	startSPIFFS();
	loadConfig();
	setWifiManagerMenu();

	_lastRead = millis();

	pinMode(_pinBuz, OUTPUT);
	buzz(1);

	if (_wm.autoConnect("RFIDReader"))
	{
		_Log.print(LogObject::DebugLevels::Normal, F("IP Address "));
		_Log.println(LogObject::DebugLevels::Normal, WiFi.localIP().toString());

		//setOTA();

		_wm.startWebPortal();

		//wm.server->onNotFound(handleWebRequests);

		_updater.CheckUpdate();
		buzz(3);
	}
	else
	{
		_Log.println(LogObject::DebugLevels::Normal, F("Config portal running!"));
		buzz(10);
	}
}

void loop()
{
	_wm.process();
	_rfid.PCD_AntennaOn();
	uint32 id = RFIDRead();
	_rfid.PCD_AntennaOff();

	if (id > 0)
	{
		buzz(1);

		if (!sendAPIRequest(
				_config.url_server,
				String(id),
				_config.user,
				_config.password,
				_config.response_Ok,
				_config.uidNode))
		{
			buzz(8);
		}

		buzz(1);
	}
}
