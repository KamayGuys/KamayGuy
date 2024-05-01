int state = 20;
int message = 0;

void setup() { Serial.begin(38400); }

void loop() {
  if (Serial.available() > 0) {
    message = Serial.read();
    Serial.println(message);
  }
  Serial.write(1234);
  delay(1000);
}