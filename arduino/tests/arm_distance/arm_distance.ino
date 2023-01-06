 #include <Servo.h>

#define ELBOW_PIN 3
#define FIST_PIN 5
#define PICKER_PIN 6
#define MOTOR_GO_PIN 7
#define MOTOR_BACK_PIN 8

const int ELBOW_START = 150;
const int FIST_START = 80;
const int PICKER_START = 180;

Servo elbow;
Servo fist;
Servo picker;

void motorSetup() {
  digitalWrite(MOTOR_GO_PIN, LOW);
  digitalWrite(MOTOR_BACK_PIN, LOW);
}

void pickerSetup() {
  elbow.write(ELBOW_START);
  fist.write(FIST_START);
  picker.write(PICKER_START);
}

void setup() {
  Serial.begin(9600);
  
  elbow.attach(ELBOW_PIN);
  fist.attach(FIST_PIN);
  picker.attach(PICKER_PIN);
  pinMode(MOTOR_GO_PIN, OUTPUT);
  pinMode(MOTOR_BACK_PIN, OUTPUT);
  
  motorSetup();  
  pickerSetup();
}

void loop() {
  digitalWrite(MOTOR_GO_PIN, HIGH);
  digitalWrite(MOTOR_BACK_PIN, LOW);  
}
