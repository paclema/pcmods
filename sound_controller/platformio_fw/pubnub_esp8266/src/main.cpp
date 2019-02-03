#include <Arduino.h>

#include <ESP8266WiFi.h>
#define PubNub_BASE_CLIENT WiFiClient
#define PUBNUB_DEFINE_STRSPN_AND_STRNCASECMP
#define PUBNUB_DEBUG


#include <PubNub.h>


const char* ssid = "losers"; // Rellena con el nombre de tu red WiFi
const char* password = "chippassfortest"; // Rellena con la contraseña de tu red WiFi


const static char pubkey[]  = "pub-c-1aec47b3-2251-4f9c-b845-3b64e409d6ec";
const static char subkey[]  = "sub-c-50c79a80-26c5-11e9-af7c-c69eed1db94e";
const static char channel[] = "iphone_esp8266";

int loop_count = 0;

void setup() {

  Serial.begin(115200);
  delay(100);

  // Conectamos a la red WiFi

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Configuramos el ESP8266 como cliente WiFi. Si no lo hacemos
     se configurará como cliente y punto de acceso al mismo tiempo */
  WiFi.mode(WIFI_STA); // Modo cliente WiFi
  WiFi.begin(ssid, password);

  // Esperamos a que estemos conectados a la red WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Mostramos la IP



  PubNub.begin(pubkey, subkey,"ps.pndsn.com");
  Serial.println("PubNub set up");

}


bool StartMessageSent = false;

void loop() {

  //Send startup report, only once
  if (StartMessageSent == false)
  {
      if (WiFi.status() == WL_CONNECTED)
      {
          SendStartReport();
          StartMessageSent = true; //Do not try sending anymore
      }
  }

  // Creamos la msg para la petición

  Serial.print(loop_count);
  Serial.print(" - connecting to PubNub: ");

  char msg[50];
  sprintf(msg,"\"Hello world from esp: %d\"",loop_count);
  Serial.println(msg);

  // Creamos el cliente


//   WiFiClient* client = PubNub.publish(channel, msg);
//   if (0 == client) {
//       Serial.println("publishing error");
//       delay(1000);
//       return;
//   }
// /* Don't care about the outcome */
//   client->stop();




  // /* Publish message. You could use `auto` here... */
  // PubNonSubClient *pclient = PubNub.publish(channel, "{\"text\":\"Hello world from esp\"}");
  // if (!pclient) return;
  // PublishCracker cheez;
  // cheez.read_and_parse(pclient);

  // /** You're mostly interested in `outcome()`, and,
  //     if it's "failed", then `description()`.
  // */
  // Serial.print("Outcome: "); Serial.print(cheez.outcome());
  // Serial.print(' '); Serial.println(cheez.to_str(cheez.outcome()));
  // Serial.print("description: "); Serial.println(cheez.description());
  // Serial.print("timestamp: "); Serial.println(cheez.timestamp());
  // Serial.print("state: "); Serial.print(cheez.state());
  // Serial.print(' '); Serial.println(cheez.to_str(cheez.state()));
  // //pclient->stop();


  //"{\"text\" : \"Hello world from esp\"}"

  // PubNub_BASE_CLIENT *client = PubNub.publish(channel, "{\"text\" : \"Hello world from esp\"}");
  // if (client != 0) {
  //     client->stop();
  // }
  //
  // Serial.println("\t msg send!");


  PubNub_BASE_CLIENT *client;

  client = PubNub.publish(channel, "\"\\\"Hello world!\\\" from Arduino.\"");
    if (!client) {
        Serial.println("publishing error");
        delay(1000);
        return;
    }
    while (client->connected()) {
        while (client->connected() && !client->available());
        char c = client->read();
        Serial.print(c);
    }
    client->stop();
    Serial.println();
















  // Enviamos la petición

  //
  // while (client.available() == 0) {
  //   if (millis() - timeout > 5000) {
  //     Serial.println(">>> Superado el tiempo de espera !");
  //     client.stop();
  //     return;
  //   }
  // }

  // Consutar la memoria libre
  // Quedan un poco más de 40 kB
  Serial.printf("\nMemoria libre en el ESP8266: %d Bytes\n\n",ESP.getFreeHeap());


  Serial.println();
  Serial.println("Cerrando la conexión");

  delay(1000);
  loop_count++;
}
