/**
 * OnDemandNonBlocking.ino
 * example of running the webportal or configportal manually and non blocking
 * trigger pin will start a webportal for 120 seconds then turn it off.
 * startCP = true will start both the configportal AP and webportal
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

WiFiManager _wm;

unsigned int  timeout   = 120; // seconds to run for
unsigned int  startTime = millis();
bool portalRunning      = false;
bool startAP            = false; // start AP and webserver if true, else start only webserver

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  // put your setup code here, to run once
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
}

void loop() {
  doWiFiManager();
  // put your main code here, to run repeatedly:
}

void doWiFiManager(){
  // is auto timeout portal running
  if(portalRunning){
    _wm.process();
    if((millis()-startTime) > (timeout*1000)){
      Serial.println("portaltimeout");
      portalRunning = false;
      if(startAP){
        _wm.stopConfigPortal();
      }  
      else{
        _wm.stopWebPortal();
      } 
   }
  }

  // is configuration portal requested?
  if(digitalRead(TRIGGER_PIN) == LOW && (!portalRunning)) {
    if(startAP){
      Serial.println("Button Pressed, Starting Config Portal");
      _wm.setConfigPortalBlocking(false);
      _wm.startConfigPortal();
    }  
    else{
      Serial.println("Button Pressed, Starting Web Portal");
      _wm.startWebPortal();
    }  
    portalRunning = true;
    startTime = millis();
  }
}


