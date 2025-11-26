// ===================== IR SENSORS =====================
#define IR_LEFT A0
#define IR_RIGHT A1

// ===================== ULTRASONIC =====================
#define TRIG 8
#define ECHO 9

// ===================== MOTOR DRIVER L298N =====================
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

#define ENA 3
#define ENB 11

// Line & obstacle settings
#define OBSTACLE_DIST 15

int motorSpeed = 110;
int turnSpeed  = 70;

String command = "CLEAR";      // <<< FORCE SAFE STATE
bool motorsActive = false;     // <<< MOTORS OFF AT START

void setup() {
  Serial.begin(9600);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);         // <<< IMPORTANT
  pinMode(ENB, OUTPUT);         // <<< IMPORTANT

  stopMotors();                 // <<< ENSURE MOTORS OFF
  delay(300);

  Serial.println("Arduino started in CLEAR mode.");
}

// ================== MOTOR CONTROL ==================
void moveForward() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void slightLeft() {
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, motorSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void slightRight() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, turnSpeed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// =============== ULTRASONIC ===============
int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 20000);
  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

// =============== LINE FOLLOW =================
void followLine() {
  int L = digitalRead(IR_LEFT);
  int R = digitalRead(IR_RIGHT);

  if (L == LOW && R == LOW) moveForward();
  else if (L == LOW && R == HIGH) slightLeft();
  else if (L == HIGH && R == LOW) slightRight();
  else moveForward();
}

// ===================== MAIN LOOP =====================
void loop() {

  // Read incoming command
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "RAIN") {
      motorsActive = true;
      Serial.println("RAIN → Motors ON");
    }
    else if (command == "CLEAR") {
      motorsActive = false;
      stopMotors();
      Serial.println("CLEAR → Motors OFF");
    }
  }

  // Only run motors in RAIN mode
  if (command == "RAIN" && motorsActive) {

    int dist = getDistance();

    // Obstacle stop
    if (dist < OBSTACLE_DIST) {
      stopMotors();
      Serial.println("Obstacle detected → STOP");
      return;
    }

    // Resume following line when obstacle clears
    followLine();
  }

  // Always stop motors in CLEAR mode
  if (command == "CLEAR") {
    stopMotors();
  }
}
