#ifndef ota_H
#define ota_H

#include <Arduino.h>

#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
//#include "images.h"

// OTA Includes
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

class ota{

public:
     void init(SSD1306Wire *display) {
      ArduinoOTA.begin();
      ArduinoOTA.onStart([display]() {
        display->clear();
        display->setFont(ArialMT_Plain_10);
        display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
        display->drawString(display->getWidth()/2, display->getHeight()/2 - 10, "OTA Update");
        display->display();
      });

      ArduinoOTA.onProgress([display](unsigned int progress, unsigned int total) {
        display->clear();
        display->setFont(ArialMT_Plain_10);
        display->setTextAlignment(TEXT_ALIGN_CENTER);
        display->drawString(display->getWidth()/2, display->getHeight()/2 - 16, "OTA Update: " + String(progress / (total / 100)) + "%");
        display->drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
        display->display();
      });

      ArduinoOTA.onEnd([display]() {
        display->clear();
        //display->drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
        display->setFont(ArialMT_Plain_10);
        display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
        display->drawString(display->getWidth()/2, display->getHeight()/2-14, "Restarting...");
        display->display();
      });

    };


     void handle(void) {
      ArduinoOTA.handle();
    }

};

#endif
