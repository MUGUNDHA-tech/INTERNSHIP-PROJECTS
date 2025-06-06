#include <WiFi.h>
#include <PubSubClient.h>

#define FLOW_SENSOR_PIN 32

const char* ssid = "Airel_9842878776";
const char* password = "air88581";

const char* mqtt_server = "broker.emqx.io"; 
const int mqtt_port = 1883;
const char* mqtt_topic = "esp32/waterflow";

WiFiClient espClient;
PubSubClient client(espClient);

volatile int pulseCount = 0;
float flowRate = 0;
float totalMilliLitres = 0;
unsigned long oldTime = 0;

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected.");
  Serial.println(WiFi.localIP());
}

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32WaterFlowClient")) {
      Serial.println("connected.");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);

  oldTime = millis();
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  unsigned long currentTime = millis();

  if (currentTime - oldTime > 1000) {
    detachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN));

    flowRate = ((1000.0 / (currentTime - oldTime)) * pulseCount) / 7.5;
    totalMilliLitres += (flowRate / 60.0) * 1000.0;

    oldTime = currentTime;
    pulseCount = 0;

    String payload = "{";
    payload += "\"flow_rate_Lmin\":" + String(flowRate, 2) + ",";
    payload += "\"total_millilitres\":" + String(totalMilliLitres, 2);
    payload += "}";

    Serial.println("Publishing to MQTT:");
    Serial.println(payload);

    client.publish(mqtt_topic, payload.c_str());

    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
  }
}
