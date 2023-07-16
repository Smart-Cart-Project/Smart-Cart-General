#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "certs.h"

#ifndef STASSID
#define STASSID "Mihail"
#define STAPSK "Misho123!"
#endif

ESP8266WiFiMulti WiFiMulti;
bool isGET;
bool isPOST;

void httpGET(HTTPClient& https, BearSSL::WiFiClientSecure* client, String path);
void httpsPOST(HTTPClient& https, BearSSL::WiFiClientSecure* client, String path);



void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(STASSID, STAPSK);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  pinMode(LED_BUILTIN, OUTPUT);

  isPOST = true;
  isGET = false;

  //Serial.println("setup() done connecting to ssid '" STASSID "'");
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);  //Turn the light on -> Indicates ready mode
  String input;

  if (Serial.available()) {
    digitalWrite(LED_BUILTIN, HIGH);  //Turn the light off -> Indicates scanned item
    input = Serial.readStringUntil('\n');
    //Serial.print("Read input is: ");
    //Serial.println(input);

    // wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

      client->setFingerprint(fingerprint_sni_cloudflaressl_com);
      // Or, if you happy to ignore the SSL certificate, then use the following line instead:
      client->setInsecure();

      HTTPClient https;

      //Serial.print("[HTTPS] begin...\n");

      if (isPOST) {
        String path = "/items?cartId=a9c3a&itemId=" + input;
        httpsPOST(https, client.get(), path);
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the ligh On/Off -> Indicates a request
      } else if (isGET) {
        httpsGET(https, client.get(), "/items?cartId=a9c3a");
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the ligh On/Off -> Indicates a request
      }
    }
    
    delay(500); //Wait for a half second before switching back to ready mode
  }
}

void httpsGET(HTTPClient& https, BearSSL::WiFiClientSecure* client, String path) {
  if (https.begin(*client, jigsaw_host, jigsaw_port, path)) {  // HTTPS

    //Serial.print("[HTTPS] GET...\n");
    // start connection and send HTTP header
    int httpCode = https.GET();
    
    
    /**
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    **/


    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
}

void httpsPOST(HTTPClient& https, BearSSL::WiFiClientSecure* client, String path) {
  if (https.begin(*client, jigsaw_host, jigsaw_port, path)) {  // HTTPS

    //Serial.print("[HTTPS] POST...\n");
    // start connection and send HTTP header
    int httpCode = https.POST("");


    /**
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been sent and the server response header has been handled
      Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

      // file found at the server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    **/


    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
}
