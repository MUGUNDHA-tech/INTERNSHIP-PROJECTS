#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "Airel_9842878776";
const char* password = "air88581";

// MQTT Broker
const char* mqtt_server = "test.mosquitto.org";  // Or your own broker

WiFiClient espClient;
PubSubClient client(espClient);

// Sensor pins (change if needed)
const int irSensorPin = D5;   // GPIO5 (IR sensor - digital)
const int ldrPin = A0;        // A0 on ESP8266 (LDR - analog)

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
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

void setup() {
  Serial.begin(115200);
  pinMode(irSensorPin, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read sensor values
  int irValue = digitalRead(irSensorPin);
  int ldrValue = analogRead(ldrPin);


  // Print to Serial Monitor
  Serial.print("IR: ");
  Serial.print(irValue);
  Serial.print(" | LDR: ");
  Serial.println(ldrValue);

  // Create JSON-like payload
  String payload = "{\"ir\": \"" + irStatus + "\", \"ldr\": " + String(ldrValue) + "}";

  // Publish to MQTT topic
  client.publish("project/irldr", payload.c_str());

  delay(1000);
}
