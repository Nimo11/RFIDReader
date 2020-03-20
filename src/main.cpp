/**************************************************************
   Ce sketch permet de connaître l'IUD d'un tag RFID
   branché à un ESP8266 ou un ESP32.
   
   Utilisation de la bibliothèque MFRC522 par Miguel Balboa
   
   https://electroniqueamateur.blogspot.com/2019/02/module-rfid-rc522-et-esp8266.html
   
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
	
	WiFi.hostname(F("RFIDReader"));
	
	SPI.begin();	 // Init SPI bus
	_rfid.PCD_Init(); // Init MFRC522
	_rfid.PCD_AntennaOff();
	delay(3000);
	
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

		//checkForUpdates();
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
