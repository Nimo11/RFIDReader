#include <api.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "Global.h"
#include "ArduinoJson.h"

///
/// return true if koString wasn't found
///
bool sendAPIRequest(const String url,const String uid,const char* user,const char* password,const String koString,String uidNode)
{
  if ((WiFi.status() == WL_CONNECTED))
  {
    BearSSL::WiFiClientSecure httpsClient;
    HTTPClient http;
    unsigned long time = millis();
    httpsClient.setSession(&_session);

    //httpsClient.setCiphersLessSecure();
    httpsClient.setCiphers({BR_TLS_RSA_WITH_AES_256_CBC_SHA256,
                            BR_TLS_RSA_WITH_AES_128_CBC_SHA256,
                            BR_TLS_RSA_WITH_AES_256_CBC_SHA,
                            BR_TLS_RSA_WITH_AES_128_CBC_SHA});
    httpsClient.setInsecure();

    const char *headerkeys[] = {"Set-Cookie"};
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);
    http.collectHeaders(headerkeys, headerkeyssize);
    http.begin(httpsClient, url); //Specify the URL
    
    http.setAuthorization(user,password);

    http.addHeader("Content-Type", "application/json");
    http.addHeader("User-Agent", "Mozilla/5.0 (ESP8266)");
    if (_Cookie != "")
    {
      http.addHeader("Cookie", _Cookie);
    }

    int httpCode = http.POST("{'"+uidNode+"':'" + uid + "'}"); //Make the request
    bool found = false;

    if (httpCode > 0)
    {
      Serial.println("HTTP response received.");
      Serial.println(millis() - time);
      _Cookie = http.header("Set-Cookie").c_str();

      int c;
      ulong startMillis = millis();
      ulong timeout = 6000;
      
      uint searchMatch = 0;
      uint len = http.getSize();

      do
      {
        c = httpsClient.read();
        if (c >= 0)
        {
          len--;

          Serial.print((char)c);
          if (koString[searchMatch] == c)
          {
            searchMatch++;
            found = searchMatch == koString.length();
            if (found)
              break;
          }
          else
          {
            searchMatch = 0;
          }
        }
        if (timeout == 0)
          break;
        yield();
      } while (millis() - startMillis < timeout && len > 0);

      http.end();
      return found; 
    }
    else
    {
      Serial.print(F("Error on HTTP request. Error code:"));
      Serial.println(httpCode);
    }

    http.end();

    return false;
  }
}

void sendAPIRequest2(String url)
{
  const char *headerkeys[] = {"User-Agent", "Set-Cookie", "Cookie", "Date", "Content-Type", "Connection"};
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);

  String uri = "";
  String server = "";

  String user = "";
  String password = "";

  HTTPClient http; //must be declared after WiFiClient for correct destruction order, because used by http.begin(client,...)

  Serial.print("[HTTP] begin...\n");

  // configure traged server and url
  http.begin(_client, String(server) + String(uri));

  const char *keys[] = {"WWW-Authenticate"};
  http.collectHeaders(keys, 1);

  http.collectHeaders(headerkeys, headerkeyssize);
  Serial.printf("Header count: %d\r\n", http.headers());

  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  for (int i = 0; i < http.headers(); i++)
  {
    Serial.printf("%s = %s\r\n", http.headerName(i).c_str(), http.header(i).c_str());
  }
  Serial.printf("Cookie: %s\r\n", http.header("Cookie").c_str());
  Serial.printf("Set-Cookie: %s\r\n", http.header("Set-Cookie").c_str());

  http.setReuse(true);

  if (httpCode > 0)
  {
    String authReq = http.header("WWW-Authenticate");
    Serial.println(authReq);

    String realm = exractParam(authReq, "realm=\"", '"');
    String nonce = exractParam(authReq, "nonce=\"", '"');
    String opaque = exractParam(authReq, "opaque=\"", '"');
    String algo = exractParam(authReq, "algorithm=\"", '"');
    String qop = exractParam(authReq, "qop=\"", '"');
    String cnonce = getCNonce(8); //random string
    String nc = "00000001";

    // parameters for the RFC 2617 newer Digest
    MD5Builder md5;
    md5.begin();
    md5.add(user + ":" + realm + ":" + password); // md5 of the user:realm:user
    md5.calculate();
    String HA1 = md5.toString();

    if (algo = "MD5-sess")
    {
      algo = "MD5-sess";
      Serial.println("MD5-sess");
      md5.begin();
      md5.add(HA1 + ":" + nonce + ":" + cnonce); // MD5(MD5(username:realm:password):nonce:cnonce)
      md5.calculate();
      HA1 = md5.toString();
    }
    Serial.println(HA1);
    md5.begin();
    if (qop.indexOf("auth-int") > -1)
    {
      md5.add("GET:" + uri + ":"); // MD5(method:digestURI:MD5(entityBody)) To do entitybody
    }
    else
      md5.add("GET:" + uri);
    md5.calculate();
    String HA2 = md5.toString();
    Serial.println(HA2);
    md5.begin();
    if (qop == "")
      md5.add(HA1 + ":" + nonce + ":" + HA2); // MD5(HA1:nonce:HA2)
    else
      md5.add(HA1 + ":" + nonce + ":" + nc + ":" + cnonce + ":" + qop + ":" + HA2); //MD5(HA1:nonce:nonceCount:cnonce:qop:HA2)
    md5.calculate();
    String response = md5.toString();
    Serial.println(response);
    http.end();
    http.begin(server + uri);

    String authorization = "Digest username=\"" + user + "\", realm=\"" + realm + "\", nonce=\"" + nonce + "\", uri=\"" + uri + "\", algorithm=" + algo + ", qop=\"auth\", nc=" + nc + ", cnonce=\"" + cnonce + "\", response=\"" + response + "\"";

    if (opaque != "")
      authorization += ", opaque=\"" + opaque + "\"";

    Serial.println(authorization);
    http.addHeader("Authorization", authorization);

    int httpCode = http.GET();
    if (httpCode > 0)
    {
      String payload = http.getString();
      Serial.println(payload);
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  delay(10000);
}

/// return false if node exist and is not null
bool deserialize(String json)
{
  StaticJsonDocument<5000> doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  if (doc.containsKey("error"))
    return doc["error"] == "";

  return true;
}

String exractParam(String &authReq, const String &param, const char delimit)
{
  int _begin = authReq.indexOf(param);
  if (_begin == -1)
  {
    return "";
  }
  return authReq.substring(_begin + param.length(), authReq.indexOf(delimit, _begin + param.length()));
}

String getCNonce(const int len)
{
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  String s = "";

  for (int i = 0; i < len; ++i)
  {
    s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return s;
}

String getDigestAuth(String &authReq, const String &username, const String &password, const String &uri, unsigned int counter)
{
  // extracting required parameters for RFC 2069 simpler Digest
  String realm = exractParam(authReq, "realm=\"", '"');
  String nonce = exractParam(authReq, "nonce=\"", '"');
  String cNonce = getCNonce(8);

  char nc[9];
  snprintf(nc, sizeof(nc), "%08x", counter);

  // parameters for the RFC 2617 newer Digest
  MD5Builder md5;
  md5.begin();
  md5.add(username + ":" + realm + ":" + password); // md5 of the user:realm:user
  md5.calculate();
  String h1 = md5.toString();

  md5.begin();
  md5.add(String("GET:") + uri);
  md5.calculate();
  String h2 = md5.toString();

  md5.begin();
  md5.add(h1 + ":" + nonce + ":" + String(nc) + ":" + cNonce + ":" + "auth" + ":" + h2);
  md5.calculate();
  String response = md5.toString();

  String authorization = "Digest username=\"" + username + "\", realm=\"" + realm + "\", nonce=\"" + nonce +
                         "\", uri=\"" + uri + "\", algorithm=\"MD5\", qop=auth, nc=" + String(nc) + ", cnonce=\"" + cNonce + "\", response=\"" + response + "\"";
  Serial.println(authorization);

  return authorization;
}
