#include <Arduino.h>
// Example: storing JSON configuration file in flash file system
//
// Uses ArduinoJson library by Benoit Blanchon.
// https://github.com/bblanchon/ArduinoJson
//
// Created Aug 10, 2015 by Ivan Grokhotkov.
//
// This example code is in the public domain.

#include <ArduinoJson.h>
#include "FS.h"

bool loadConfig() {
  File configFile = SPIFFS.open("/bitmaps.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }


  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, configFile);
  serializeJson(doc, Serial);
// if (error)
//   return;

  const char* serverName = doc["serverName"];
  const char* accessToken = doc["accessToken"];

  const char* wifi_logo = doc["wifi_logo"][0];

  // Real world application would store these values in some variables for
  // later use.

  Serial.print("Loaded serverName: ");
  Serial.println(serverName);
  Serial.print("Loaded accessToken: ");
  Serial.println(accessToken);
  Serial.print("wifi_logo: ");
  Serial.println(wifi_logo);
  return true;
}

bool saveConfig() {
  StaticJsonDocument<1000> doc;

  doc["serverName"] = "api.example.com";
  doc["accessToken"] = "128du9as8du12eoue8da98h123ueh9h98";

  File configFile = SPIFFS.open("/bitmaps.json", "a+");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  serializeJson(doc, configFile);
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("");
  delay(1000);
  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }


  // if (!saveConfig()) {
  //   Serial.println("Failed to save config");
  // } else {
  //   Serial.println("Config saved");
  // }

  if (!loadConfig()) {
    Serial.println("Failed to load config");
  } else {
    Serial.println("Config loaded");
  }
}

void loop() {
}
