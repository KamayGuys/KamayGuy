// slave.ino

// Code for the microcontroller that receives commands from master. Should move
// wheels and arm joints

// hardware requirements:

// - arduino mega 2560
// - hc-05 bluetooth module
// - four drive motors
// - three servos

#include "ServoJoint.h"

#define CLAW_PIN 9
#define ELBOW_PIN 10
#define SHOULDER_PIN 11

#define REVERT 0
#define STAY 1
#define EXTEND 2

#define COLLISION_ANGLE 20

#define FRONT_LEFT 3
#define FRONT_LEFT_2 4

#define FRONT_RIGHT 7
#define FRONT_RIGHT_2 8

#define BACK_LEFT 5
#define BACK_LEFT_2 6

#define BACK_RIGHT 13
#define BACK_RIGHT_2 12

// claw: 0-70
// elbow: 90-180
// shoulder: 0-90
ServoJoint claw(CLAW_PIN, 0, 70);
ServoJoint elbow(ELBOW_PIN, 0, 90);
ServoJoint shoulder(SHOULDER_PIN, 0, 50);

auto elbowIsSafe = []() -> bool { return shoulder.getAngle() > COLLISION_ANGLE; };

void wheelForward(int in1, int in2) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void wheelBackward(int in1, int in2) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void wheelStop(int in1, int in2) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void handleElbow(int mode) {
  if (mode == 0) {
    elbow.setAngle(270);
  }
  else if (mode == 2) {
    elbow.setAngle(elbowIsSafe() ? 0 : 60);
  } else {
    return;
  }
}

void handleServo(int mode, ServoJoint &servo) {
  if (mode == 0) {
    // Serial.println("REVERT");
    servo.setAngle(servo.getAngle() + 1);
    delay(20);
  }
  else if (mode == 2) {
    // Serial.println("EXTEND");
    servo.setAngle(servo.getAngle() - 1);
    delay(20);
  } else {
    return;
  }
}

void handleShoulder(int mode) {
  if (mode == 0) {
    shoulder.setAngle(shoulder.getAngle() + 1);
    delay(20);
  }
  else if (mode == 2) {
    shoulder.setAngle(shoulder.getAngle() - 1);
    delay(20);
  } else {
    return;
  }
}

bool isClosed = false;

void handleClaw(int mode) {
  if (mode == 0) {
    return;
  }
  else if (mode == 1) {
    if (isClosed) {
      claw.setAngle(0);
      isClosed = false;
    } else {
      claw.setAngle(270);
      isClosed = true;
    }
  }
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
    // Serial.println("Stop");

    wheelStop(FRONT_LEFT, FRONT_LEFT_2);
    wheelStop(FRONT_RIGHT, FRONT_RIGHT_2);
    wheelStop(BACK_LEFT, BACK_LEFT_2);
    wheelStop(BACK_RIGHT, BACK_RIGHT_2);
  };

  auto forward = []() {
    // Serial.println("Forward");

    wheelForward(FRONT_LEFT, FRONT_LEFT_2);
    wheelForward(FRONT_RIGHT, FRONT_RIGHT_2);
    wheelForward(BACK_LEFT, BACK_LEFT_2);
    wheelForward(BACK_RIGHT, BACK_RIGHT_2);
  };

  auto backward = []() {
    // Serial.println("Backward");

    wheelBackward(FRONT_LEFT, FRONT_LEFT_2);
    wheelBackward(FRONT_RIGHT, FRONT_RIGHT_2);
    wheelBackward(BACK_LEFT, BACK_LEFT_2);
    wheelBackward(BACK_RIGHT, BACK_RIGHT_2);
  };

  auto turnLeft = []() {
    // Serial.println("Left");

    wheelBackward(FRONT_LEFT, FRONT_LEFT_2);
    wheelBackward(BACK_LEFT, BACK_LEFT_2);
    wheelForward(FRONT_RIGHT, FRONT_RIGHT_2);
    wheelForward(BACK_RIGHT, BACK_RIGHT_2);
  };

  auto turnRight = []() {
    // Serial.println("Right");

    wheelForward(FRONT_LEFT, FRONT_LEFT_2);
    wheelForward(BACK_LEFT, BACK_LEFT_2);
    wheelBackward(FRONT_RIGHT, FRONT_RIGHT_2);
    wheelBackward(BACK_RIGHT, BACK_RIGHT_2);
  };

  if (mode == 0)
    stop();
  else if (mode == 1)
    backward();
  else if (mode == 2)
    forward();
  else if (mode == 3)
    turnRight();
  else if (mode == 4)
    turnLeft();
}

void setup() {
  Serial.begin(38400);

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
  elbow.setAngle(90);
  shoulder.setAngle(270);
}

// Receive a 4-byte message from the master for the arm modes and rover mode:
// ### `[claw mode] [elbow mode] [shoulder mode] [rover mode]`
void loop() {
  if (Serial.available() >= 4) { // Ensure four bytes are available
    // handleClaw(Serial.read());
    handleClaw(Serial.read());
    handleShoulder(Serial.read());
    handleElbow(Serial.read());
    // handleServo(Serial.read(), elbow);
    handleRover(Serial.read());
  }
}