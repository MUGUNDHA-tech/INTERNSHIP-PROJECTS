import paho.mqtt.client as mqtt
import mysql.connector
import json

DB_CONFIG = {
    'host': 'localhost',
    'user': 'root',
    'password': '',  
    'database': 'p1',  
    'port': 3306
}

# --- MQTT Config ---
MQTT_BROKER = "broker.emqx.io"  
MQTT_PORT = 1883
MQTT_TOPIC = "quantanics/streetlight"


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
            CREATE TABLE IF NOT EXISTS sensor (
                ir_value int,
                ldr_value int,
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

            ir=data.get('ir_sensor')
            ldr=data.get('ldr_value')

            self.store_data(ir,ldr)
            print(f"📊 Data: IR sensor reading: {ir} LDR sensor reading: {ldr}")
        except Exception as e:
            print(f"❌ Error processing message: {e}")

    def store_data(self, ir,ldr):
        try:
            conn = mysql.connector.connect(**DB_CONFIG)
            cursor = conn.cursor()
            query = '''
                INSERT INTO sensor (ir_value,ldr_value)
                VALUES (%s, %s)
            '''
            values = (ir,ldr)
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
