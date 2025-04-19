#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>
#include <time.h>  

const char* ssid = "Hiepwifi";
const char* password = "16112003";

#define FIREBASE_HOST "https://hiep-eaa75-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "AbrZbJoJ0PIp9XdLdcEArttXR2sRiIN2tEBnlLdX"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long previousMillis = 0;  
const long interval = 5000;  

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Đang kết nối WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected! IP: " + WiFi.localIP().toString());
}

void setupTime() {
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov"); 
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Đang đồng bộ thời gian...");
    delay(1000);
  }
  Serial.println("Đã đồng bộ thời gian!");
}

String parseToJson(String data, String timestamp) {
    DynamicJsonDocument doc(256);

    int tempStart = data.indexOf("Temp: ") + 6;
    int tempEnd = data.indexOf("C,");
    int humStart = data.indexOf("Hum: ") + 5;
    int humEnd = data.indexOf("%");

    if (tempStart == 5 || tempEnd == -1 || humStart == 4 || humEnd == -1) {
        Serial.println("Lỗi trích xuất dữ liệu! Định dạng không đúng.");
        return "{}";
    }

    doc["temperature"] = data.substring(tempStart, tempEnd).toFloat();
    doc["humidity"] = data.substring(humStart, humEnd).toFloat();
    doc["timestamp"] = timestamp;  

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

void sendDataToFirebase(String jsonData) {
    FirebaseJson json;
    json.setJsonData(jsonData);  
    
    String path = "/esp32/data";
    if (Firebase.pushJSON(fbdo, path.c_str(), json)) {
        Serial.println("Đã gửi dữ liệu lên Firebase!");
    } else {
        Serial.println("Lỗi Firebase: " + fbdo.errorReason());
    }
}

void setup() {
  Serial.begin(115200, SERIAL_8N1, 3, 1);  
  
  connectWiFi();
  setupTime(); 

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();


  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Lỗi lấy thời gian!");
      return;
    }
    char timeStr[20]; 
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
    String timestamp = String(timeStr);  
    Serial.println("Thời gian hiện tại: " + timestamp);
    if (Serial.available()) {
      String receivedData = Serial.readStringUntil('\n');
      receivedData.trim();
      Serial.println("Dữ liệu nhận từ UART: " + receivedData);
      String jsonData = parseToJson(receivedData, timestamp);  
      sendDataToFirebase(jsonData);  
    }
  }
}
