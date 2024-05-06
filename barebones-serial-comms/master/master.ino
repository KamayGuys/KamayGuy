// master.ino

int state = 0;

void setup() { 
  Serial.begin(38400); 
}

// bruh
void loop() {
  Serial.write(0);
  Serial.write(0);
  Serial.write(7);
  delay(100);
}
