// ====== Pins ======
#define trigC 2
#define echoC 3
#define trigL 4
#define echoL 5
#define trigR 6
#define echoR 7

#define IR_LEFT A0
#define IR_RIGHT A1

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// ====== Parameters ======
int obstacleDist = 20;  // cm
int spd = 200;          // PWM speed (0-255)

// ====== Functions ======
long readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 20000); // 20ms timeout
  return duration * 0.034 / 2; // convert to cm
}

void forward() {
  analogWrite(IN1, spd);
  analogWrite(IN2, 0);
  analogWrite(IN3, spd);
  analogWrite(IN4, 0);
}

void backward() {
  analogWrite(IN1, 0);
  analogWrite(IN2, spd);
  analogWrite(IN3, 0);
  analogWrite(IN4, spd);
}

void leftTurn() {
  analogWrite(IN1, 0);
  analogWrite(IN2, spd);
  analogWrite(IN3, spd);
  analogWrite(IN4, 0);
}

void rightTurn() {
  analogWrite(IN1, spd);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, spd);
}

void stopBot() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

void setup() {
  Serial.begin(9600);

  pinMode(trigC, OUTPUT); pinMode(echoC, INPUT);
  pinMode(trigL, OUTPUT); pinMode(echoL, INPUT);
  pinMode(trigR, OUTPUT); pinMode(echoR, INPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // Read IR sensors
  int irLeft = digitalRead(IR_LEFT);
  int irRight = digitalRead(IR_RIGHT);

  // Read distances
  long dC = readUltrasonic(trigC, echoC);
  long dL = readUltrasonic(trigL, echoL);
  long dR = readUltrasonic(trigR, echoR);

  // ---- Priority 1: Obstacle avoidance ----
  if (dC < obstacleDist || dL < obstacleDist || dR < obstacleDist) {
    stopBot();
    delay(200);

    if (dL > dR) {
      leftTurn();
      delay(400);
    } else {
      rightTurn();
      delay(400);
    }
  }
  // ---- Priority 2: Line following ----
  else {
    if (irLeft == LOW && irRight == LOW) {
      forward(); // on track
    } else if (irLeft == LOW && irRight == HIGH) {
      leftTurn();
    } else if (irLeft == HIGH && irRight == LOW) {
      rightTurn();
    } else { // both HIGH -> no line
      stopBot();
    }
  }
}
