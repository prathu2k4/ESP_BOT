Here’s your updated README, reflecting the new **adaptive PID control** feature! 🚀  

---

# 🚗 Line Follower Bot using ESP32 with Adaptive PID Control  

## 📌 Overview  
This project is a **PID-controlled Line Follower Bot** powered by an **ESP32** microcontroller. It uses **infrared sensors (IR)** to detect the line and dynamically adjusts its movement using an **Adaptive PID Algorithm** for smoother and more accurate tracking. The ESP32 also hosts a **WiFi-enabled web interface** for real-time PID tuning, allowing you to fine-tune parameters directly from your browser.  

---

## 🔧 Features  
✔ **ESP32-based control** with real-time web interface  
✔ **Adaptive PID Algorithm** for auto-tuning on the go  
✔ **IR sensors** for precise line detection  
✔ **WiFi-enabled tuning** of PID parameters  
✔ **Web dashboard** to monitor and adjust settings dynamically  

---

## 🛠 Components Required  
- **ESP32 Development Board**  
- **L298N Motor Driver**  
- **Two DC Motors (with wheels)**  
- **5 IR Sensors (for line tracking)**  
- **12V Power Supply / Battery**  
- **Connecting Wires & Chassis**  

---

## 🔗 Circuit Diagram  
📝 *Connect the components as per the following pin mapping:*  

| **Component**      | **ESP32 Pin** |
|--------------------|--------------|
| IR Sensor 1        | GPIO 32      |
| IR Sensor 2        | GPIO 33      |
| IR Sensor 3        | GPIO 34      |
| IR Sensor 4        | GPIO 35      |
| IR Sensor 5        | GPIO 36      |
| Motor Driver ENA   | GPIO 14      |
| Motor Driver IN1   | GPIO 27      |
| Motor Driver IN2   | GPIO 26      |
| Motor Driver IN3   | GPIO 25      |
| Motor Driver IN4   | GPIO 33      |
| Motor Driver ENB   | GPIO 12      |
| Status LED         | GPIO 2       |

---

## 📝 Code Explanation  

### **ESP32 Code (C++)**  
📌 The ESP32 performs the following tasks:  
- Reads **IR sensor values** to determine the bot’s position relative to the line.  
- Uses an **Adaptive PID Algorithm** to dynamically adjust **Kp, Ki, and Kd** based on the error:  
  - Increases **Kp** when error is large for sharper corrections.  
  - Makes subtle adjustments to **Ki** for long-term correction.  
  - Increases **Kd** when oscillations are detected to stabilize movements.  
- Controls the motor driver using **PWM signals** for speed and direction.  
- Hosts a **web server** to fine-tune PID parameters in real-time over WiFi.  

### **Web Interface (HTML, CSS, JS)**  
📌 The Web Dashboard allows:  
- **Real-time monitoring** of connection status and PID values.  
- **Live adjustments** of **Kp, Ki, and Kd** parameters for better performance.  
- **Automatic updates** every 2 seconds to fetch the latest values.  

---

## 🌐 Web Dashboard  
🔹 The ESP32 hosts a **web-based PID tuner**, accessible from any browser.  
🔹 Default URL: `http://<ESP32_IP>/`  
🔹 Modify **Kp, Ki, and Kd** dynamically via simple input fields.  

---

## 🚀 Getting Started  

### 1️⃣ **Flash the ESP32 Code**  
- Install the **Arduino IDE** and ESP32 board support.  
- Replace `Your_SSID` and `Your_PASSWORD` in the code with your WiFi credentials.  
- Upload the sketch to ESP32.  

### 2️⃣ **Connect the Hardware**  
- Assemble the bot as per the circuit diagram.  
- Ensure motors, IR sensors, and ESP32 are properly connected.  

### 3️⃣ **Run the Bot**  
- Power on the system.  
- Open a browser and enter `http://<ESP32_IP>/` to access the Web Dashboard.  
- Fine-tune PID values for optimal line-following performance.  

---

## 📷 Demonstration  
*(Include images or videos of the bot in action!)*  

---

## 📌 Future Enhancements  
🔹 Add **Bluetooth control** as an alternative to WiFi.  
🔹 Implement an **OLED display** for real-time PID tuning without the web interface.  
🔹 Optimize the adaptive learning rate for different track complexities.  

---

## 📜 License  
This project is **open-source** and can be modified and distributed freely.  

---

🚀 **Happy Building!** 🎉  