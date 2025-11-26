#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <DHT.h>

// ------------------ USER CONFIG -----------------------
const char* ssid = "BELL4G-F2C2";
const char* password = "B1DM5QLNB17";

const char* firebaseHost = "finalapp-54e14-default-rtdb.asia-southeast1.firebasedatabase.app";
const char* firebaseApiKey = "AIzaSyCz8qAl2cAfk5U1OsPPcowyAbXHiOK95P8";

// DHT11 sensor
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Rain sensor
#define RAIN_SENSOR D6
// -------------------------------------------------------

unsigned long lastUpdate = 0;
unsigned long lastCommandPoll = 0;

void setup() {
  Serial.begin(9600);
  delay(500);

  dht.begin();
  pinMode(RAIN_SENSOR, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
  }
}

void loop() {
  unsigned long now = millis();

  // Upload sensor data every 3 seconds
  if (now - lastUpdate > 3000) {
    lastUpdate = now;
    sendSensorData();
  }

  // Check start command every 1 sec
  if (now - lastCommandPoll > 1000) {
    lastCommandPoll = now;
    pollStartCommand();
  }
}

// -------------------------------------------------------
// SEND SENSOR DATA INTO SmartRack/
// -------------------------------------------------------
void sendSensorData() {
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  int rainRaw = digitalRead(RAIN_SENSOR);
  int rain = (rainRaw == LOW) ? 1 : 0;

  // Update Firebase
  sendNumber("SmartRack/temp", temp);
  sendNumber("SmartRack/humidity", humidity);
  sendNumber("SmartRack/rain", rain);

  // --- CLEAN OUTPUT FOR ARDUINO ONLY ---
  if (rain == 1) {
    Serial.println("RAIN");   // ✔ clean command
  } else {
    Serial.println("CLEAR");  // ✔ clean command
  }
}

// -------------------------------------------------------
void sendNumber(String path, float value) {
  String url = String("https://") + firebaseHost + "/" + path + ".json?auth=" + firebaseApiKey;

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  if (https.begin(client, url)) {
    https.addHeader("Content-Type", "application/json");
    String json = String(value);
    https.PUT(json);
    https.end();
  }
}

// -------------------------------------------------------
void pollStartCommand() {
  String url = String("https://") + firebaseHost + "/SmartRack/command.json?auth=" + firebaseApiKey;

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  if (https.begin(client, url)) {
    int code = https.GET();

    if (code == 200) {
      String cmd = https.getString();
      cmd.replace("\"", "");

      if (cmd == "start") {
        Serial.println("START");  // ✔ send clean command
        clearStartCommand();
      }
    }

    https.end();
  }
}

// -------------------------------------------------------
void clearStartCommand() {
  String url = String("https://") + firebaseHost + "/SmartRack/command.json?auth=" + firebaseApiKey;

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  if (https.begin(client, url)) {
    https.addHeader("Content-Type", "application/json");
    https.PUT("\"none\"");
    https.end();
  }
}
