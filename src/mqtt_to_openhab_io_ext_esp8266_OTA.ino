
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "Adafruit_MCP23017.h"

char* ssid = "11111";
char* password = "11111111";
char mqtt_server[40] = "192.168.1.110";
char* mqtt_name = "cb";
int board_number = 0;
int number[] = {1, 2, 3, 4, 5, 6, 7, 8};
char* intopic = "/in/";
char* outtopic = "/out/";
String basetopic = "light_";
char* subtopic = "/in/#";

String intopic1 = intopic + basetopic + (number[0] + 8 * board_number);
String intopic2 = intopic + basetopic + (number[1] + 8 * board_number);
String intopic3 = intopic + basetopic + (number[2] + 8 * board_number);
String intopic4 = intopic + basetopic + (number[3] + 8 * board_number);
String intopic5 = intopic + basetopic + (number[4] + 8 * board_number);
String intopic6 = intopic + basetopic + (number[5] + 8 * board_number);
String intopic7 = intopic + basetopic + (number[6] + 8 * board_number);
String intopic8 = intopic + basetopic + (number[7] + 8 * board_number);

String souttopic1 = outtopic + basetopic + (number[0] + 8 * board_number);
String souttopic2 = outtopic + basetopic + (number[1] + 8 * board_number);
String souttopic3 = outtopic + basetopic + (number[2] + 8 * board_number);
String souttopic4 = outtopic + basetopic + (number[3] + 8 * board_number);
String souttopic5 = outtopic + basetopic + (number[4] + 8 * board_number);
String souttopic6 = outtopic + basetopic + (number[5] + 8 * board_number);
String souttopic7 = outtopic + basetopic + (number[6] + 8 * board_number);
String souttopic8 = outtopic + basetopic + (number[7] + 8 * board_number);

// як перетворити це на масив символів????
char* outtopic1;
char* outtopic2;
char* outtopic3;
char* outtopic4;
char* outtopic5;
char* outtopic6;
char* outtopic7;
char* outtopic8;

bool shouldSaveConfig = true;     //flag for saving data

Adafruit_MCP23017 mcp;

#define TRIGGER_PIN 0

#define light1_pin 0
#define light2_pin 1
#define light3_pin 2
#define light4_pin 3
#define light5_pin 4
#define light6_pin 5
#define light7_pin 6
#define light8_pin 7

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long keepalivetime = 0;
unsigned long MQTT_reconnect = 0;
unsigned long lastMqttSwitch = 0;
unsigned long lastMqttSensor = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  Serial.print(topic);
  Serial.print("  ");
  String strTopic = String(topic);
  String strPayload = String((char*)payload);
  Serial.println(strPayload);

  if (strTopic == intopic1) {
    if (strPayload == "OFF")     mcp.digitalWrite(light1_pin, LOW);
    else if (strPayload == "ON") mcp.digitalWrite(light1_pin, HIGH);
  }
  if (strTopic == intopic2) {
    if (strPayload == "OFF")     mcp.digitalWrite(light2_pin, LOW);
    else if (strPayload == "ON") mcp.digitalWrite(light2_pin, HIGH);
  }
  if (strTopic == intopic3) {
    if (strPayload == "OFF")     mcp.digitalWrite(light3_pin, LOW);
    else if (strPayload == "ON") mcp.digitalWrite(light3_pin, HIGH);
  }
  if (strTopic == intopic4) {
    if (strPayload == "OFF")     mcp.digitalWrite(light4_pin, LOW);
    else if (strPayload == "ON") mcp.digitalWrite(light4_pin, HIGH);
  }
  if (strTopic == intopic5) {
    if (strPayload == "OFF")     mcp.digitalWrite(light5_pin, LOW);
    else if (strPayload == "ON") mcp.digitalWrite(light5_pin, HIGH);
  }
  if (strTopic == intopic6) {
    if (strPayload == "OFF")     mcp.digitalWrite(light6_pin, LOW);
    else if (strPayload == "ON") mcp.digitalWrite(light6_pin, HIGH);
  }
  if (strTopic == intopic7) {
    if (strPayload == "OFF")     mcp.digitalWrite(light7_pin, LOW);
    else if (strPayload == "ON") mcp.digitalWrite(light7_pin, HIGH);
  }
  if (strTopic == intopic8) {
    if (strPayload == "OFF")     mcp.digitalWrite(light8_pin, LOW);
    else if (strPayload == "ON") mcp.digitalWrite(light8_pin, HIGH);
  }
}

void setup() {

  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  mcp.begin(0x00);      // use default address 0

  pinMode(TRIGGER_PIN, INPUT);

  mcp.pinMode(light1_pin, OUTPUT);
  mcp.digitalWrite(light1_pin, LOW);
  mcp.pinMode(light2_pin, OUTPUT);
  mcp.digitalWrite(light2_pin, LOW);
  mcp.pinMode(light3_pin, OUTPUT);
  mcp.digitalWrite(light3_pin, LOW);
  mcp.pinMode(light4_pin, OUTPUT);
  mcp.digitalWrite(light4_pin, LOW);
  mcp.pinMode(light5_pin, OUTPUT);
  mcp.digitalWrite(light5_pin, LOW);
  mcp.pinMode(light6_pin, OUTPUT);
  mcp.digitalWrite(light6_pin, LOW);
  mcp.pinMode(light7_pin, OUTPUT);
  mcp.digitalWrite(light7_pin, LOW);
  mcp.pinMode(light8_pin, OUTPUT);
  mcp.digitalWrite(light8_pin, LOW);

  if (client.connect(mqtt_name)) {
    client.publish("/out/lost_con_1", "ON");
    client.publish(outtopic1, "OFF");
    client.publish(outtopic2, "OFF");
    client.publish(outtopic3, "OFF");
    client.publish(outtopic4, "OFF");
    client.publish(outtopic5, "OFF");
    client.publish(outtopic6, "OFF");
    client.publish(outtopic7, "OFF");
    client.publish(outtopic8, "OFF");
    client.subscribe(subtopic);
    //ArduinoOTA.setPassword("123");
    ArduinoOTA.onStart([]() {
      Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
void reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_name)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("/in/#");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(50);
    }
  }
}

void loop() {

  ArduinoOTA.handle();

  souttopic1.toCharArray(outtopic1, 20);
  souttopic2.toCharArray(outtopic2, 20);
  souttopic3.toCharArray(outtopic3, 20);
  souttopic4.toCharArray(outtopic4, 20);
  souttopic5.toCharArray(outtopic5, 20);
  souttopic6.toCharArray(outtopic6, 20);
  souttopic7.toCharArray(outtopic7, 20);
  souttopic8.toCharArray(outtopic8, 20);


  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    WiFiManager wifiManager;
    wifiManager.startConfigPortal("OnDemandAP");
    WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
    wifiManager.addParameter(&custom_mqtt_server);
  }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  if (lastMqttSwitch > millis()) lastMqttSwitch = 0;

   if (millis() > (lastMqttSwitch + 60000)) {
    }
    if (client.connected()) {
      client.publish("/out/lost_con_1", "ON");
      if (mcp.digitalRead(light1_pin)) client.publish(outtopic1, "ON");
      else client.publish(outtopic1, "OFF");
      if (mcp.digitalRead(light2_pin)) client.publish(outtopic2, "ON");
      else client.publish(outtopic2, "OFF");
      if (mcp.digitalRead(light3_pin)) client.publish(outtopic3, "ON");
      else client.publish(outtopic3, "OFF");
      if (mcp.digitalRead(light4_pin)) client.publish(outtopic4, "ON");
      else client.publish(outtopic4, "OFF");
      if (mcp.digitalRead(light5_pin)) client.publish(outtopic5, "ON");
      else client.publish(outtopic5, "OFF");
      if (mcp.digitalRead(light6_pin)) client.publish(outtopic6, "ON");
      else client.publish(outtopic6, "OFF");
      if (mcp.digitalRead(light7_pin)) client.publish(outtopic7, "ON");
      else client.publish(outtopic7, "OFF");
     if (mcp.digitalRead(light8_pin)) client.publish(outtopic8, "ON");
     else client.publish(outtopic8, "OFF");
  }

  client.loop();

  lastMqttSwitch = millis();
}