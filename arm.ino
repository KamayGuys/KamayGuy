#include <Servo.h>

#define REVERT 0
#define STAY 1
#define ROTATE 2

const int CLAW_FLEX_PIN = A0;
const int ELBOW_FLEX_PIN = A1;
const int SHOULDER_FLEX_PIN = A2;

const int CLAW_PIN = 13;
const int ELBOW_PIN = 12;
const int SHOULDER_PIN = 11;

// TODO: set thresholds for each servo
const int UPPER = 900;
const int LOWER = 424;

const int STEP = 15;
const int DELAY = 15;

int claw_flex_val, elbow_flex_val, shoulder_flex_val;
int claw_mode, elbow_mode, shoulder_mode;
int claw_servo_pos, elbow_servo_pos, shoulder_servo_pos = 0;

Servo claw_servo, elbow_servo, shoulder_servo;

void setup() {
  claw_servo.attach(CLAW_PIN);
  Serial.begin(9600);
}

void loop() {
  claw_flex_val = analogRead(CLAW_FLEX_PIN);
  elbow_flex_val = analogRead(ELBOW_FLEX_PIN);
  shoulder_flex_val = analogRead(SHOULDER_FLEX_PIN);
  // Serial.println(claw_flex_val);
  claw_mode = map(claw_flex_val, LOWER, UPPER, 0, 1);
  elbow_mode = map(elbow_flex_val, LOWER, UPPER, 0, 2);
  shoulder_mode = map(shoulder_flex_val, LOWER, UPPER, 0, 2);
  // Serial.println(claw_mode);

  if (claw_flex_val) { // start rotating servo until 180deg
    claw_servo_pos = min(claw_servo_pos + STEP, 180);
    claw_servo.write(claw_servo_pos);
  } else { // return servo to beginning
    claw_servo.write(claw_servo_pos - STEP, 0);
  }

  if (elbow_flex_val == REVERT) { // return servo to beginning
    elbow_servo_pos = max(elbow_servo_pos - STEP, 0);
    elbow_servo.write(elbow_servo_pos);
  } else if (elbow_flex_val == ROTATE) { // start rotating servo until 180deg
    elbow_servo_pos = min(elbow_servo_pos + STEP, 180);
    elbow_servo.write(elbow_servo_pos);
  } else { // stay at current position
    elbow_servo.write(elbow_servo_pos);
  }

  if (shoulder_flex_val == REVERT) { // return servo to beginning
    shoulder_servo_pos = max(shoulder_servo_pos - STEP, 0);
    shoulder_servo.write(shoulder_servo_pos);
  } else if (shoulder_flex_val == ROTATE) { // start rotating servo until 180deg
    shoulder_servo_pos = min(shoulder_servo_pos + STEP, 180);
    shoulder_servo.write(shoulder_servo_pos);
  } else { // stay at current position
    shoulder_servo.write(shoulder_servo_pos);
  }

  delay(DELAY);
}
