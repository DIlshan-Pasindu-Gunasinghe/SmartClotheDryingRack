#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT11
#define RAIN_SENSOR D6    // safer pin

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RAIN_SENSOR, INPUT);
}

void loop() {

  // Read humidity & temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Read rain sensor
  int rain = digitalRead(RAIN_SENSOR);

  if (rain == LOW) {
    Serial.println("RAIN");
  } else {
    Serial.println("CLEAR");
  }

  Serial.print("Humidity: "); Serial.print(h);
  Serial.print(" Temp: "); Serial.print(t);
  Serial.print(" Rain Sensor: ");
  Serial.println(rain ? "CLEAR" : "RAIN");

  delay(1000);
}
