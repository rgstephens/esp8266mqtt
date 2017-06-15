/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define PROG_NAME "esp8266mqtt, "
#define PROG_VERS "v0.70"

const char* ssid     = "nworks24";
const char* password = "golfbgc1";

#define MQTT_SERVER  "15.1.4.4"
#define MQTT_PORT    8883
#define MQTT_TOPICBASE "esp8266/"

// Sleep time - 1m = 60000, 5m = 300000, 15m = 900000
int delayMS = 15000;
// number of intervals to send IP address, 0 = never (only on startup)
int ipInterval = 0;
int intervalCount = 0;

String strIp = "";

// Use WiFiClient class to create TCP connections
WiFiClient client;
PubSubClient mqttClient(client);

void setup() {
  Serial.begin(115200);
  
  Serial.print(F("Started, "));
  Serial.print(PROG_NAME);
  Serial.println(PROG_VERS);

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
  Serial.println();

  strIp = IpAddress2String(WiFi.localIP());
  Serial.print("WiFi connected, IP: ");  
  Serial.println(WiFi.localIP());

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(callback);
}

void loop() {
  intervalCount++;
  mqttClient.loop();
  while (!mqttClient.connected()) {
    //Serial.println(">> mqtt connect <<");
    //Serial.println((char *)MQTT_TOPICBASE "name");
    mqttClient.connect((char *)MQTT_TOPICBASE "name");
    
    Serial.print("subscribe: ");
    Serial.println(MQTT_TOPICBASE "config/#");
    boolean status = mqttClient.subscribe(MQTT_TOPICBASE "config/#");  // Allow bootup config fetching using MQTT persist flag!
    Serial.println(status);    
  }
  mqttClient.loop();
  if (mqttClient.connected()) {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    Serial.print("mqtt send IP addr: ");
    Serial.println(WiFi.localIP());
    mqttClient.publish((char *)MQTT_TOPICBASE "ip", strIp.c_str());
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }
  mqttClient.loop();
  delay(delayMS);
}

void callback(char* topic, byte* payload, unsigned int length)
{
  //Serial.print("mqtt received topic: ");
  //Serial.println(topic);
  payload[length] = 0;    // Hack to be able to use this as a char string.

  if (strstr(topic, MQTT_TOPICBASE "config/"))
  {
    if (strstr(topic, "delay"))
      delayMS = atoi((const char *)payload);
      Serial.print("setting delay to ");
      Serial.println(delayMS);
  }
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}


