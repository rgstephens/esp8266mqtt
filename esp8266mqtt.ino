/*
 *  This sketch sends an MQTT message with the IP address of the esp8266 to the
 *  configured MQTT broker
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "nworks24";
const char* password = "golfbgc1";

#define MQTT_SERVER  "15.1.4.4"
#define MQTT_PORT    8883
#define MQTT_TOPICBASE "esp8266/"

String strIp = "";

// Use WiFiClient class to create TCP connections
WiFiClient client;
PubSubClient mqttClient(client);

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  strIp = IpAddress2String(WiFi.localIP());
  Serial.print("WiFi connected, IP: ");  
  Serial.println(WiFi.localIP());

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

int value = 0;

void loop() {
  delay(20000);
  ++value;

  while (!mqttClient.connected()) {
    Serial.print("mqtt connect as ");
    Serial.println((char *)MQTT_TOPICBASE "name");
    mqttClient.connect((char *)MQTT_TOPICBASE "name");
  }
  if (mqttClient.connected()) {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    Serial.print("mqtt send IP addr: ");
    Serial.println(WiFi.localIP());
    mqttClient.publish((char *)MQTT_TOPICBASE "ip", strIp.c_str());
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}


