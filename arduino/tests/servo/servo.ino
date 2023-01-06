 #include <Servo.h>

#define ELBOW_PIN 3
#define FIST_PIN 5
#define PICKER_PIN 6

const int ELBOW_START = 150;
const int FIST_START = 80;
const int PICKER_START = 130;

Servo elbow;
Servo fist;
Servo picker;

void pickerSetup() {
  elbow.write(ELBOW_START);
  fist.write(FIST_START);
  picker.write(PICKER_START);
}

void moveArm(Servo knuckle, int startPoint, int endPoint, int timeInterval) {  
  if(startPoint > endPoint) {
    for(int deg = startPoint; deg > endPoint; deg--) {
      knuckle.write(deg);
      delay(timeInterval);
    }
  } else {
    for(int deg = startPoint; deg < endPoint; deg++){
      knuckle.write(deg);
      delay(timeInterval);
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  elbow.attach(ELBOW_PIN);
  fist.attach(FIST_PIN);
  picker.attach(PICKER_PIN);
  
  pickerSetup();
}

void loop() {
   
}
