import paho.mqtt.client as mqtt
import mysql.connector
import json

# --- Database Configuration ---
DB_CONFIG = {
    'host': 'localhost',
    'user': 'root',
    'password': '',  
    'database': 'p1',  
    'port': 3306
}

# --- MQTT Configuration ---
MQTT_BROKER = "test.mosquitto.org"
MQTT_PORT = 1883
MQTT_TOPIC = "weather/station"  # Match topic used in ESP8266

class MQTTSubscriber:
    def __init__(self):
        self.create_table()
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def create_table(self):
        try:
            conn = mysql.connector.connect(**DB_CONFIG)
            cursor = conn.cursor()
            query = '''
            CREATE TABLE IF NOT EXISTS weatherdata (
                temperature FLOAT,
                humidity FLOAT,
                rainfall FLOAT,
                irradiation FLOAT,
                timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
            )
            '''
            cursor.execute(query)
            conn.commit()
            conn.close()
            print("✅ Database table 'weatherdata' is ready.")
        except mysql.connector.Error as err:
            print(f"❌ Error creating table: {err}")
            exit()

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print("✅ Connected to MQTT Broker!")
            client.subscribe(MQTT_TOPIC)
        else:
            print(f"❌ Failed to connect with result code {rc}")

    def on_message(self, client, userdata, msg):
        print(f"\n📩 Message received from {msg.topic}")
        try:
            payload = msg.payload.decode()
            data = json.loads(payload)

            temperature = data.get('temperature')
            humidity = data.get('humidity')
            rainfall = data.get('rainfall')
            irradiation = data.get('irradiation')

            if None not in (temperature, humidity, rainfall, irradiation):
                self.store_data(temperature, humidity, rainfall, irradiation)
                print(f"🌡 Temp: {temperature}°C | 💧 Humidity: {humidity}% | ☔ Rainfall: {rainfall} mm | ☀️ Irradiation: {irradiation} W/m²")
            else:
                print("❗ Invalid or incomplete data.")

        except Exception as e:
            print(f"❌ Error processing message: {e}")

    def store_data(self, temperature, humidity, rainfall, irradiation):
        try:
            conn = mysql.connector.connect(**DB_CONFIG)
            cursor = conn.cursor()
            query = '''
                INSERT INTO weatherdata (temperature, humidity, rainfall, irradiation)
                VALUES (%s, %s, %s, %s)
            '''
            values = (temperature, humidity, rainfall, irradiation)
            cursor.execute(query, values)
            conn.commit()
            conn.close()
            print("✅ Data stored in DB:", values)
        except mysql.connector.Error as err:
            print(f"❌ Error storing data: {err}")

    def start(self):
        self.client.connect(MQTT_BROKER, MQTT_PORT, 60)
        self.client.loop_forever()


if __name__ == "__main__":
    subscriber = MQTTSubscriber()
    subscriber.start()
