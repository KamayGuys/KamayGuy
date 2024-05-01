// ServoJoint.cpp

#include "ServoJoint.h"
#include "Arduino.h"

ServoJoint::ServoJoint(int pin, int minAngle, int maxAngle)
    : pin(pin), minAngle(minAngle), maxAngle(maxAngle), servo() {}

void ServoJoint::attach() { servo.attach(pin); }

int ServoJoint::getAngle() { return servo.read(); }

void ServoJoint::setAngle(int angle) {
  int clampedAngle = constrain(angle, minAngle, maxAngle);
  servo.write(clampedAngle);
}