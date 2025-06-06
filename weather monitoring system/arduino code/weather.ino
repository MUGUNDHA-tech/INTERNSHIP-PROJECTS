#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <ArduinoJson.h>
#include <BH1750.h>
#include "DHT.h"
#include <time.h>
#include <PubSubClient.h>
// --- MQTT Broker Configuration ---
const char* mqtt_server = "test.mosquitto.org";  // You can use your broker IP/domain
const int mqtt_port = 1883;
const char* mqtt_topic = "weather/station";

WiFiClient espClient;
PubSubClient client(espClient);
// --- Wi-Fi Credentials ---
const char* ssid = "Airel_9842878776";
const char* password = "air88581";

// --- NTP Config ---
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;    // GMT +5:30
const int daylightOffset_sec = 0;

// --- DHT Sensor ---
#define DHTPIN D5   // GPIO14 on NodeMCU
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- BH1750 Light Sensor ---
BH1750 lightMeter;
#define CONVERSION_FACTOR_SUNLIGHT 39.8

// --- Rain Gauge Setup ---
const int rainGaugePin = D6;  // GPIO12
volatile int rainCounter = 0;
int lastResetHour = -1;
void connectToMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void IRAM_ATTR countRainfall() {
  rainCounter++;
}

float lux_to_irradiance(float lux, float conversion_factor) {
  return lux / conversion_factor;
}

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void checkAndResetRainCounter() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);

  if (!timeinfo) {
    Serial.println("Failed to obtain time");
    return;
  }

  if (timeinfo->tm_hour == 6 && lastResetHour != 6) {
    rainCounter = 0;
    lastResetHour = 6;
    Serial.println("Rain counter reset at 6 AM");
  } else if (timeinfo->tm_hour != 6) {
    lastResetHour = timeinfo->tm_hour;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  connectToWiFi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  client.setServer(mqtt_server, mqtt_port);  // MQTT setup
  connectToMQTT();

  dht.begin();
  pinMode(rainGaugePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rainGaugePin), countRainfall, FALLING);

  Wire.begin(D1, D2);
  lightMeter.begin();
}

void publishData(float temperature, float humidity, float rainfall, float irradiation) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["rainfall"] = rainfall;
  doc["irradiation"] = irradiation;

  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer);
  Serial.println(jsonBuffer);

  client.publish(mqtt_topic, jsonBuffer);
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  checkAndResetRainCounter();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float rainfall = rainCounter * 0.2;
  float lux = lightMeter.readLightLevel();
  float irradiance = lux_to_irradiance(lux, CONVERSION_FACTOR_SUNLIGHT);

  publishData(temperature, humidity, rainfall, irradiance);

  delay(5000); // Increased delay to avoid publishing too frequently
}