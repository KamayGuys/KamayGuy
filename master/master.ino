// master.ino

// Code for the remote microcontroller that sends commands to the rover and arm

// hardware requirements:

// - arduino mega 2560
// - hc-05 bluetooth module
// - 6 flex sensors

#define LOWER 0
#define UPPER 1023
#define DELAY_MS 50

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

unsigned long flexStartTime = 0;
bool currentlyFlexing = false;

// Send a 4-byte message to the slave for the arm modes and rover mode:
// ### `[claw mode] [elbow mode] [shoulder mode] [rover mode]`
void loop() {
  int leftMiddle = analogRead(LEFT_MIDDLE);
  int leftIndex = analogRead(LEFT_INDEX);
  int leftThumb = analogRead(LEFT_THUMB);

  int rightMiddle = analogRead(RIGHT_MIDDLE);
  int rightIndex = analogRead(RIGHT_INDEX);
  int rightThumb = analogRead(RIGHT_THUMB);

  int shoulderMode = rightIndex < 20 ? 2 : rightIndex < 900 ? 1 : 0;
  int elbowMode = rightMiddle < 40 ? 2 : rightMiddle < 900 ? 1 : 0;
  int isFlexingClaw = rightThumb < 200;

  int clawMode = 0;

  if (isFlexingClaw) {
    if (!currentlyFlexing) {
      flexStartTime = millis();
      currentlyFlexing = true;
    }

    if (millis() - flexStartTime > 1000) {
      clawMode = 1;
      flexStartTime = millis();
    }
  } else {
    currentlyFlexing = false;
  }

  // Serial.print("middle:");
  // Serial.print(leftMiddle);
  // Serial.print("\tindex:");
  // Serial.print(leftIndex);
  // Serial.print("\tthumb:");
  // Serial.println(leftThumb);

  // Serial.print("||||||||||||||||||||||||||");
  // Serial.print("middle:");
  // Serial.print(rightMiddle);
  // Serial.print("\tindex:");
  // Serial.print(rightIndex);
  // Serial.print("\tthumb:");
  // Serial.println(rightThumb);

  bool forward = leftIndex < 200;
  bool left = leftMiddle < 200;
  bool right = leftThumb < 200;
  bool backward = left && right;
  int roverMode = calcDirection(forward, backward, left, right);

  // Serial.print(forward ? "FORWARD" : "");
  // Serial.print("\t\t");
  // Serial.print(left ? "LEFT" : "");
  // Serial.print("\t\t");
  // Serial.print(right ? "RIGHT" : "");
  // Serial.print("\t\t");
  // Serial.println(backward ? "BACKWARD" : "");

  // Serial.print(shoulderMode == 2   ? "SHOULDER-EXTEND"
  //              : shoulderMode == 1 ? "SHOULDER-HOLD"
  //                                  : "");
  // Serial.print("\t\t");
  // Serial.print(elbowMode == 2   ? "ELBOW-EXTEND"
  //              : elbowMode == 1 ? "ELBOW-HOLD"
  //                               : "");
  // Serial.print("\t\t");
  // Serial.println(clawMode ? "CLAW" : "");

  // Serial.println(roverMode);

  // String message = encode(clawMode, elbowMode, shoulderMode, roverMode);

  // Serial.write()

  Serial.write(clawMode);
  Serial.write(shoulderMode);
  Serial.write(elbowMode);
  Serial.write(roverMode);

  delay(DELAY_MS); // delay to prevent buffer overflow
}