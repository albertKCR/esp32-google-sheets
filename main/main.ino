#include "WiFi.h"
#include "HTTPSRedirect.h"
#include "DHT.h"
#include <RTClib.h>
#include <Wire.h>

int totalPoints = 153;
String test[153];
int loadCounter = 0;
float volt;
float current;
float dif;

#define LDR 36 //luminosity sensor
#define DHTPIN 4 // data pin
#define DHTTYPE DHT22 // type of DHT (DHT11)

DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
HTTPSRedirect* client = nullptr;
int stringCounter = 0;

//URL to access the google sheet
const char* GScriptId   = "AKfycbwAc066yXAIMH84iAJHCmq6K1sNtSqQtBfGjoEJtb8XSTWOpXTj33y-lcckYLiEBnbq";
String payload_base =  "{\"command\": \"append_row\", \"sheet_name\": \"test\", \"values\": ";
String payload = "";
const char* host = "script.google.com";
const int httpsPort = 443;
String url = String("/macros/s/") + GScriptId + "/exec?cal";

//array that store the received values of current and voltage
String toSendData[40];

void setup() {
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  dht.begin();
  pinMode(LDR, INPUT);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  delay(100);
  WiFi.begin("", "");
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

  if (! rtc.begin()) {
    Serial.println("RTC NAO INICIALIZADO");
    while (1);
  }
  rtc.adjust(DateTime(2017, 8, 31, 14, 45, 14 ));

  for (int j = 0; j < 153; j++) {
    test[j] = String("132") + "," + String("133");
  }
}


void loop() {
  // --- HTTPS protocol ---
  static bool flag = false;
  if (!flag) {
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
  String url1 = String("/macros/s/") + "AKfycbzjP_akNXHgYh1XiYvLqgZGqUJwEuVOewOqv_7lsZ8ZCHHWhSSdXy2IsvtCrs0I_ME" + "/exec?cal";
  if (client->GET(url1, host)) { //Send the data through the google API
    Serial.println(" [OK]");
  }
  else {
    Serial.println("[Error]");
  }

  while (totalPoints > 0) {
    DateTime now = rtc.now();
    totalPoints = totalPoints - 40;
    if (totalPoints > 0) {

      // --- HTTPS protocol ---
      static bool flag = false;
      if (!flag) {
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

      stringCounter = 0;
      for (int i = 0; i < 40; i++) {
        toSendData[i] = test[i];
        loadCounter++;
        stringCounter++;
      }

      // the payload will be constructed according to the received data
      payload = payload_base + "\"" + toSendData[0];
      for (int i = 1; i < stringCounter; i++) {
        payload = payload + "," + toSendData[i];
      }
      payload = payload + "\"}";

      Serial.println("Sending...");

      if (client->POST(url, host, payload)) { //Send the data through the google API
        Serial.println(" [OK]");
      }
      else {
        Serial.println("[Error]");
      }
    }
    else {
      stringCounter = 0;
      Serial.print("totalpoints= ");
      Serial.println(totalPoints);
      for (int i = 0; i < 40 - abs(totalPoints); i++) {
        toSendData[i] = test[i];
        loadCounter++;
        stringCounter++;
      }
      // the payload will be constructed according to the received data
      payload = payload_base + "\"" + toSendData[0];
      for (int i = 1; i < stringCounter; i++) {
        payload = payload + "," + toSendData[i];
      }
      payload = payload + "\"}";

      Serial.println("Sending...");

      if (client->POST(url, host, payload)) { //Send the data through the google API
        Serial.println(" [OK]");
      }
      else {
        Serial.println("[Error]");
      }
    }
  }

  String payload_base =  "{\"command\": \"sensor\", \"sheet_name\": \"sensor\", \"values\": ";
  payload = payload_base + "\"" + dht.readHumidity() + "," + dht.readTemperature() + "," + analogRead(LDR) + "\"}";

  Serial.println("Sending...");
  if (client->POST(url, host, payload)) { //Send the data through the google API
    Serial.println(" [OK]");
  }
  else {
    Serial.println("[Error]");
  }
  totalPoints = 0;
  while (1) {
    // --- HTTPS protocol ---
    static bool flag = false;
    if (!flag) {
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
    String url2 = String("/macros/s/") + "AKfycbxxyoFAWe9FOx0jmXTF6jMJ0wp0D2woNQrWDkj4cs0_sVRMfqazh6Sq8jylQ3UaZRbJ" + "/exec?cal";
    if (client->GET(url2, host)) { //Send the data through the google API
      Serial.println(" [OK]");
    }
    else {
      Serial.println("[Error]");
    }
  }
}
