#include <Servo.h>

// IR Sensors for line following
#define IR_LEFT A0
#define IR_RIGHT A1

// Ultrasonic Sensor Pins
#define TRIG 8
#define ECHO 9

// Motor Driver Pins (L298N)
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

// Servo Pins
#define SERVO_PIN 10          // Main servo (rack lock)
#define ULTRA_SERVO_PIN 11    // Servo to rotate ultrasonic sensor

Servo rackServo;
Servo ultraServo;

String command = "";
bool isMoving = false;

void setup() {
  Serial.begin(9600);
  
  // Initialize IR sensors
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  
  // Initialize Ultrasonic
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  // Initialize Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Initialize Servos
  rackServo.attach(SERVO_PIN);
  rackServo.write(0);  // Initial position - rack open
  
  ultraServo.attach(ULTRA_SERVO_PIN);
  ultraServo.write(90);   // Start facing forward
  
  // Start with motors stopped
  stopMotor();
  
  Serial.println("Arduino Ready - Waiting for RAIN command to start...");
}

// Get distance from ultrasonic sensor
int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

// Motor control functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotor() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // Right wheel backward for sharp turn
  Serial.println("Turning LEFT");
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  // Left wheel backward for sharp turn
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Turning RIGHT");
}

void smoothLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void smoothRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Scan for free direction using ultrasonic servo
int scanDirection() {
  int leftDist, rightDist, frontDist;

  // Look LEFT (30°)
  ultraServo.write(30);
  delay(500);
  leftDist = getDistance();

  // Look RIGHT (150°)
  ultraServo.write(150);
  delay(500);
  rightDist = getDistance();

  // Look FRONT (90°)
  ultraServo.write(90);
  delay(300);
  frontDist = getDistance();

  Serial.print("Scan - Left: ");
  Serial.print(leftDist);
  Serial.print("cm, Front: ");
  Serial.print(frontDist);
  Serial.print("cm, Right: ");
  Serial.print(rightDist);
  Serial.println("cm");

  // Decide best direction
  if (leftDist > rightDist && leftDist > 25) {
    return 1;  // Left is better
  } else if (rightDist > 25) {
    return 2;  // Right is better
  } else if (frontDist > 25) {
    return 3;  // Forward is better
  } else {
    return 0;  // No good direction, go backward
  }
}

// Line following function
void followLine() {
  int leftIR = digitalRead(IR_LEFT);
  int rightIR = digitalRead(IR_RIGHT);

  // Line following logic (assuming IR sensors give LOW on line, HIGH off line)
  if (leftIR == LOW && rightIR == LOW) { 
    // Both sensors on line - move forward
    moveForward();
    Serial.println("Line Following: FORWARD");
  } 
  else if (leftIR == LOW && rightIR == HIGH) { 
    // Left on line, right off - turn left slightly
    smoothLeft();
    Serial.println("Line Following: Adjusting LEFT");
  } 
  else if (leftIR == HIGH && rightIR == LOW) { 
    // Right on line, left off - turn right slightly
    smoothRight();
    Serial.println("Line Following: Adjusting RIGHT");
  } 
  else {
    // Both sensors off line - stop or search
    stopMotor();
    Serial.println("Line Following: LINE LOST - Stopping");
    // Optional: Add line search behavior here
  }
}

// Obstacle avoidance function
bool avoidObstacle() {
  int dist = getDistance();
  
  if (dist < 20) { // Obstacle detected within 20cm
    Serial.println("OBSTACLE DETECTED! Finding alternative path...");
    stopMotor();
    delay(1000);
    
    int bestDir = scanDirection();
    
    switch(bestDir) {
      case 1: // Turn Left
        turnLeft();
        delay(800);
        moveForward();
        delay(1000);
        break;
        
      case 2: // Turn Right
        turnRight();
        delay(800);
        moveForward();
        delay(1000);
        break;
        
      case 3: // Go Forward
        moveForward();
        delay(1000);
        break;
        
      case 0: // Go Backward
        moveBackward();
        delay(1500);
        turnRight();
        delay(1000);
        break;
    }
    
    stopMotor();
    delay(200);
    return true; // Obstacle was avoided
  }
  
  return false; // No obstacle
}

void loop() {
  // Read command from NodeMCU
  if (Serial.available() > 0) {
    String newCommand = Serial.readString();
    newCommand.trim();
    
    if (newCommand == "RAIN" || newCommand == "CLEAR") {
      command = newCommand;
      Serial.println("Received: " + command);
      
      if (command == "RAIN") {
        isMoving = true;
        Serial.println("RAIN DETECTED - Starting movement system!");
        rackServo.write(90); // Close rack for rain protection
      } else {
        isMoving = false;
        Serial.println("CLEAR WEATHER - Stopping system");
        stopMotor();
        rackServo.write(0); // Open rack
      }
    }
  }

  // MAIN MOVEMENT LOGIC - Only when RAIN is detected
  if (isMoving) {
    // First check for obstacles (highest priority)
    bool obstacleAvoided = avoidObstacle();
    
    // If no obstacle was avoided, then follow line
    if (!obstacleAvoided) {
      followLine();
    }
    
    // Small delay for stability
    delay(100);
  } else {
    // Not moving - ensure motors are stopped
    stopMotor();
  }
}