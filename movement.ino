// C++ code

int sensorPin1 = A0;
int sensorPin2 = A1;
int sensorPin3 = A2;
int sensorPin4 = A3;
int sensor = 0;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  checkForward(sensorPin1);
  checkBackward(sensorPin2);
  checkLeft(sensorPin3);
  checkRight(sensorPin4);
}

// 13 HIGH, 12 LOW, 2 HIGH, 3 LOW --> Forward
// 13 LOW, 12 HIGH, 2 LOW, 3 HIGH --> Backward
// 13 LOW, 12 HIGH, 2 HIGH, 3 LOW --> Left
// 13 HIGH, 12 LOW, 2 LOW, 3 HIGH --> Right

void checkForward(int sensorPin) {
  sensor = analogRead(sensorPin);
  if (sensor > 650) {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
  } else if (sensor < 650) {
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
  }
}

void checkBackward(int sensorPin) {
  sensor = analogRead(sensorPin);
  if (sensor > 650) {
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
  }
}

void checkLeft(int sensorPin) {
  sensor = analogRead(sensorPin);
  if (sensor > 650) {
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
  } else {
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
  }
}

void checkRight(int sensorPin) {
  sensor = analogRead(sensorPin);
  if (sensor > 650) {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
  }
}