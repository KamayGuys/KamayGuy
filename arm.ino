#include <Servo.h>
const int flexPin = A2;
const int Servo1Pin = 13;
const int Servo2Pin = 12;
const int Servo3pin = 11;
const int UpperThresh = 900;
const int LowerThresh = 424;

int flexPos;
int servo1Pos = 0;
Servo myservo1;

void setup()
{
  myservo1.attach(Servo1Pin);
  Serial.begin(9600);
}

void loop()
{
  flexPos = analogRead(flexPin);
  //Serial.println(flexPos);
  flexPos = map(flexPos, LowerThresh, UpperThresh, 0, 2);
  Serial.println(flexPos);
  
  if (flexPos == 0){ // return servo to beginning
    servo1Pos = max(servo1Pos - 15, 0);
    myservo1.write(servo1Pos);
  } else if (flexPos == 2){ // start rotating servo until 180deg
    servo1Pos = min(servo1Pos + 15, 180);
    myservo1.write(servo1Pos);
  } else { // stay at current position
    myservo1.write(servo1Pos);
  }
  delay(15);
}
