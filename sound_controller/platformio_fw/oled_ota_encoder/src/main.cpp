// WiFi includes
#include <ESP8266WiFi.h>

// OTA Includes
#include <ota.h>
#include <display2.h>

const char* ssid = "losers"; // Rellena con el nombre de tu red WiFi
const char* password = "chippassfortest"; // Rellena con la contraseña de tu red WiFi

#include <Encoder.h>


// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "images.h"


// Initialize the OLED display using Wire library
SSD1306Wire display(0x3c, D4, D5);


// Object for my classes:
ota ota;
display2 dp2;


// Menus:
#define DEMO_DURATION 3000
typedef void (*Demo)(void);
int demoMode = 0;
int counter = 1;
int newMode = 0;





// Encoder vars:
Encoder myEnc(D1, D2);
long oldPosition  = -999;
boolean isButtonPressed = false;
long lastUpdateMillis = 0;









void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Hello world");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "Hello world");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, "Hello world");
}

void drawTextFlowDemo() {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawStringMaxWidth(0, 0, 128,
      "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
}

void drawTextAlignmentDemo() {
    // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}

void drawRectDemo() {
      // Draw a pixel at given position
    for (int i = 0; i < 10; i++) {
      display.setPixel(i, i);
      display.setPixel(10 - i, i);
    }
    display.drawRect(12, 12, 20, 20);

    // Fill the rectangle
    display.fillRect(14, 14, 17, 17);

    // Draw a line horizontally
    display.drawHorizontalLine(0, 40, 20);

    // Draw a line horizontally
    display.drawVerticalLine(40, 0, 20);
}

void drawCircleDemo() {
  for (int i=1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(32, 32, i*3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(96, 32, 32 - i* 3);
  }
}

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");

}

void drawImageDemo() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}










Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;





void handleKey() {
  isButtonPressed = true;
  //myEnc.write(0);
  //newMode = 0;
  Serial.println("Pressed!");
}


void setup() {

  Serial.begin(115200);
  delay(100);


  WiFi.begin ( ssid, password );
  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 10 );
  }


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setContrast(10);
  display.setFont(ArialMT_Plain_10);

  ota.init(&display);
  dp2.init(&display);


  //Configure encoder:

  pinMode(D7, INPUT_PULLUP);
  attachInterrupt(D7, handleKey, RISING);

}

void loop() {
  ota.handle();

  // Read encoders:
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
        oldPosition = newPosition;

        if (newPosition >= 40){
          newPosition = 40;
          myEnc.write(40);
        }
        if (newPosition <= 0){
          newPosition = 0;
          myEnc.write(0);
        }

        Serial.printf("\nMemoria libre en el ESP8266: %d Bytes\n\n",ESP.getFreeHeap());
        Serial.println(newPosition);

      // software debounce
      if (isButtonPressed && millis() - lastUpdateMillis > 50) {
        isButtonPressed = false;
        lastUpdateMillis = millis();
        // Reset the counter
        myEnc.write(0);
      }

      // clear the display
      display.clear();
      // draw the current demo method
      demos[demoMode]();

      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      display.drawString(10, 128, String(millis()));
      // write the buffer to the display
      display.display();

      // if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
      //   demoMode = (demoMode + 1)  % demoLength;
      //   timeSinceLastModeSwitch = millis();
      // }

      if (newPosition%4 == 0) {
        //demoMode = (demoMode + 1)  % demoLength;
        newMode = (newPosition/4);
        demoMode = newMode % demoLength;
        timeSinceLastModeSwitch = millis();
      }




  }
  counter++;
  delay(10);


}
