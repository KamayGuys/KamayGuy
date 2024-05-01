// slave.ino

// board: mega2560

#include "ServoJoint.h"

#define CLAW_PIN 13
#define ELBOW_PIN 12
#define SHOULDER_PIN 11

#define LEFT_FORWARD_PIN 10
#define LEFT_BACKWARD_PIN 9
#define RIGHT_FORWARD_PIN 8
#define RIGHT_BACKWARD_PIN 7

ServoJoint claw(CLAW_PIN, 0, 180);
ServoJoint elbow(ELBOW_PIN, 0, 180);
ServoJoint shoulder(SHOULDER_PIN, 0, 180);

// ### Handle the mode of the servo
//
// `0`: revert to 0
// `1`: stay at current position
// `2`: rotate to 180
//
void handleServo(int mode, ServoJoint &servo) {
  if (mode == 0)
    servo.setAngle(0);
  else if (mode == 1)
    servo.setAngle(servo.getAngle());
  else if (mode == 2)
    servo.setAngle(180);
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
    digitalWrite(LEFT_FORWARD_PIN, LOW);
    digitalWrite(LEFT_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_BACKWARD_PIN, LOW);
  };

  auto forward = []() {
    digitalWrite(LEFT_FORWARD_PIN, HIGH);
    digitalWrite(LEFT_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_FORWARD_PIN, HIGH);
    digitalWrite(RIGHT_BACKWARD_PIN, LOW);
  };

  auto backward = []() {
    digitalWrite(LEFT_FORWARD_PIN, LOW);
    digitalWrite(LEFT_BACKWARD_PIN, HIGH);
    digitalWrite(RIGHT_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_BACKWARD_PIN, HIGH);
  };

  auto turnLeft = []() {
    digitalWrite(LEFT_FORWARD_PIN, LOW);
    digitalWrite(LEFT_BACKWARD_PIN, HIGH);
    digitalWrite(RIGHT_FORWARD_PIN, HIGH);
    digitalWrite(RIGHT_BACKWARD_PIN, LOW);
  };

  auto turnRight = []() {
    digitalWrite(LEFT_FORWARD_PIN, HIGH);
    digitalWrite(LEFT_BACKWARD_PIN, LOW);
    digitalWrite(RIGHT_FORWARD_PIN, LOW);
    digitalWrite(RIGHT_BACKWARD_PIN, HIGH);
  };

  // TODO: what if we want to turn left and move forward at the same time?
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

  // Set wheel pins
  pinMode(LEFT_FORWARD_PIN, OUTPUT);
  pinMode(LEFT_BACKWARD_PIN, OUTPUT);
  pinMode(RIGHT_FORWARD_PIN, OUTPUT);
  pinMode(RIGHT_BACKWARD_PIN, OUTPUT);

  // Attach servos
  claw.attach();
  elbow.attach();
  shoulder.attach();
}

// Message format:
// <claw_mode>
// <elbow_mode>
// <shoulder_mode>
// <rover_mode>

void loop() {
  if (Serial.available() >= 4) { // Ensure four bytes are available
    handleServo(Serial.read(), claw);
    handleServo(Serial.read(), elbow);
    handleServo(Serial.read(), shoulder);
    handleRover(Serial.read());
  }
}