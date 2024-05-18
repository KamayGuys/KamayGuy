// master.ino

// Code for the remote microcontroller that sends commands to the rover and arm

// hardware requirements:

// - arduino mega 2560
// - hc-05 bluetooth module
// - 6 flex sensors

#define LOWER 0
#define UPPER 1023
#define DELAY_MS 100

// arm flex pins
const int LEFT_MIDDLE = A0;
const int LEFT_INDEX = A1;
const int LEFT_THUMB = A2;

// rover flex pins
const int RIGHT_THUMB = A3;
const int RIGHT_INDEX = A4;
const int RIGHT_MIDDLE = A5;
// const int RIGHT_PIN = A6;

void setup() { Serial.begin(38400); }

int calcDirection(bool forward, bool backward, bool left, bool right) {
  if (backward)
    return 2;
  else if (left)
    return 3;
  else if (right)
    return 4;
  else if (forward)
    return 1;
  else
    return 0;
}

// Send a 4-byte message to the slave for the arm modes and rover mode:
// ### `[claw mode] [elbow mode] [shoulder mode] [rover mode]`
void loop() {
  const leftMiddle = analogRead(LEFT_MIDDLE);
  const leftIndex = analogRead(LEFT_INDEX);
  const leftThumb = analogRead(LEFT_THUMB);

  const rightMiddle = analogRead(RIGHT_MIDDLE);
  const rightIndex = analogRead(RIGHT_INDEX);
  const rightThumb = analogRead(RIGHT_THUMB);

  const shoulderMode = map(rightMiddle, LOWER, UPPER, 2, 0);
  const elbowMode = map(rightIndex, LOWER, UPPER, 2, 0);
  const clawMode = map(rightThumb, LOWER, UPPER, 2, 0);

  Serial.print("middle:");
  Serial.print(leftMiddle);
  Serial.print("\tindex:");
  Serial.print(leftIndex);
  Serial.print("\tthumb:");
  Serial.println(leftThumb);

  Serial.print("-RIGHT VALUES-");
  Serial.print("middle:");
  Serial.print(rightMiddle);
  Serial.print("\tindex:");
  Serial.print(rightIndex);
  Serial.print("\tthumb:");
  Serial.println(rightThumb);

  int forward = map(leftIndex, LOWER, UPPER, 1, 0);
  int left = map(leftMiddle, LOWER, UPPER, 1, 0);
  int right = map(leftThumb, LOWER, UPPER, 1, 0);
  int backward = left && right;
  int roverMode = calcDirection(forward, backward, left, right);

  Serial.write(clawMode);
  Serial.write(elbowMode);
  Serial.write(shoulderMode);
  Serial.write(roverMode);

  delay(DELAY_MS); // delay to prevent buffer overflow
}