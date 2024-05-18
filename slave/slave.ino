// slave.ino

// Code for the microcontroller that receives commands from master. Should move
// wheels and arm joints

// hardware requirements:

// - a mega 2560
// - 2 hc-05 bluetooth modules
// - two drive motors
// - three servos

#include "ServoJoint.h"

#define CLAW_PIN 9
#define ELBOW_PIN 10
#define SHOULDER_PIN 11

#define REVERT 2
#define STAY 1
#define ROTATE 0

ServoJoint claw(CLAW_PIN, 20, 90);
ServoJoint elbow(ELBOW_PIN, 20, 180);
ServoJoint shoulder(SHOULDER_PIN, 20, 90);

// ### Handle the mode of the servo
//
// `0`: revert to 0
// `1`: stay at current position
// `2`: rotate to 180
//
void handleServo(int mode, ServoJoint &servo) {
  if (mode == REVERT)
    servo.setAngle(0);
  else if (mode == STAY)
    servo.setAngle(servo.getAngle());
  else if (mode == ROTATE)
    servo.setAngle(270);
}

// ### Handle the mode and direction of the rover
//
// Modes:
// `0`: stop
// `1`: forward
// `2`: backward
// `3`: left
// `4`: right
//
void handleRover(int mode) {
  auto stop = []() {
    Serial.println("Stop");
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  };

  auto forward = []() {
    Serial.println("Forward");
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(3, LOW);
  };

  auto backward = []() {
    Serial.println("Backward");
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
  };

  auto turnLeft = []() {
    Serial.println("Left ");
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  };

  auto turnRight = []() {
    Serial.println("Right ");
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
  };

  if (mode == 0)
    stop();
  else if (mode == 1)
    forward();
  else if (mode == 2)
    backward();
  else if (mode == 3)
    turnLeft();
  else if (mode == 4)
    turnRight();
}

void setup() {
  Serial.begin(38400);
  Serial.println("version 0.7.0");

  // motor pins
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);

  claw.attach();
  elbow.attach();
  shoulder.attach();

  claw.setAngle(20);
  elbow.setAngle(20);
  shoulder.setAngle(20);
}

auto moveArm = []() {
  Serial.println("elbow flex");
  handleServo(2, elbow);
  Serial.println("shoulder flex");
  handleServo(2, shoulder);
  delay(2000);
  Serial.println("elbow down");
  handleServo(0, elbow);
  Serial.println("shoulder down");
  handleServo(0, shoulder);
  delay(2000);
};

// Receive a 4-byte message from the master for the arm modes and rover mode:
// [claw mode] [elbow mode] [shoulder mode] [rover mode]

void loop() {
  if (Serial.available() >= 4) { // Ensure four bytes are available
    handleServo(Serial.read(), claw);
    handleServo(Serial.read(), elbow);
    handleServo(Serial.read(), shoulder);
    handleRover(Serial.read());
  }
  // moveArm();
}