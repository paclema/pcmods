// WiFi includes
#include <ESP8266WiFi.h>

// OTA Includes
#include <ota.h>
#include <display2.h>

// MQTT
#include <PubSubClient.h>

// SPIFFS include
#include "FS.h"

// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`


// Wifi config
const char* ssid = ""; // Rellena con el nombre de tu red WiFi
const char* password = ""; // Rellena con la contraseña de tu red WiFi


// Initialize the OLED display using Wire library
SSD1306Wire display(0x3c, D4, D5);

// Object for my classes:
ota ota;
display2 dp2;

// MQTT connection:
const char* mqtt_broker_endpoint = ""; //MQTT broker ip
const char* clientName = "";
const char* mqtt_username = "";
const char* mqtt_password = "";

long connection_time = millis();

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char buff[length + 1];
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    buff[i] = (char)payload[i];
  }


  buff[length] = '\0';
  String message(buff);

  Serial.print(message);
  Serial.println();

  if (strcmp(topic, "/lamp") == 0) {
    //Lamp color request:

    if (message.equals("red")){
      Serial.println("Turning lamp to red");
      //colorWipe(strip.Color(255, 0, 0), 10);
    }
    else if (strcmp(buff, "blue") == 0){
        Serial.println("Turning lamp to blue");
        //colorWipe(strip.Color(0, 0, 255), 10);
    } else if (message.equals("green")){
        Serial.println("Turning lamp to green");
        //colorWipe(strip.Color(0, 255, 0), 10);
    }
    //client.publish((char*)"/lamp",(char*)"color changed");

  }

  Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  ///

}

WiFiClientSecure espClient;
PubSubClient client(mqtt_broker_endpoint, 8883, callback, espClient); //set  MQTT port number to 8883 as per //standard
long lastMsg = 0;
char msg[50];
int value = 0;


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("#");

      long time_now = millis() - connection_time;
      Serial.print("Time to setup and be connected: ");
      Serial.print(time_now/1000);
      Serial.println("s");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void print_heap_memory(){
  Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());
}

void setup() {
  // Init Serial:
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  delay(300);
  print_heap_memory();

  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  // Align text vertical/horizontal center
  // display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  // display.setFont(ArialMT_Plain_10);
  // display.drawString(display.getWidth()/2, display.getHeight()/2, "Ready for OTA:\n" + WiFi.localIP().toString());
  // display.display();

  dp2.print(&display, "Connecting...");
  delay(200);
  dp2.print_connecting(&display);
  // Init SPIFFS file storage:
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  // Print files stored:
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
      Serial.print(dir.fileName());
      Serial.print(":");
      if(dir.fileSize()) {
          File f = dir.openFile("r");
          Serial.println(f.size());
      }
  }
  // Load certificate file:
  // But you must convert it to .der
  // openssl x509 -in ./certs/IoLed_controller/client.crt -out ./certs/IoLed_controller/cert.der -outform DER
  File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt with your uploaded file name
  if (!cert) Serial.println("Failed to open cert file");
  else Serial.println("Success to open cert file");
  //delay(1000);
  if (espClient.loadCertificate(cert)) Serial.println("cert loaded");
  else Serial.println("cert not loaded");
  // Load private key:
  // But you must convert it to .der
  // openssl rsa -in ./certs/IoLed_controller/client.key -out ./certs/IoLed_controller/private.der -outform DER
  File private_key = SPIFFS.open("/private.der", "r");
  if (!private_key) Serial.println("Failed to open key file");
  else Serial.println("Success to open key file");
  //delay(1000);
  if (espClient.loadPrivateKey(private_key)) Serial.println("key loaded");
  else Serial.println("key not loaded");

  // Load CA file:
  File ca = SPIFFS.open("/ca.crt", "r");
  if (!ca) Serial.println("Failed to open CA file");
  else Serial.println("Success to open CA file");
  //delay(1000);
  if (espClient.loadCACert(ca)) Serial.println("CA loaded");
  else Serial.println("CA not loaded");


  // Print some info:
  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

  Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
  Serial.printf("Flash real size: %u bytes\n\n", realSize);

  Serial.printf("Flash ide  size: %u bytes\n", ideSize);
  Serial.printf("Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  if (ideSize != realSize) {
    Serial.println("Flash Chip configuration wrong!\n");
  } else {
    Serial.println("Flash Chip configuration ok.\n");
  }



  // Init wifi:
  WiFi.begin ( ssid, password );
  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 10 );
  }


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Init OTA manager after being connected
  ota.init(&display);
  dp2.init(&display);

}

void loop() {
  ota.handle();

  // Handle mqtt reconnection:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publish msg to mqtt
  // long now = millis();
  // if (now - lastMsg > 2000) {
  //   lastMsg = now;
  //   ++value;
  //   snprintf (msg, 75, "hello world #%ld", value);
  //   Serial.print("Publish message: ");
  //   Serial.println(msg);
  //   client.publish("outTopic", msg);
  //   Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  // }


}
