#include <Arduino.h>

#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266mDNS.h>        // Include the mDNS library so you can ping on  http://esp8266.local

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

#include <PubSubClient.h>


const char* ssid = "losers"; // Rellena con el nombre de tu red WiFi
const char* password = "chippassfortest"; // Rellena con la contraseña de tu red WiFi
const char* ssid2 = "paclema_iphone"; // Rellena con el nombre de tu red WiFi
const char* password2 = "password"; // Rellena con la contraseña de tu red WiFi


const char* mqttServer = "m24.cloudmqtt.com";
const int mqttPort = 18638;
const char* mqttUser = "wfvlavqm";
const char* mqttPassword = "taR-AKV_V1NA";


WiFiClient espClient;
PubSubClient client(espClient);

int loop_count = 0;
long lastMsg = 0;
char msg[50];



void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}




void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {

    Serial.begin(115200);
    delay(100);

    // Conectamos a la red WiFi
    Serial.println();
    Serial.println();
    Serial.println("Connecting ...");

    /* Configuramos el ESP8266 como cliente WiFi. Si no lo hacemos
       se configurará como cliente y punto de acceso al mismo tiempo */
    WiFi.mode(WIFI_STA); // Modo cliente WiFi
    //WiFi.begin(ssid, password);

    // Esperamos a que estemos conectados a la red WiFi
    // while (WiFi.status() != WL_CONNECTED) {
    //   delay(500);
    //   Serial.print(".");
    // }
    //
    // Serial.println("");
    // Serial.println("WiFi connected");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP()); // Mostramos la IP



    wifiMulti.addAP(ssid, password);   // add Wi-Fi networks you want to connect to
    wifiMulti.addAP(ssid2, password2);

    int i = 0;
    while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
      delay(1000);
      Serial.print(++i); Serial.print(' ');
    }
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());              // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

    if (!MDNS.begin("esp8266")) {             // Start the mDNS responder for esp8266.local
      Serial.println("Error setting up MDNS responder!");
    }
    Serial.println("mDNS responder started");

    // Mqtt init:

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected()) {
      Serial.println("Connecting to MQTT...");
      if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
        Serial.println("connected");
      } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
      }
    }




    // client.publish("esp/test", "Hello from ESP8266");
    // client.subscribe("esp/test");




}

void loop() {



  // Consutar la memoria libre
  // Quedan un poco más de 40 kB
  //Serial.printf("\nMemoria libre en el ESP8266: %d Bytes\n\n",ESP.getFreeHeap());



  if (!client.connected()) {
      //reconnect();
      Serial.println("Reconnect!");
    }
    client.loop();

    long now = millis();
    if (now - lastMsg > 20) {
      lastMsg = now;
      ++loop_count;
      snprintf (msg, 50, "hello world #%ld", loop_count);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("outTopic", msg);
    }

}
