// slave.ino

// Code for the microcontroller that receives commands from master. Should move
// wheels and arm joints

// hardware requirements:

// - arduino mega 2560
// - 2 hc-05 bluetooth modules
// - four drive motors
// - three servos

#include "ServoJoint.h"

#define CLAW_PIN 9
#define ELBOW_PIN 10
#define SHOULDER_PIN 11

#define REVERT 2
#define STAY 1
#define EXTEND 0

#define COLLISION_ANGLE 70

// TODO: make it so one finger controls the arm where the elbow stays level with
// the ground, then the other finger controls elbow for if we still need to move
// elbow a diff. angle

// claw: ?
// elbow: 90-180
// shoulder: 0-90
ServoJoint claw(CLAW_PIN, 0, 50);
ServoJoint elbow(ELBOW_PIN, 30, 90);
ServoJoint shoulder(SHOULDER_PIN, 0, 90);

auto elbowIsSafe = []() { return shoulder.getAngle() > COLLISION_ANGLE; };

// ### Handle elbow modes
//
// `REVERT`: revert to min angle
// `STAY`: stay at current position
// `EXTEND`: rotate to max angle
//
void handleElbow(int mode) {
  if (mode == REVERT)
    elbow.setAngle(90);
  else if (mode == STAY)
    elbow.setAngle(elbow.getAngle());
  else if (mode == EXTEND)
    elbow.setAngle(elbowIsSafe() ? 30 : 60);
}

// ### Handle shoulder modes
//
// `REVERT`: revert to min angle
// `STAY`: stay at current position
// `EXTEND`: rotate to max angle
//
void handleShoulder(int mode) {
  if (mode == REVERT)
    shoulder.setAngle(0);
  else if (mode == STAY)
    shoulder.setAngle(shoulder.getAngle());
  else if (mode == EXTEND)
    shoulder.setAngle(270);
}

// ### Handle claw modes
//
// `REVERT`: open claw
// `EXTEND`: close claw
//
void handleClaw(int mode) {
  if (mode == REVERT)
    shoulder.setAngle(0);
  else if (mode == EXTEND)
    shoulder.setAngle(270);
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
    Serial.println("Left");
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
    Serial.println("Right");
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

  claw.setAngle(0);
  elbow.setAngle(0);
  shoulder.setAngle(0);
}

auto moveArm = []() {
  Serial.println("shoulder down");
  handleShoulder(REVERT);
  Serial.println("elbow down");
  handleElbow(REVERT);
  delay(2000);

  Serial.println("shoulder flex");
  handleShoulder(EXTEND);
  Serial.println("elbow flex");
  handleElbow(EXTEND);
  delay(2000);
};

// Receive a 4-byte message from the master for the arm modes and rover mode:
// ### `[claw mode] [elbow mode] [shoulder mode] [rover mode]`
void loop() {
  if (Serial.available() >= 4) { // Ensure four bytes are available
    handleClaw(Serial.read());
    handleElbow(Serial.read());
    handleShoulder(Serial.read());
    handleRover(Serial.read());
  }
  // moveArm();
}