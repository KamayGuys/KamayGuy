// ServoJoint.h

#ifndef ServoJoint_h
#define ServoJoint_h

#include <Servo.h>

class ServoJoint {
public:
  ServoJoint(int pin, int minAngle, int maxAngle);
  void attach();
  int getAngle();
  void setAngle(int angle);

private:
  int pin;
  int min;
  int max;
  Servo servo;
};

#endif
