#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
WiFiManager _wm;
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP    
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //reset settings - wipe credentials for testing
    //wm.resetSettings();
    _wm.addParameter(&custom_mqtt_server);
    _wm.setConfigPortalBlocking(false);
    _wm.setSaveParamsCallback(saveParamsCallback);

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

void saveParamsCallback () {
  Serial.println("Get Params:");
  Serial.print(custom_mqtt_server.getID());
  Serial.print(" : ");
  Serial.println(custom_mqtt_server.getValue());
}
