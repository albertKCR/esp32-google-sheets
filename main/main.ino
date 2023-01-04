#include "WiFi.h"
#include "HTTPSRedirect.h"

HTTPSRedirect* client = nullptr;
int stringCounter = 0;

//URL to access the google sheet
const char* GScriptId   = "AKfycbxrtin5P-VncJmSKlx2dsphOA4bplVVLjcx_A4ZB8jCMshNo4t5QQuAGFWkii0A-NxY";
String payload_base =  "{\"command\": \"append_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";
const char* host = "script.google.com";
const int httpsPort = 443;
String url = String("/macros/s/") + GScriptId + "/exec?cal";

//array that store the received values of current and voltage
String toSendData[40];

void setup() {
  Serial.begin(115200);
  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  delay(100);
  WiFi.begin("ROQUE EDICULA", "01082002");
  Serial.println(WiFi.macAddress());

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // --- HTTPS Redirect Setup ---
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  Serial.print("Connecting to Google...");

  bool flag = false;
  for (int i = 0; i < 5; i++)
  {
    int retval = client->connect(host, httpsPort);
    if (retval == 1)
    {
      flag = true;
      Serial.println("[OK]");
      break;
    }
    else
      Serial.println("[Error]");
  }
  if (!flag)
  {
    Serial.print("[Error]");
    Serial.println(host);
    return;
  }
  delete client;
  client = nullptr;
  Serial.println("Setup done");
  // ---
}


void loop() {
  while (WiFi.status() == WL_CONNECTED){ //while have internet connection

    // --- HTTPS protocol ---
    static bool flag = false;
    if (!flag){
      client = new HTTPSRedirect(httpsPort);
      client->setInsecure();
      flag = true;
      client->setPrintResponseBody(true);
      client->setContentTypeHeader("application/json");
    }
    if (client != nullptr) {
      if (!client->connected()) {
        client->connect(host, httpsPort);
      }
    }
    else {
      Serial.println("[Error]");
    }
    // ---

    for (int i = 0; i < 40; i++){
      toSendData[i] = "112412, 01285";
    }

    // the payload will be constructed according to the amount of data
    payload = payload_base + "\"" + toSendData[0];
    for (int i = 1; i < 40; i++){
      payload = payload + "," + toSendData[i];
    }
    payload = payload + "\"}";
    
    Serial.println("Sending...");
  
    if (client->POST(url, host, payload)){ //Send the data through the google API
      Serial.println(" [OK]");
    }
    else {
      Serial.println("[Error]");
    }
  }
}
