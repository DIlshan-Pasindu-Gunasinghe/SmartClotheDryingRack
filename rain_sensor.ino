

const int rainDigitalPin = 2;    // Digital pin from rain sensor
const int ledPin = 13;           // Built-in LED or external LED
const int analogPin = A0;        // Analog pin (optional - for sensitivity)

void setup() {
  pinMode(rainDigitalPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Rain Sensor Test Started...");
  Serial.println("-----------------------------");
}

void loop() {
  int rainDigital = digitalRead(rainDigitalPin);  // 0 = wet/rain, 1 = dry
  int rainAnalog = analogRead(analogPin);         // 0 = very wet, 1023 = dry

  // Print analog value for calibration
  Serial.print("Analog Value: ");
  Serial.print(rainAnalog);
  Serial.print(" | Digital: ");
  Serial.print(rainDigital);

  if (rainDigital == LOW) {  // Sensor detects water/rain
    Serial.println("  ==> It's Raining!");
    digitalWrite(ledPin, HIGH);   // Turn ON LED
  } else {
    Serial.println("  ==> No Rain - Dry");
    digitalWrite(ledPin, LOW);    // Turn OFF LED
  }

  delay(500);  // Update every 0.5 seconds
}