#define PIN_MOTOR_1_FORWARD 11
#define PIN_MOTOR_1_REVERSE 10
#define PIN_MOTOR_2_FORWARD 9
#define PIN_MOTOR_2_REVERSE 8
#define PIN_MOTOR_3_FORWARD 4
#define PIN_MOTOR_3_REVERSE 5
#define PIN_MOTOR_4_FORWARD 2
#define PIN_MOTOR_4_REVERSE 3
// // Bump Sensors
// #define PIN_BUMP_FRONT 32
// #define PIN_BUMP_REAR 33
// // Ultrasonic Sensors
// #define PIN_US_FRONT_TRIG 18
// #define PIN_US_FRONT_ECHO 19
// #define PIN_US_REAR_TRIG 27
// #define PIN_US_REAR_ECHO 26
// // Infrared Sensors
// #define PIN_IR_1 15
// #define PIN_IR_2 17
// #define PIN_IR_3 14
// #define PIN_IR_4 16
// // WiFi Modules
// #define PIN_WIFI_TX 52
// #define PIN_WIFI_RX 50

void begin() {
  // Set Pinmodes

  // Motors
  pinMode(PIN_MOTOR_1_FORWARD, OUTPUT);
  pinMode(PIN_MOTOR_1_REVERSE, OUTPUT);
  pinMode(PIN_MOTOR_2_FORWARD, OUTPUT);
  pinMode(PIN_MOTOR_2_REVERSE, OUTPUT);
  pinMode(PIN_MOTOR_3_FORWARD, OUTPUT);
  pinMode(PIN_MOTOR_3_REVERSE, OUTPUT);
  pinMode(PIN_MOTOR_4_FORWARD, OUTPUT);
  pinMode(PIN_MOTOR_4_REVERSE, OUTPUT);

  // // Bump Sensors
  // pinMode(PIN_BUMP_FRONT, INPUT);
  // pinMode(PIN_BUMP_REAR, INPUT);

  // // Ultrasonic Sensors
  // pinMode(PIN_US_FRONT_TRIG, OUTPUT);
  // pinMode(PIN_US_FRONT_ECHO, INPUT);
  // pinMode(PIN_US_REAR_TRIG, OUTPUT);
  // pinMode(PIN_US_REAR_ECHO, INPUT);

  // // Infrared Sensors
  // pinMode(PIN_IR_1, INPUT);
  // pinMode(PIN_IR_2, INPUT);
  // pinMode(PIN_IR_3, INPUT);
  // pinMode(PIN_IR_4, INPUT);
};

void setMotorPWM(int motorId, int pwm) {
  // Inputs: motorId, an int [1, 4] to select the target motor
  //         pwm, the pwm of the motor, can be + or - to indicate direction

  // Verify and correct bounds on pwm value
  if (pwm > 255) {
    pwm = 255;
  } else if (pwm < -255) {
    pwm = -255;
  }

  // Verify motorId
  if (motorId < 1 || motorId > 4) {
    return;
  }

  int forward_pin;
  int reverse_pin;

  // Get pins to use
  if (motorId == 1) {
    forward_pin = PIN_MOTOR_1_FORWARD;
    reverse_pin = PIN_MOTOR_1_REVERSE;
  } else if (motorId == 2) {
    forward_pin = PIN_MOTOR_2_FORWARD;
    reverse_pin = PIN_MOTOR_2_REVERSE;
  } else if (motorId == 3) {
    forward_pin = PIN_MOTOR_3_FORWARD;
    reverse_pin = PIN_MOTOR_3_REVERSE;
  } else {
    forward_pin = PIN_MOTOR_4_FORWARD;
    reverse_pin = PIN_MOTOR_4_REVERSE;
  }

  // Set pin PWM and GND
  if (pwm >= 0) {
    analogWrite(forward_pin, abs(pwm));
    digitalWrite(reverse_pin, 0);
  } else {
    digitalWrite(forward_pin, 0);
    analogWrite(reverse_pin, abs(pwm));
  }
};

// sets all motor speeds to 0
void turnOffMotors() {
  setMotorPWM(1, 0);
  setMotorPWM(2, 0);
  setMotorPWM(3, 0);
  setMotorPWM(4, 0);
};

// sets motors 1 and 2 speeds to specified pwm
void setLeftMotorPWM(int pwm) {
  setMotorPWM(1, pwm);
  setMotorPWM(2, pwm);
};

// sets motors 3 and 4 speeds to specified pwm
void setRightMotorPWM(int pwm) {
  setMotorPWM(3, pwm);
  setMotorPWM(4, pwm);
};

// reads and returns distance in meters (maximum of 1) from specified ultrasonic
// distance sensor (only valid for sensorIds 1 or 7)
float readDistanceSensor(int sensorId) {
  // Verify sensorId
  if (sensorId != 1 && sensorId != 7) {
    return -1;
  }

  int trig_pin;
  int echo_pin;

  // Get pin to use
  if (sensorId == 1) {
    trig_pin = PIN_US_FRONT_TRIG;
    echo_pin = PIN_US_FRONT_ECHO;
  } else {
    trig_pin = PIN_US_REAR_TRIG;
    echo_pin = PIN_US_REAR_ECHO;
  }

  // Trigger and read the distance sensor
  digitalWrite(trig_pin, 0);
  delayMicroseconds(2);
  digitalWrite(trig_pin, 1);
  delayMicroseconds(10);
  digitalWrite(trig_pin, 0);
  int duration = pulseIn(echo_pin, 1);
  if (duration < 20000 && duration > 0) {
    return min(duration * .0001715, 1);
  } else
    return -1;
};

// reads and returns the state of the specified infrared sensor (only valid for
// sensorIds 1 through 4)
int readIrSensor(int sensorId) {
  // Verify sensorId
  if (sensorId < 1 || sensorId > 4) {
    return -1;
  }

  int sensor_pin;

  // Get pin to use
  if (sensorId == 1) {
    sensor_pin = PIN_IR_1;
  } else if (sensorId == 2) {
    sensor_pin = PIN_IR_2;
  } else if (sensorId == 3) {
    sensor_pin = PIN_IR_3;
  } else {
    sensor_pin = PIN_IR_4;
  }

  // Read and return the sensor state
  return digitalRead(sensor_pin);
};

// reads and returns the state of the specified infrared sensor (only valid for
// sensorIds 1 or 7)
int readBumpSensor(int sensorId) {
  // Verify sensorId
  if (sensorId != 1 && sensorId != 7) {
    return -1;
  }

  int sensor_pin;

  // Get pin to use
  if (sensorId == 1) {
    sensor_pin = PIN_BUMP_FRONT;
  } else {
    sensor_pin = PIN_BUMP_REAR;
  }

  // Read and return the sensor state
  return digitalRead(sensor_pin);
};