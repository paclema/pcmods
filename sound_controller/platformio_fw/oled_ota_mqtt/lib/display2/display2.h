#ifndef display2_H
#define display2_H

#include <Arduino.h>

#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

#define PROGMEM   ICACHE_RODATA_ATTR
#define ICACHE_RODATA_ATTR  __attribute__((section(".irom.text")))
class display2 {

  public:
    void init (SSD1306Wire *display){
      display->clear();
      display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display->setFont(ArialMT_Plain_10);
      display->drawString(display->getWidth()/2, display->getHeight()/2, "Ready for OTA:\n" + WiFi.localIP().toString());
      display->display();

    }

    void print (SSD1306Wire *display, char* msg){
      display->clear();
      display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display->setFont(ArialMT_Plain_10);
      display->drawString(display->getWidth()/2, display->getHeight()/2, msg);
      display->display();

    }

    void invertDisplay (SSD1306Wire *display){
      // Invert background and text color:
      display->invertDisplay();
      display->display();

    }

    void print_connecting (SSD1306Wire *display){

      display->clear();
      display->drawXbm(0, 0, 64 ,  64, myBitmap );
      display->display();


    }


};


#endif
