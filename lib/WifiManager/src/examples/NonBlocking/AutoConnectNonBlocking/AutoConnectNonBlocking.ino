#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
WiFiManager _wm;

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP    
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    _wm.setConfigPortalBlocking(false);

    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    if(_wm.autoConnect("AutoConnectAP")){
        Serial.println("connected...yeey :)");
    }
    else {
        Serial.println("Configportal running");
    }
}

void loop() {
    _wm.process();
    // put your main code here, to run repeatedly:
}
