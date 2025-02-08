# Smart Wearable Device for Women's Safety

## 📌 Project Overview
This project is a **smart wearable device** designed for women's safety using the **ESP8266**, a **GPS module**, and an **MPU6050 accelerometer**. The device detects sudden or unusual hand movements that indicate distress and automatically sends an alert email containing the **real-time location** of the user.

## 🚀 Features
- **Real-time GPS Tracking:** Captures and sends the user's current location.
- **Motion Detection:** Uses an **MPU6050 accelerometer** to detect abnormal hand movements.
- **Automatic Alerts:** Sends an emergency email with GPS coordinates when a distress movement is detected.
- **Wireless Communication:** Uses the **ESP8266 Wi-Fi module** to connect to the internet for sending alerts.

## 🛠️ Technologies & Components Used
- **ESP8266** (NodeMCU) – Microcontroller for Wi-Fi communication.
- **MPU6050 Accelerometer & Gyroscope** – Detects movement and orientation.
- **GPS Module (Neo-6M)** – Provides real-time location tracking.
- **SMTP Server** – Sends alert emails.
- **Arduino IDE** – Programming and uploading the firmware.
- **C/C++** – Programming language for the microcontroller.

## 🔧 How It Works
1. The **MPU6050 sensor** continuously monitors hand movements.
2. If a movement surpasses the predefined danger **threshold**, the system triggers an alert.
3. The **GPS module** retrieves the user’s real-time location.
4. The ESP8266 sends an **email alert** containing GPS coordinates to a predefined emergency contact.

## 📑 Installation & Setup
1. **Clone the Repository:**
   ```sh
   git clone https://github.com/amansh-tty/Stree-Raksha-Women-s-Safety-IOT-Device-.git
   cd Stree-Raksha-Women-s-Safety-IOT-Device-
   ```
2. **Install Required Libraries in Arduino IDE:**
   - `Wire.h` (for I2C communication with MPU6050)
   - `TinyGPS++` (for GPS data parsing)
   - `ESP8266WiFi.h` (for Wi-Fi communication)
   - `SMTPClient.h` (for sending emails)
3. **Upload the Code to ESP8266:**
   - Open the `.ino` file in Arduino IDE.
   - Connect the ESP8266 via USB.
   - Configure **Wi-Fi credentials** and **SMTP email settings** in the code.
   - Upload and run the program.

## 📊 Circuit Diagram
*Connect the components as follows:*
- **MPU6050 (I2C Protocol):**
  - VCC → 3.3V (ESP8266)
  - GND → GND
  - SDA → D2 (ESP8266)
  - SCL → D1 (ESP8266)
- **GPS Module (UART Communication):**
  - VCC → 3.3V
  - GND → GND
  - TX → D7 (ESP8266 RX)
  - RX → D8 (ESP8266 TX)

## 📬 Email Alert Example
When a distress motion is detected, an email is sent with the following content:
```
Subject: Emergency Alert - User in Distress!
Message: User's location: https://www.google.com/maps?q=28.7041,77.1025
```

## 🔮 Future Enhancements
- **SOS Button:** Manual distress trigger.
- **Mobile App Integration:** Real-time location tracking.
- **Machine Learning Model:** Improve motion detection accuracy.

## 🤝 Contributing
Feel free to fork this project and enhance it! Contributions are welcome via pull requests.

## 📜 License
This project is licensed under the **MIT License**.

---
✨ *Developed with passion for women's safety!* ✨

