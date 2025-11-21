#include <Servo.h>

Servo myServo;                  // Create servo object

const int trigPin = 9;          // Trig pin of HC-SR04
const int echoPin = 10;         // Echo pin of HC-SR04
const int servoPin = 6;         // Servo signal pin

long duration;
int distance;
int threshold = 20;             // Distance in cm to trigger servo (adjust as needed)

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  myServo.attach(servoPin);
  myServo.write(0);             // Start at closed position
  
  Serial.begin(9600);
  Serial.println("Ultrasonic + Servo Obstacle Detector Started");
  delay(1000);
}

void loop() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send 10us pulse to trigPin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in cm
  distance = duration * 0.034 / 2;
  
  // Print distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Control servo based on distance
  if (distance < threshold && distance > 0) {
    Serial.println("==> Obstacle Detected! Opening...");
    myServo.write(90);          // Move servo to 90 degrees (open)
  } else {
    Serial.println("==> No Obstacle - Closing...");
    myServo.write(0);           // Return to 0 degrees (closed)
  }
  
  delay(500);  // Update every 0.5 seconds (adjust for smoother/faster response)
}