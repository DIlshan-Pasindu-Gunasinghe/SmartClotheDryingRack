
const int irSensor = 2;     // IR sensor output pin
const int in1 = 7;
const int in2 = 8;
const int in3 = 4;
const int in4 = 5;
const int ena = 9;          // PWM for left motor speed
const int enb = 10;         // PWM for right motor speed

int sensorState;

void setup() {
  pinMode(irSensor, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  Serial.begin(9600);
  Serial.println("Line Follower Started!");
  
  // Set motor speed (0-255)
  analogWrite(ena, 150);   // Left motor speed
  analogWrite(enb, 150);   // Right motor speed
}

void loop() {
  sensorState = digitalRead(irSensor);  // Read IR sensor

  if (sensorState == LOW) {  
    // Black line detected → Go Forward
    Serial.println("Line Detected - Going Forward");
    moveForward();
  } 
  else {  
    // No line (white surface) → Search for line (turn right slowly)
    Serial.println("No Line - Searching...");
    searchLine();
  }
  
  delay(50);
}

// Functions
void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void searchLine() {
  // Turn right slowly to find the line again
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(200);
  // You can also add small left-right movements for better search
}

void stopRobot() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}