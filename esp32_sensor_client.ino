#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// ============================================================================
// WIFI & MQTT CONFIGURATION
// ============================================================================
const char* ssid = "YOUR_WIFI_SSID";           // Thay đổi WiFi name
const char* password = "YOUR_WIFI_PASSWORD";   // Thay đổi WiFi password

// HiveMQ Cloud MQTT - HARD CODED SETTINGS
const char* mqtt_server = "5719ead78bbf4c12a926f2dfe8dcc0c3.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "ESP32";
const char* mqtt_password = "123456aB";

// Device Configuration
const String HOME_ID = "home_001";
const String DEVICE_TYPE = "sensor";  // có thể là "esp32", "sensor", "light", v.v.
const String DEVICE_ID = "living_room_01";  // ID unique cho device này
const String ROOM_ID = "living_room";

// ============================================================================
// SENSOR CONFIGURATION
// ============================================================================
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define LDR_PIN A0
#define MOTION_PIN 2

DHT dht(DHT_PIN, DHT_TYPE);

// ============================================================================
// MQTT & WIFI CLIENTS
// ============================================================================
WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

// ============================================================================
// DEVICE STATE
// ============================================================================
unsigned long lastSensorRead = 0;
unsigned long lastStatusSend = 0;
const unsigned long SENSOR_INTERVAL = 30000;  // Gửi sensor data mỗi 30 giây
const unsigned long STATUS_INTERVAL = 60000;  // Gửi status mỗi 60 giây

String deviceMAC;

// ============================================================================
// SETUP
// ============================================================================
void setup() {
  Serial.begin(115200);
  
  // Lấy MAC address làm device identifier
  deviceMAC = WiFi.macAddress();
  Serial.println("🔧 Device MAC: " + deviceMAC);
  
  // Initialize sensors
  dht.begin();
  pinMode(MOTION_PIN, INPUT);
  
  // Connect to WiFi
  connectToWiFi();
  
  // Setup MQTT
  setupMQTT();
  
  // Connect to MQTT
  connectToMQTT();
  
  Serial.println("✅ ESP32 Smart Home Sensor Ready!");
}

// ============================================================================
// MAIN LOOP
// ============================================================================
void loop() {
  // Maintain MQTT connection
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();
  
  unsigned long now = millis();
  
  // Send sensor data
  if (now - lastSensorRead > SENSOR_INTERVAL) {
    sendSensorData();
    lastSensorRead = now;
  }
  
  // Send device status
  if (now - lastStatusSend > STATUS_INTERVAL) {
    sendDeviceStatus();
    lastStatusSend = now;
  }
  
  delay(1000);
}

// ============================================================================
// WIFI CONNECTION
// ============================================================================
void connectToWiFi() {
  Serial.print("📶 Connecting to WiFi: " + String(ssid));
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("✅ WiFi Connected!");
  Serial.println("📡 IP Address: " + WiFi.localIP().toString());
}

// ============================================================================
// MQTT SETUP & CONNECTION
// ============================================================================
void setupMQTT() {
  // Configure SSL (HiveMQ yêu cầu SSL)
  wifiClient.setInsecure(); // Không verify certificate (cho demo)
  
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setKeepAlive(60);
}

void connectToMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("🌐 Connecting to MQTT broker...");
    
    String clientId = "ESP32_" + deviceMAC;
    clientId.replace(":", "");
    
    if (mqttClient.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println(" Connected!");
      
      // Subscribe to command topic cho device này
      String commandTopic = "smarthome/" + HOME_ID + "/commands/" + DEVICE_TYPE + "/" + DEVICE_ID;
      mqttClient.subscribe(commandTopic.c_str());
      Serial.println("📡 Subscribed to: " + commandTopic);
      
      // Gửi thông báo device online
      sendDeviceRegistration();
      
    } else {
      Serial.print(" Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

// ============================================================================
// MQTT CALLBACK (nhận commands)
// ============================================================================
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.println("📨 Received command: " + String(topic));
  Serial.println("💬 Message: " + message);
  
  // Parse JSON command
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, message);
  
  String command = doc["command"];
  JsonObject params = doc["params"];
  
  // Handle commands
  if (command == "calibrate") {
    calibrateSensors();
  } else if (command == "reset") {
    ESP.restart();
  } else if (command == "get_status") {
    sendDeviceStatus();
  }
}

// ============================================================================
// SEND SENSOR DATA
// ============================================================================
void sendSensorData() {
  Serial.println("📊 Reading sensors...");
  
  // Đọc cảm biến DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Đọc cảm biến ánh sáng
  int lightValue = analogRead(LDR_PIN);
  float lightPercent = map(lightValue, 0, 4095, 0, 100);
  
  // Đọc cảm biến chuyển động
  bool motionDetected = digitalRead(MOTION_PIN);
  
  // Kiểm tra lỗi DHT
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("❌ Failed to read DHT sensor!");
    return;
  }
  
  // Tạo JSON data theo format của hệ thống
  DynamicJsonDocument doc(1024);
  JsonObject data = doc.createNestedObject("data");
  
  data["temperature"] = temperature;
  data["humidity"] = humidity;
  data["light"] = lightPercent;
  data["motion"] = motionDetected;
  
  doc["device_id"] = DEVICE_ID;
  doc["device_type"] = DEVICE_TYPE;
  doc["mac_address"] = deviceMAC;
  doc["room_id"] = ROOM_ID;
  doc["timestamp"] = getTimestamp();
  
  // Serialize JSON
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Gửi lên MQTT topic
  String topic = "smarthome/" + HOME_ID + "/data/" + DEVICE_TYPE + "/" + DEVICE_ID;
  
  if (mqttClient.publish(topic.c_str(), jsonString.c_str())) {
    Serial.println("✅ Sensor data sent successfully!");
    Serial.println("🌡️  Temperature: " + String(temperature) + "°C");
    Serial.println("💧 Humidity: " + String(humidity) + "%");
    Serial.println("💡 Light: " + String(lightPercent) + "%");
    Serial.println("🚶 Motion: " + String(motionDetected ? "Yes" : "No"));
  } else {
    Serial.println("❌ Failed to send sensor data");
  }
}

// ============================================================================
// SEND DEVICE STATUS
// ============================================================================
void sendDeviceStatus() {
  Serial.println("🔋 Sending device status...");
  
  DynamicJsonDocument doc(1024);
  JsonObject status = doc.createNestedObject("status");
  
  status["online"] = true;
  status["wifi_signal"] = WiFi.RSSI();
  status["free_memory"] = ESP.getFreeHeap();
  status["uptime"] = millis() / 1000;
  status["ip_address"] = WiFi.localIP().toString();
  
  doc["device_id"] = DEVICE_ID;
  doc["device_type"] = DEVICE_TYPE;
  doc["mac_address"] = deviceMAC;
  doc["room"] = ROOM_ID;
  doc["timestamp"] = getTimestamp();
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  String topic = "smarthome/" + HOME_ID + "/status/" + DEVICE_TYPE + "/" + DEVICE_ID;
  
  if (mqttClient.publish(topic.c_str(), jsonString.c_str())) {
    Serial.println("✅ Device status sent!");
  } else {
    Serial.println("❌ Failed to send device status");
  }
}

// ============================================================================
// DEVICE REGISTRATION (khi kết nối lần đầu)
// ============================================================================
void sendDeviceRegistration() {
  Serial.println("📱 Registering device...");
  
  DynamicJsonDocument doc(1024);
  
  doc["device_id"] = DEVICE_ID;
  doc["device_type"] = DEVICE_TYPE;
  doc["mac_address"] = deviceMAC;
  doc["name"] = "ESP32 Sensor " + DEVICE_ID;
  doc["room_id"] = ROOM_ID;
  doc["manufacturer"] = "Espressif";
  doc["model"] = "ESP32";
  doc["ip_address"] = WiFi.localIP().toString();
  doc["timestamp"] = getTimestamp();
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  String topic = "smarthome/" + HOME_ID + "/discovery";
  
  if (mqttClient.publish(topic.c_str(), jsonString.c_str())) {
    Serial.println("✅ Device registered!");
  }
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================
String getTimestamp() {
  // Tạo timestamp đơn giản (có thể dùng NTP cho chính xác hơn)
  return String(millis());
}

void calibrateSensors() {
  Serial.println("🔧 Calibrating sensors...");
  
  // Reset và đọc lại cảm biến
  dht.begin();
  
  Serial.println("✅ Sensors calibrated!");
  
  // Gửi status update
  sendDeviceStatus();
}

// ============================================================================
// ADDITIONAL SENSOR FUNCTIONS (mở rộng)
// ============================================================================

// Gửi specific sensor reading
void sendTemperatureReading() {
  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    DynamicJsonDocument doc(512);
    doc["sensor_type"] = "temperature";
    doc["value"] = temp;
    doc["unit"] = "°C";
    doc["mac_address"] = deviceMAC;
    doc["timestamp"] = getTimestamp();
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    String topic = "smarthome/" + HOME_ID + "/data/sensor/temperature";
    mqttClient.publish(topic.c_str(), jsonString.c_str());
  }
}

void sendHumidityReading() {
  float humidity = dht.readHumidity();
  if (!isnan(humidity)) {
    DynamicJsonDocument doc(512);
    doc["sensor_type"] = "humidity";
    doc["value"] = humidity;
    doc["unit"] = "%";
    doc["mac_address"] = deviceMAC;
    doc["timestamp"] = getTimestamp();
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    String topic = "smarthome/" + HOME_ID + "/data/sensor/humidity";
    mqttClient.publish(topic.c_str(), jsonString.c_str());
  }
} 