#include "painlessMesh.h"

#define MESH_PREFIX     "MyMeshNetwork"
#define MESH_PASSWORD   "12345678"
#define MESH_PORT       5555

#define TXD_PIN 1  
#define RXD_PIN 3  
#define NODE_3_ID 1548945969

painlessMesh mesh;
unsigned long lastSendTime = 0;  
const unsigned long sendInterval = 5000;  

const uint32_t ALLOWED_NODE_ID = 1548937101;
String bufferedMessage = "";

void receivedCallback(uint32_t from, String &msg) {
    if (from == ALLOWED_NODE_ID) {  
        Serial.printf("Nhận từ Node 1: %s\n", msg.c_str());
        bufferedMessage = msg;
    } else {
        Serial.printf("Bỏ qua dữ liệu từ Node (%u)\n", from);
    }
}



void sendBufferedMessage() {
    if (millis() - lastSendTime >= sendInterval && bufferedMessage != "") {
        Serial.printf("Đã gửi qua UART: %s\n", bufferedMessage.c_str());
        Serial.print(bufferedMessage + "\n");  
        lastSendTime = millis();
        bufferedMessage = "";  
    }
}

void setup() {
    Serial.begin(115200, SERIAL_8N1, -1, TXD_PIN);  
    mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
    mesh.onReceive(&receivedCallback);
}

void loop() {
    mesh.update();
    sendBufferedMessage();
}
