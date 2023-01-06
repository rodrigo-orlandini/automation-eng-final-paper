#define MOTOR_GO_PIN 7
#define MOTOR_BACK_PIN 8

void motorSetup() {
  digitalWrite(MOTOR_GO_PIN, LOW);
  digitalWrite(MOTOR_BACK_PIN, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_GO_PIN, OUTPUT);
  pinMode(MOTOR_BACK_PIN, OUTPUT);
  
  motorSetup();
}

void loop() {
  digitalWrite(MOTOR_GO_PIN, HIGH);
  digitalWrite(MOTOR_BACK_PIN, LOW);
  delay(5000);
}
