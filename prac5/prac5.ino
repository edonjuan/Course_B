#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define LDR 22          // CONFIGURE DONDE SE CONECTA LA LDR
#define LED_BOARD 2
/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "IngreseNombreDeRed"
#define WLAN_PASS       "IngreseContraseñaDeRed"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "IngreseUsuarioDeAdafruit"
#define AIO_KEY         "IngreseLlavedeAdafruit"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
Adafruit_MQTT_Publish luz = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/luz");
Adafruit_MQTT_Subscribe boton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/boton");

void MQTT_connect();

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(LED_BOARD, OUTPUT);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&boton);
}

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &boton) {
      Serial.print(F("Got: "));
      Serial.println((char *)boton.lastread);

      if (strcmp((char *)boton.lastread, "1") == 0) {
        digitalWrite(LED_BOARD, HIGH); // Enciende el LED
      } else if (strcmp((char *)boton.lastread, "0") == 0) {
        digitalWrite(LED_BOARD, LOW); // Apaga el LED
      }
    }
  }
  uint32_t ldr = analogRead(LDR);
  Serial.print(F("\nValor de foto-resistencia "));
  Serial.print(ldr);
  Serial.print("...");
  if (! luz.publish(ldr)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
