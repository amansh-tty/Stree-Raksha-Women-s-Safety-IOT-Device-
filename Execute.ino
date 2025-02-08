#include <Wire.h>
#include <TinyGPS++.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>


#define WIFI_SSID "Redmi 9"
#define WIFI_PASSWORD "zccu5rea8n"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "amlendu786.562@gmail.com"
#define AUTHOR_PASSWORD "bvzn riaq ykvo rsrj"
#define RECIPIENT_EMAIL "amlendu786@gmail.com"

// Accelerometer and GPS setup
MPU6050 mpu(0x68);
TinyGPSPlus gps;
HardwareSerial neogps(1);
#define RXD2 16
#define TXD2 17
const char* recipientEmails[] = {
  "amlendu786@gmail.com",
  "amanshetty300@gmail.com",
  "akshay220404@gmail.com",
  "rohitgajre456@gmail.com"
};



void addRecipients(SMTP_Message& message) {
  for (const char* email : recipientEmails) {
    message.addRecipient("Recipient", email);
  }
}
// Email session
SMTPSession smtp;

// Threshold for detecting sudden jerk 
float prev_ax = 0, prev_ay = 0, prev_az = 0;
int jerkCount = 0;  // Counter for jerks  
const float threshold = 1;   // Reset count if no jerk within 2 seconds
const int jerkThreshold=3;
bool gpsValueUpdated=true;
float lat_val, lng_val;
bool jerkDetected = false;

// Temporary storage for GPS and accelerometer data
String gpsData="Jerk Detected! Latitude: 12.9078360, Longitude: 77.5662140, Time (IST): 10:27:04", accelData;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  Wire.begin(21, 22);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Initialize MPU6050
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed. Check wiring!");
    while (1);
  }
  Serial.println("MPU6050 Initialized");

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Set up email
  smtp.debug(1);
}

void loop() {
  Serial.println("Starting loop...");

  if(!jerkDetected){
  checkAccelerometer();

  }

  if(gpsValueUpdated){
  updateGPSData();
  }

  if (jerkDetected) {
    Serial.println("Jerk detected! Preparing to send email...");
    sendEmailWithGPSData();
    gpsValueUpdated=true;
  }

  delay(1000);  // Adjust delay as needed
}



void checkAccelerometer() {
 int16_t ax, ay, az;  // Acceleration values (in raw units)

  // Get acceleration data
  mpu.getAcceleration(&ax, &ay, &az);

  // Convert acceleration from raw values to 'g' (gravitational units)
  float ax_g = ax / 16384.0;  // Scale factor for 2g range
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  // Detect sudden jerk based on a threshold for change in acceleration
  if (abs(ax_g - prev_ax) > threshold || abs(ay_g - prev_ay) > threshold || abs(az_g - prev_az) > threshold) {
    jerkCount++;
    Serial.println("Jerk detected!");
  }

  // Check if the number of jerks reaches the threshold
  if (jerkCount >= jerkThreshold) {
    Serial.println("Three jerks detected!");
    jerkCount = 0;  // Reset the counter
    jerkDetected=true;
  }

  // Update previous values
  prev_ax = ax_g;
  prev_ay = ay_g;
  prev_az = az_g;

  // Output the data to Serial Monitor
  Serial.print("Accel X: "); Serial.print(ax_g, 4); Serial.print(" g ");
  Serial.print("Accel Y: "); Serial.print(ay_g, 4); Serial.print(" g ");
  Serial.print("Accel Z: "); Serial.print(az_g, 4); Serial.print(" g ");
  Serial.println();

  delay(500);
}


void updateGPSData() {
  Serial.println("Recording GPS Data");
  while (neogps.available() > 0) {
    if (gps.encode(neogps.read())) {
      if (gps.location.isValid()) {
    lat_val = gps.location.lat();
    lng_val = gps.location.lng();

    // Get UTC time from GPS
    int hour = gps.time.hour();   // UTC Hour
    int minute = gps.time.minute();
    int second = gps.time.second();

    // Adjust to Indian Standard Time (UTC + 5:30)
    hour = (hour + 5) % 24; // Add 5 hours
    minute = minute + 30;   // Add 30 minutes
    if (minute >= 60) {     // Handle minute overflow
        minute -= 60;
        hour = (hour + 1) % 24;
    }

    // Format time as HH:MM:SS
    String timeIST = String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second);

    gpsData = "Latitude: " + String(lat_val, 6) + ", Longitude: " + String(lng_val, 6) + ", Time (IST): " + timeIST;
    gpsValueUpdated = false;

    Serial.println("GPS Data: " + gpsData);
}
    }
  }

  

}

void sendEmailWithGPSData() {
  if (gpsData.length()==0 ) {
    Serial.println("Insufficient data to send email.");
    return;
  }

  SMTP_Message message;
  message.sender.name = "ESP";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Emergency Alert: Sudden Jerk Detected!";
   for (const char* email : recipientEmails) {
    message.addRecipient("Recipient", email);
  }

  String emailContent = "Jerk Detected!\n" + accelData + "\n" + gpsData;
  message.text.content = emailContent.c_str();

  // Email session setup
  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;

  // Connect and send email
  Serial.println("Attempting to connect to mail server...");
  if (!smtp.connect(&config)) {
    Serial.println("Failed to connect to mail server");
    return;
  }
  
  Serial.println("Sending email...");
  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Failed to send email");
  } else {
    Serial.println("Email sent successfully!");
    jerkDetected = false;

  }

  smtp.closeSession();
}
