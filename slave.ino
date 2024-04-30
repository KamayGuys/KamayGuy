int state = 20;
int buttonState = 0;
int message = 0;

void setup() { Serial.begin(38400); }

void loop() {
  if (Serial.available() > 0) {
    state = Serial.read();
    Serial.println(state);
  }
  Serial.write(1234);
  delay(1000);
}