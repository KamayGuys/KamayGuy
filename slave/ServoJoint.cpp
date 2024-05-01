// ServoJoint.cpp

#include "ServoJoint.h"

ServoJoint::ServoJoint(int pin, int minAngle, int maxAngle)
    : pin(pin), min(minAngle), max(maxAngle), servo() {}

void ServoJoint::attach() { servo.attach(pin); }

int ServoJoint::getAngle() { return servo.read(); }

void ServoJoint::setAngle(int angle) {
  int clampedAngle = constrain(angle, min, max);
  servo.write(clampedAngle);
}