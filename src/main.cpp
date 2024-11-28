#include <Arduino.h>
#include <WiFi.h>
#include <math.h>
#include <HttpClient.h>
#include <ArduinoJson.h>

#include "stepper.h"

const char *ssid = "NOS-E946";
const char *password = "9WX2HMRV";

Stepper stepperA(16, 17, 18);

void setup()
{
  Serial1.begin(115200);
  Serial1.println("Starting!");
  stepperA.init();
  while (stepperA.isBusy())
  {
    delay(10);
  }

  Serial1.print("Connecting to ");
  Serial1.print(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial1.print(".");
  }
  Serial1.println("");

  Serial1.print("WiFi connected: ");
  Serial1.println(WiFi.localIP());
}

void loop()
{
  WiFiClient wifiClient;

  auto addr = IPAddress(192, 168, 1, 101);
  HttpClient http(wifiClient, addr, 8484);

  int err = http.get("/getdevdata.cgi?device=2&sn=SZ00126022270436");
  if (err != 0)
  {
    Serial1.print("HTTP error: ");
    Serial1.println(err);
  }

  String response = http.responseBody();
  // Decode the json (we are on Arudino you dummy)
  JsonDocument doc;
  deserializeJson(doc, response);

  // Get the values
  int etd = doc["etd"];
  int pac = doc["pac"];

  Serial1.print("etd: ");
  Serial1.println(etd);
  Serial1.print("pac: ");
  Serial1.println(pac);

  delay(1000);
}