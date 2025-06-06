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
MQTT_BROKER = "broker.emqx.io"
MQTT_PORT = 1883
MQTT_TOPIC = "esp32/waterflow"

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
            CREATE TABLE IF NOT EXISTS waterflowdata (
                flow_rate FLOAT,
                total_volume FLOAT,
                timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
            )
            '''
            cursor.execute(query)
            conn.commit()
            conn.close()
            print("✅ Database table ready.")
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

            flow_rate = data.get('flow_rate_Lmin')
            total_volume = data.get('total_millilitres')

            if flow_rate is not None and total_volume is not None:
                self.store_data(flow_rate, total_volume)
                print(f"📊 Flow Rate: {flow_rate} L/min | Total Volume: {total_volume} mL")
            else:
                print("❗ Invalid data format or missing keys.")

        except Exception as e:
            print(f"❌ Error processing message: {e}")

    def store_data(self, flow_rate, total_volume):
        try:
            conn = mysql.connector.connect(**DB_CONFIG)
            cursor = conn.cursor()
            query = '''
                INSERT INTO waterflowdata (flow_rate, total_volume)
                VALUES (%s, %s)
            '''
            values = (flow_rate, total_volume)
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
