/**
 * This is a kind of unit test for DEV for now
 * It contains many of the public methods
 * 
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <time.h>
#include <stdio.h>

#define USEOTA
// enable OTA
#ifdef USEOTA
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif

#define TRIGGER_PIN 0
const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

unsigned long mtime = 0;

// // #define MYOLED

// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// #ifdef MYOLED 

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 32 // OLED display height, in pixels

// // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// void init_oled(){
//   Wire.begin(SCL,SDA);  // begin(sda, scl) SWAP!
//   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
//     Serial.println(F("SSD1306 allocation failed"));
//   }

//   display.clearDisplay();
//   display.setTextSize(1);             // Normal 1:1 pixepl scale
//   display.setTextColor(WHITE);        // Draw white text
//   display.setCursor(0,0);             // Start at top-left corner
//   display.display();
// }

// void print_oled(String str,uint8_t size){
//   display.clearDisplay();
//   display.setTextSize(size);
//   display.setTextColor(WHITE);
//   display.setCursor(0,0);
//   display.println(str);
//   display.display();
// }
// #else
//   void print_oled(String str,uint8_t size){
//     (void)str;
//     (void)size;
//   }
// #endif

void print_oled(String str,uint8_t size){
  (void)str;
  (void)size;
}

WiFiManager _wm;

// OPTION FLAGS
bool TEST_CP  = true; // always start the configportal, even if ap found
bool TEST_NET = true; // do a network test after connect, (gets ntp time)
bool ALLOWONDEMAND = true;

// char ssid[] = "*************";  //  your network SSID (name)
// char pass[] = "********";       // your network password

void saveWifiCallback(){
  Serial.println("[CALLBACK] saveCallback fired");
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("[CALLBACK] configModeCallback fired");
  #ifdef ESP8266
    print_oled("WiFiManager Waiting\nIP: " + WiFi.softAPIP().toString() + "\nSSID: " + WiFi.softAPSSID(),1); 
  #endif  
  // myWiFiManager->setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); 
  // Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  // Serial.println(myWiFiManager->getConfigPortalSSID());
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  // wm.stopConfigPortal();
}

void bindServerCallback(){
  _wm.server->on("/custom",handleRoute);
  // wm.server->on("/info",handleRoute); // can override wm!
}

void handleRoute(){
  Serial.println("[HTTP] handle route");
  _wm.server->send(200, "text/plain", "hello from user code");
}

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial1.begin(115200);

  // Serial.setDebugOutput(true);  
  delay(1000);
  // Serial1.println("TXD1 Enabled");

  Serial.println("\n Starting");
  // WiFi.setSleepMode(WIFI_NONE_SLEEP); // disable sleep, can improve ap stability
  
  #ifdef OLED
  init_oled();
  #endif
  print_oled(F("Starting..."),2);
  _wm.debugPlatformInfo();

  //Local intialization. Once its business is done, there is no need to keep it around
  //reset settings - for testing
  // wm.resetSettings();
  // wm.erase();
  
  _wm.setClass("invert");

  WiFiManagerParameter custom_html("<p>This Is Custom HTML</p>"); // only custom html
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "", 6);
  WiFiManagerParameter custom_token("api_token", "api token", "", 16);
  WiFiManagerParameter custom_tokenb("invalid token", "invalid token", "", 0); // id is invalid, cannot contain spaces
  WiFiManagerParameter custom_ipaddress("input_ip", "input IP", "", 15,"pattern='\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}'"); // custom input attrs (ip mask)

  const char _customHtml_checkbox[] = "type=\"checkbox\""; 
  WiFiManagerParameter custom_checkbox("checkbox", "my checkbox", "T", 2, _customHtml_checkbox, WFM_LABEL_AFTER);

  // callbacks
  _wm.setAPCallback(configModeCallback);
  _wm.setWebServerCallback(bindServerCallback);
  _wm.setSaveConfigCallback(saveWifiCallback);
  _wm.setSaveParamsCallback(saveParamCallback);

  //add all your parameters here
  _wm.addParameter(&custom_html);
  _wm.addParameter(&custom_mqtt_server);
  _wm.addParameter(&custom_mqtt_port);
  _wm.addParameter(&custom_token);
  _wm.addParameter(&custom_tokenb);
  _wm.addParameter(&custom_ipaddress);
  _wm.addParameter(&custom_checkbox);

  custom_html.setValue("test",4);
  custom_token.setValue("test",4);

  // Set cutom menu via menu[] or vector
  // const char* menu[] = {"wifi","wifinoscan","info","param","close","sep","erase","restart","exit"};
  // wm.setMenu(menu,9); // custom menu array must provide length

  std::vector<const char *> menu = {"wifi","wifinoscan","info","param","close","sep","erase","restart","exit"};
  // wm.setMenu(menu); // custom menu, pass vector
  
  _wm.setParamsPage(true); // move params to seperate page, not wifi, do not combine with setmenu!

  // set static sta ip
  // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  // wm.setShowStaticFields(false);
  // wm.setShowDnsFields(false);

  // set static ip
  // wm.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  // wm.setAPStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); 

  // set country
  _wm.setCountry("US"); // setting wifi country seems to improve OSX soft ap connectivity, may help others as well
  
  // set channel
  // wm.setWiFiAPChannel(13);
  
  // set AP hidden
  // wm.setAPHidden(true);

  // show password publicly!
  // wm.setShowPassword(true);

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep in seconds
    _wm.setConfigPortalTimeout(120);
  
  // wm.setConnectTimeout(20);
  // wm.setShowStaticFields(true);
  
  // wm.startConfigPortal("AutoConnectAP", "password");
  
  // wm.setCleanConnect(true); // disconenct before connect, clean connect
  
  // wm.setBreakAfterConfig(true);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  wifiInfo();
  print_oled(F("Connecting..."),2);  
  if(!_wm.autoConnect("WM_AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
    print_oled("Not Connected",2);
  }
  else if(TEST_CP) {
    delay(1000);
    Serial.println("TEST_CP ENABLED");
    // start configportal always
    _wm.setConfigPortalTimeout(60);
    _wm.startConfigPortal("WM_ConnectAP");
  }
  else {
    //if you get here you have connected to the WiFi
     Serial.println("connected...yeey :)");
      print_oled("Connected\nIP: " + WiFi.localIP().toString() + "\nSSID: " + WiFi.SSID(),1);    
  }
  
  wifiInfo();
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  #ifdef USEOTA
    ArduinoOTA.begin();
  #endif
}

void wifiInfo(){
  WiFi.printDiag(Serial);
  Serial.println("SAVED: " + (String)_wm.getWiFiIsSaved() ? "YES" : "NO");
  Serial.println("SSID: " + (String)_wm.getWiFiSSID());
  Serial.println("PASS: " + (String)_wm.getWiFiPass());
}

void loop() {

  #ifdef USEOTA
  ArduinoOTA.handle();
  #endif
  // is configuration portal requested?
  if (ALLOWONDEMAND && digitalRead(TRIGGER_PIN) == LOW ) {
    delay(100);
    if ( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("BUTTON PRESSED");
      _wm.setConfigPortalTimeout(140);
      _wm.setParamsPage(false); // move params to seperate page, not wifi, do not combine with setmenu!

      // disable captive portal redirection
      // wm.setCaptivePortalEnable(false);
      
      if (!_wm.startConfigPortal("OnDemandAP","12345678")) {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
      }
    }
    else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
      print_oled("Connected\nIP: " + WiFi.localIP().toString() + "\nSSID: " + WiFi.SSID(),1);    
      getTime();
    }
  }

  if(WiFi.status() == WL_CONNECTED && millis()-mtime > 10000 ){
    getTime();
    mtime = millis();
  }
  // put your main code here, to run repeatedly:
  delay(100);
}

void getTime() {
  int tz           = -5;
  int dst          = 0;
  time_t now       = time(nullptr);
  unsigned timeout = 5000;
  unsigned start   = millis();  
  configTime(tz * 3600, dst * 3600, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  while (now < 8 * 3600 * 2 ) {
    delay(100);
    Serial.print(".");
    now = time(nullptr);
    if((millis() - start) > timeout){
      Serial.println("[ERROR] Failed to get NTP time.");
      return;
    }
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void debugchipid(){
  // WiFi.mode(WIFI_STA);
  // WiFi.printDiag(Serial);
  // Serial.println(modes[WiFi.getMode()]);
  
  // ESP.eraseConfig();
  // wm.resetSettings();
  // wm.erase(true);
  WiFi.mode(WIFI_AP);
  // WiFi.softAP();
  WiFi.enableAP(true);
  delay(500);
  // esp_wifi_start();
  delay(1000);
  WiFi.printDiag(Serial);
  delay(60000);
  ESP.restart();

  // AP esp_267751
  // 507726A4AE30
  // ESP32 Chip ID = 507726A4AE30
}