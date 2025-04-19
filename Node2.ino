#include "painlessMesh.h"

#define MESH_PREFIX     "MyMeshNetwork"
#define MESH_PASSWORD   "12345678"
#define MESH_PORT       5555
#define NODE_2_ID       1548937101 

painlessMesh mesh;

const uint32_t ALLOWED_SENDER_ID = 1548946881;
const uint32_t TARGET_RECEIVER_ID = 1548945969;

String bufferedMessage = "";
unsigned long previousMillis = 0;
const long interval = 5000;

void receivedCallback(uint32_t from, String &msg) {
    if (from == ALLOWED_SENDER_ID) {  
        Serial.printf("Nhận từ Node 1 (%u): %s\n", from, msg.c_str());
        bufferedMessage = msg;
    } else {
        Serial.printf("Node 2: Bỏ qua dữ liệu từ Node (%u)\n", from);
    }
}

void setup() {
    Serial.begin(115200);
    mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
    mesh.onReceive(&receivedCallback);
}

void loop() {
    mesh.update();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval && bufferedMessage != "") {
        previousMillis = currentMillis;
        if (mesh.sendSingle(TARGET_RECEIVER_ID, bufferedMessage)) {
            Serial.printf("Đã gửi tới Node 3 (%u): %s\n", TARGET_RECEIVER_ID, bufferedMessage.c_str());
            Serial.println("Node 2: Đã gửi thành công đến Node 3!");
        } else {
            Serial.println("Node 2: Lỗi gửi đến Node 3!");
        }
        bufferedMessage = "";
    }
}
