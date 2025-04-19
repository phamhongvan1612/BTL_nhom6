#include "painlessMesh.h"
#include "DHT.h"

#define MESH_PREFIX     "MyMeshNetwork"
#define MESH_PASSWORD   "12345678"
#define MESH_PORT       5555
#define DHTPIN          4  
#define DHTTYPE         DHT11
#define NODE_1_ID       1548946881
#define NODE_2_ID       1548937101  

DHT dht(DHTPIN, DHTTYPE);
painlessMesh mesh;

unsigned long previousMillis = 0;
const long interval = 5000;

void receivedCallback(uint32_t from, String &msg) {
    if (from == NODE_2_ID) {
        Serial.printf("Nhận phản hồi từ node 2 (%u): %s\n", from, msg.c_str());
    }
}

void setup() {
    Serial.begin(115200);
    dht.begin();
    mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
    mesh.onReceive(&receivedCallback);
}

void loop() {
    mesh.update();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        float temp = dht.readTemperature();
        float hum = dht.readHumidity();
        if (!isnan(temp) && !isnan(hum)) {
            String msg = "Temp: " + String(temp) + "C, Hum: " + String(hum) + "%";
            if (mesh.sendSingle(NODE_2_ID, msg)) {
                Serial.printf("Đã gửi tới node 2 (%u): %s\n", NODE_2_ID, msg.c_str());
            } else {
                Serial.println("Gửi thất bại!");
            }
        } else {
            Serial.println("Lỗi đọc cảm biến!");
        }
    }
}
