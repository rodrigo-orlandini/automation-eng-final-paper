 #include <Servo.h>

#define ELBOW_PIN 3
#define FIST_PIN 5
#define PICKER_PIN 6

const int ELBOW_START = 90;
const int FIST_START = 0;
const int PICKER_START = 130;

Servo elbow;
Servo fist;
Servo picker;

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
  
  pickerSetup();
}

void loop() {
  delay(5000);
  elbow.write(150);
  fist.write(80);
  delay(1000);
  picker.write(180);
}
