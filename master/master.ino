// master.ino

// Code for the remote microcontroller that sends commands to the rover and arm

// hardware requirements:

// - a mega 2560
// - hc-05 bluetooth module
// - 7 flex sensors

#define LOWER 424
#define UPPER 900
#define DELAY_MS 100

// arm flex pins
const int FLEX_PIN_CLAW = A0;
const int FLEX_PIN_ELBOW = A1;
const int FLEX_PIN_SHOULDER = A2;

// rover flex pins
const int FORWARD_PIN = A3;
const int BACKWARD_PIN = A4;
const int LEFT_PIN = A5;
const int RIGHT_PIN = A6;

void setup() { Serial.begin(38400); }

// Send a 4-byte message to the slave for the arm modes and rover mode:
// [claw mode] [elbow mode] [shoulder mode] [rover mode]

void loop() {
  // Send arm modes as 0, 1, or 2
  Serial.write(map(analogRead(FLEX_PIN_CLAW), LOWER, UPPER, 0, 2));
  Serial.write(map(analogRead(FLEX_PIN_ELBOW), LOWER, UPPER, 0, 2));
  Serial.write(map(analogRead(FLEX_PIN_SHOULDER), LOWER, UPPER, 0, 2));

  // Read rover flex sensors
  int isForward = map(analogRead(FORWARD_PIN), LOWER, UPPER, 0, 1);
  int isBackward = map(analogRead(BACKWARD_PIN), LOWER, UPPER, 0, 1);
  int isLeft = map(analogRead(LEFT_PIN), LOWER, UPPER, 0, 1);
  int isRight = map(analogRead(RIGHT_PIN), LOWER, UPPER, 0, 1);

  // TODO: what if we want to turn left and move forward at the same time?
  // Send rover modes as 1, 2, 3, 4, or 0
  Serial.write(isForward ? 1 : isBackward ? 2 : isLeft ? 3 : isRight ? 4 : 0);

  delay(DELAY_MS); // Slight delay to manage timing and prevent buffer overflow
}