# Hệ Thống Nhà Thông Minh Sử Dụng Công Nghệ WiFi Mesh

> **Mô tả ngắn gọn:**  
Dự án thiết kế và triển khai một hệ thống nhà thông minh giám sát nhiệt độ, độ ẩm sử dụng mạng WiFi Mesh giữa các ESP32. Ba node cảm biến thu thập dữ liệu từ DHT11, truyền về node trung tâm (node 4) qua UART. Node 4 đẩy dữ liệu lên Firebase để hiển thị trên giao diện web. Hệ thống ứng dụng cho mục đích giáo dục, học tập và nghiên cứu phát triển các mô hình IoT phân tán.

---

## 📑 Mục Lục

- [Giới thiệu](#giới-thiệu)  
- [Tổng quan về công nghệ và thư viện được dùng](#tổng-quan-về-công-nghệ-và-thư-viện-được-dùng)  
- [Danh sách linh kiện](#danh-sách-linh-kiện)  
- [Sơ đồ nguyên lý và PCB](#sơ-đồ-nguyên-lý-và-pcb)  
- [Hướng dẫn lắp ráp](#hướng-dẫn-lắp-ráp)  
- [Lập trình firmware](#lập-trình-firmware)  
- [Tài liệu chi tiết](#tài-liệu-chi-tiết)  
- [DEMO](#demo)   

---

## 👋 Giới Thiệu

- **Dự án làm gì?**  
  Thu thập dữ liệu nhiệt độ và độ ẩm tại nhiều vị trí trong nhà bằng các cảm biến DHT11 và truyền về một node trung tâm qua mạng WiFi Mesh. Node trung tâm sau đó chuyển dữ liệu qua UART đến một node gateway, nơi dữ liệu được đẩy lên Firebase và hiển thị trên giao diện web.

- **Ai là người dùng chính?**  
  Sinh viên, giảng viên, nhà nghiên cứu trong lĩnh vực IoT, kỹ thuật điều khiển – tự động hóa, hoặc người học STEM.

- **Mục tiêu của thiết kế:**  
  Phục vụ mục đích học tập và nghiên cứu mô hình truyền thông không dây phân tán, xử lý dữ liệu cảm biến môi trường, lập trình nhúng ESP32 và sử dụng các nền tảng cloud như Firebase.

---

## 📐 Tổng quan về công nghệ và thư viện được dùng
1.ESP32:
ESP32 là một vi điều khiển đa năng với các tính năng mạnh mẽ như Wi-Fi và Bluetooth tích hợp sẵn. Đây là sự lựa chọn phổ biến cho các dự án IoT nhờ vào khả năng kết nối không dây và tính linh hoạt cao.
Trong dự án này, ESP32 đóng vai trò là bộ vi điều khiển chính, thực hiện nhiệm vụ thu thập dữ liệu từ cảm biến DHT11, kết nối các node trong mạng Wi-Fi Mesh, và gửi dữ liệu tới Firebase.

2.Wi-Fi Mesh:
Wi-Fi Mesh là một cấu trúc mạng không dây phân tán, trong đó các thiết bị (node) có thể kết nối với nhau và tạo thành một mạng lưới mạnh mẽ, có thể mở rộng mà không cần cấu hình phức tạp.
Mạng Wi-Fi Mesh trong dự án này giúp các node (Node 1, 2, 3) có thể truyền tải dữ liệu qua lại mà không gặp phải giới hạn khoảng cách như các mạng Wi-Fi truyền thống.

3.UART (Universal Asynchronous Receiver-Transmitter):
UART là giao thức truyền thông đơn giản và phổ biến giữa các vi điều khiển. Trong dự án này, UART được sử dụng để truyền tải dữ liệu giữa Node 3 và Node 4, với Node 4 nhận dữ liệu từ Node 3 và gửi lên Firebase.

4.Firebase:
Firebase là nền tảng phát triển ứng dụng di động và web của Google, cung cấp các dịch vụ như cơ sở dữ liệu thời gian thực, xác thực người dùng, và lưu trữ tệp.
Trong dự án này, Firebase được sử dụng để lưu trữ dữ liệu cảm biến từ các node và cung cấp một giao diện web để người dùng có thể xem dữ liệu cảm biến trong thời gian thực.

---

## 🧰 Danh Sách Linh Kiện

| Tên linh kiện        | Số lượng | Ghi chú                                 |
|----------------------|----------|-----------------------------------------|
| ESP32 DevKit v1      | 4        | Node cảm biến (3) + Node gateway (1)    |
| Cảm biến DHT11       | 1        | Gắn vào node1 để thu thập dữ liệu       |
| Dây nối TX/RX        | -        | Kết nối giữa node 3 và node 4           |
| Nguồn 5V             | 4        | Cấp nguồn cho ESP32                     |
        

---

## 🔧 Sơ Đồ Nguyên Lý và PCB

_Hình minh họa sơ đồ mạch:_

![Schematic](BTL_nhom6/Images/Sch.png)
![PCB](BTL_nhom6/Images/Pcb.png)
![Mạch thực tế](BTL_nhom6/Images/Mạch.png)

---

## 🔩 Hướng Dẫn Lắp Ráp

1. Kết nối DHT11 vào các chân của ESP32 theo sơ đồ:  
   - VCC → 3.3V  
   - GND → GND  
   - DATA → GPIO4

2. Cấp nguồn cho từng node cảm biến.  
3. Node 3 và Node 4 nối với nhau bằng UART (TX → RX, RX → TX, chung GND).  
4. Kiểm tra dòng tiêu thụ từng node.  
5. Nạp firmware thử từng node riêng biệt.

---

## 💻 Lập Trình Firmware

- **Ngôn ngữ:** C (Arduino)
- **Firmware:**
  - `firmware/node_sensor/` – code cho node cảm biến (WiFi Mesh, DHT11)
  - `firmware/node_gateway/` – code cho node gateway (UART, Firebase)

### **Cách nạp firmware cho các node (dùng Arduino IDE):**

1. **Cài đặt ESP32 vào Arduino IDE**:
   - Mở **Arduino IDE**, vào **File** -> **Preferences**.
   - Trong phần **Additional Boards Manager URLs**, thêm URL sau:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Sau đó, vào **Tools** -> **Boards** -> **Boards Manager**, tìm kiếm **ESP32** và cài đặt.

2. **Chọn Board và Cổng COM**:
   - Chọn **Tools** -> **Board** và chọn **DOIT ESP32 DEVKIT V1**.
   - Chọn **Tools** -> **Port** và chọn cổng **COM** tương ứng với board của bạn.

3. **Nạp Firmware cho các Node**:
   - **Node 1 (Cảm biến DHT11, WiFi Mesh)**:
     1. Mở code cho **Node 1** từ thư mục `firmware/node_sensor/`.
     2. Chọn đúng board và cổng COM.
     3. Nhấn **Upload** trong Arduino IDE để nạp code vào Node 1.
   
   - **Node 2 và Node 3 (Nhận và chuyển tiếp dữ liệu)**:
     1. Mở code cho **Node 2** hoặc **Node 3** từ thư mục `firmware/node_sensor/`.
     2. Chọn đúng board và cổng COM.
     3. Nhấn **Upload** trong Arduino IDE để nạp code vào Node 2 hoặc Node 3.

   - **Node 4 (Nhận dữ liệu từ Node 3 qua UART và gửi lên Firebase)**:
     1. Mở code cho **Node 4** từ thư mục `firmware/node_gateway/`.
     2. Chọn đúng board và cổng COM.
     3. Nhấn **Upload** trong Arduino IDE để nạp code vào Node 4.
## Tài liệu chi tiết
## DEMO
