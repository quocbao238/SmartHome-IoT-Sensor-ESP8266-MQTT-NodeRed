#include "IO.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "OneButton.h"
#include "MQ2.h"




/* Constructor */
DHT dht11(DHT11_Sensor, DHT11);
DHT dht22(DHT22_Sensor, DHT22);

/* Client */
WiFiClient espClient;
PubSubClient client(espClient);