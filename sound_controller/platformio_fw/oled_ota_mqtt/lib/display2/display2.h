#ifndef display2_H
#define display2_H

#include <Arduino.h>

#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

class display2 {

  public:
    void init (SSD1306Wire *display){
      display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      display->setFont(ArialMT_Plain_10);
      display->drawString(display->getWidth()/2, display->getHeight()/2, "Ready for OTA:\n" + WiFi.localIP().toString());
      display->display();

    }

};

#endif
