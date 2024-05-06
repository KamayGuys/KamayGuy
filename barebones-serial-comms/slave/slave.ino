// slave.ino

int first, second, third;

void setup() { 
  Serial.begin(38400); 
}

void loop() {
  if (Serial.available() == 3) {
    first = Serial.read(); 
    second = Serial.read();
    third = Serial.read();
    Serial.print("Message: ");
    Serial.print(first);
    Serial.print(second);
    Serial.println(third);
  }
}
