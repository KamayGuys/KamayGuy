#define ledPin 9

int state = 0;
int potValue = 0;

void setup() { Serial.begin(38400); }

void loop() {
  if (Serial.available() > 0) {
    state = Serial.read();
    Serial.println(state);
  }
  Serial.write(42);
  delay(1000);
}