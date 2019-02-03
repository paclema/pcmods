// WiFi includes
#include <ESP8266WiFi.h>

// OTA Includes
#include <ota.h>
#include <display2.h>

const char* ssid = "losers"; // Rellena con el nombre de tu red WiFi
const char* password = "chippassfortest"; // Rellena con la contraseña de tu red WiFi


// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

// Initialize the OLED display using Wire library
SSD1306Wire display(0x3c, D4, D5);

// Object for my classes:
ota ota;
display2 dp2;


void setup() {
  WiFi.begin ( ssid, password );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 10 );
  }

  Serial.begin(115200);
  delay(100);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  display.init();
  display.flipScreenVertically();
  display.setContrast(255);


  ota.init(&display);
  dp2.init(&display);
  // Align text vertical/horizontal center
  // display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  // display.setFont(ArialMT_Plain_10);
  // display.drawString(display.getWidth()/2, display.getHeight()/2, "Ready for OTA:\n" + WiFi.localIP().toString());
  // display.display();
}

void loop() {
  ota.handle();

}
