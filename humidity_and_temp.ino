#include <DHT.h>

#define DHTPIN 2          // Digital pin connected to DHT11 DATA pin
#define DHTTYPE DHT11     // Sensor type: DHT11

DHT dht(DHTPIN, DHTTYPE); // Create DHT object

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 Temperature & Humidity Sensor Test");
  Serial.println("---------------------------------------------");
  
  dht.begin();
  delay(2000); // Wait for sensor to stabilize
}

void loop() {
  // Wait 2 seconds between readings (DHT11 is slow)
  delay(2000);

  float humidity = dht.readHumidity();        // Read humidity (%)
  float temperature = dht.readTemperature();  // Read temperature in Celsius

  // Check if reading failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return;
  }

  // Display results
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");                // \t = tab space

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  
}
