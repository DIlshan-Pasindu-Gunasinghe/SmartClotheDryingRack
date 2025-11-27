# 🧺🤖 **Smart Clothing Rack System**

The **Smart Clothing Rack System** is an IoT-powered autonomous prototype developed as part of the **Higher National Diploma in Software Engineering (HNDE – IoT Module)** at the **National Institute of Business Management (NIBM), Sri Lanka**.

This system automates the outdoor clothes-drying process by detecting rainfall, navigating indoors, avoiding obstacles, and providing real-time environmental updates through an IoT dashboard.

---

## 🚀 **Project Overview**

Traditional outdoor clothes drying becomes inconvenient during unpredictable weather 🌧️. Sudden rainfall forces users to bring clothes inside manually — leading to wasted time and repeated work.

To solve this, we built a **Smart Clothing Rack System** that can:

- Detect rain automatically  
- Move indoors without human intervention  
- Follow a predefined line-guided path  
- Avoid obstacles using ultrasonic sensing  
- Display system status live through IoT  

This project integrates **embedded systems, automation, renewable energy, and IoT technologies** to create a modern smart-home solution.

---

## ⚙️ **System Features**

### 🔧 **Core Functionalities**
- 🌧️ **Automatic Rain Detection**  
  Rain sensor identifies rainfall and triggers automatic movement indoors.

- 🚶‍♂️ **Autonomous Navigation**  
  IR line-following sensor guides the rack safely along its path.

- 🛑 **Obstacle Detection & Auto Stop**  
  Ultrasonic sensors detect nearby objects and stop movement immediately.

- 🌡️ **Environmental Monitoring**  
  DHT11/DHT22 sensors measure temperature and humidity in real time.

- 🔋 **Solar-Powered Battery System**  
  Li-ion battery + solar panel + TP4056 ensure sustainable operation.

- 📡 **IoT Web Dashboard**  
  Displays rain status, humidity, temperature, battery percentage, and obstacle alerts using Firebase Realtime Database.

---

## 📊 **Performance Summary**

| Feature | Expected Behavior | Test Result |
|--------|-------------------|-------------|
| **Rain Detection** | Immediate indoor movement | ✅ Responsive |
| **Line Following** | Stable path following | ✅ Accurate |
| **Obstacle Detection** | Safe automatic stop | ✅ Functional |
| **IoT Monitoring** | Real-time updates | ✅ Working (Firebase) |
| **Power System** | Smooth, stable operation | ✅ Solar + battery stable |

---

## 💡 **Future Improvements**
- 📱 Mobile app integration  
- 🛰️ Advanced navigation (RFID, GPS, vision-based)  
- 🌧️ Better rain detection using capacitive sensors or weather API  
- ⚡ Automatic charging dock  
- 🎙️ Voice assistant support  
- 🛡️ Waterproof and durability enhancements  

---

## 🧰 **Tools and Technologies**

| Category | Tools / Components |
|-----------|-------------------|
| **Microcontroller** | NodeMCU ESP8266 |
| **Programming** | C++ (Arduino IDE) |
| **Sensors** | Rain Sensor, DHT11/DHT22, Ultrasonic Sensors, IR Line Sensor |
| **Actuators** | DC Geared Motors |
| **Motor Drivers** | L298N |
| **IoT Backend** | Firebase Realtime Database |
| **Dashboard** | HTML, CSS, JavaScript |
| **Power** | Li-ion Battery, Solar Panel, TP4056, Buck Converters |
| **Prototyping** | Breadboard, Jumper Wires |
| **Design Tools** | Google Gemini, Circuit Design Software |

---

## 🧩 **System Architecture**

- **NodeMCU ESP8266** controls all sensors, motors, and IoT connections.
- **Rain sensor** triggers return mode when rainfall is detected.
- **IR array** tracks the path using line-following signals.
- **Ultrasonic sensors** detect obstacles and ensure safe stopping.
- **L298N drivers** control the motors for movement.
- **Solar panel + battery system** powers the entire setup sustainably.
- **Firebase dashboard** provides real-time data monitoring.

---

## 🧠 **Programming Logic Overview**

- Initialize all sensors, motor drivers, and Wi-Fi connection at startup.  
- Continuously monitor rain sensor output; when rain is detected, switch to "return indoors" mode.  
- In return mode, enable line-following logic: read IR sensors and adjust motor control (left/right PWM) to stay on the path.  
- While moving, poll ultrasonic sensors; if an obstacle is detected within a safe threshold, stop motors and send an obstacle alert to the dashboard.  
- Periodically read DHT sensor values and battery voltage, then upload telemetry (temperature, humidity, rain status, battery level, position, and alerts) to Firebase for real-time dashboard visualization.  
- Implement safe power management routines to prevent deep discharge and handle charging from the solar input.

---

## 👨‍💻 **Team Members**

- ✨ **E. K. V. A. Piyaratne**  
- ✨ **H. L. D. A. Perera**  
- ✨ **I. Edirisinghe**  
- ✨ **K. D. P. Gunasinghe**

---

## 🎓 **Acknowledgment**

Supervised by **Mr. Bathiya Seneviratne**  
Lecturer – School of Computing, **National Institute of Business Management (NIBM), Sri Lanka**.

---

## 🧾 **License**

This project was created for educational purposes under the **NIBM HNDSE – IoT Module**.  
Feel free to explore, extend, and learn from it with proper credit.

---
