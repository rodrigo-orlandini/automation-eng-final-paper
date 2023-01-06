#include <Servo.h>

#define START_OF_THREAD_PIN 2
#define ELBOW_PIN 3
#define END_OF_THREAD_PIN 4
#define FIST_PIN 5
#define PICKER_PIN 6
#define MOTOR_GO_PIN 7
#define MOTOR_BACK_PIN 8
#define START_INDICATOR A0
#define PROCESS_INDICATOR A1

const int ELBOW_START = 90;
const int FIST_START = 0;
const int PICKER_START = 130;

Servo elbow;
Servo fist;
Servo picker;

int serialBuffer[4];
int serialData;
boolean readable = false;

int threadState = 0;
int lockState = 'I';
boolean lockFlow = true;

void pickerSetup() {
  elbow.write(ELBOW_START);
  fist.write(FIST_START);
  picker.write(PICKER_START);
}

void motorSetup() {
  while(digitalRead(START_OF_THREAD_PIN)) {
    digitalWrite(MOTOR_BACK_PIN, HIGH);
    delay(100);
  }

  digitalWrite(MOTOR_GO_PIN, LOW);
  digitalWrite(MOTOR_BACK_PIN, LOW);
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

  pinMode(START_OF_THREAD_PIN, INPUT);
  elbow.attach(ELBOW_PIN);
  pinMode(END_OF_THREAD_PIN, INPUT);
  fist.attach(FIST_PIN);
  picker.attach(PICKER_PIN);
  pinMode(MOTOR_GO_PIN, OUTPUT);
  pinMode(MOTOR_BACK_PIN, OUTPUT);
  
  pinMode(START_INDICATOR, OUTPUT);
  pinMode(PROCESS_INDICATOR, OUTPUT);
  
  pickerSetup();
  motorSetup();
}

void loop() {
  if(Serial.available() > 0) {
    serialData = Serial.read();
    
    for(int index = 0; index < 3; index++){
      serialBuffer[index] = serialBuffer[index + 1];
    }
    serialBuffer[3] = serialData;
    
    readable = true;
    lockFlow = false;
  }

  if(readable && 
    (serialBuffer[0] == '<' && serialBuffer[3] == '>') &&
    (serialBuffer[1] == 'T')) 
  {
    threadState = serialBuffer[2];    
    readable = false;
  }

  //START_OF_THREAD_PIN is normal-closed
  if(!digitalRead(START_OF_THREAD_PIN)) {
    if(threadState == 'R') {
      digitalWrite(MOTOR_GO_PIN, LOW);
      digitalWrite(MOTOR_BACK_PIN, LOW);
    }
      
    lockState = 'S';
    lockFlow = false;
  }

  //END_OF_THREAD_PIN is normal-closed
  if(!digitalRead(END_OF_THREAD_PIN)) {
    if(threadState == 'L') {
      digitalWrite(MOTOR_GO_PIN, LOW);
      digitalWrite(MOTOR_BACK_PIN, LOW);
    }
    
    lockState = 'E';
    lockFlow = false;
    Serial.write("<TS>");
    Serial.write("<TE>");
    delay(300);
  }

  if(digitalRead(START_OF_THREAD_PIN) && digitalRead(END_OF_THREAD_PIN)) {
    lockState = 'I';
  }
  
  if(!lockFlow && threadState == 'L' && lockState != 'E') {
    digitalWrite(MOTOR_GO_PIN, HIGH);
    digitalWrite(MOTOR_BACK_PIN, LOW);
  }

  if(!lockFlow && threadState == 'R' && lockState != 'S') {
    digitalWrite(MOTOR_GO_PIN, LOW);
    digitalWrite(MOTOR_BACK_PIN, HIGH);
  }
  
  if(!lockFlow && threadState == 'F') {
    digitalWrite(MOTOR_GO_PIN, LOW);
    digitalWrite(MOTOR_BACK_PIN, LOW);
    lockFlow = true;
    threadState = 0;

    if(lockState != 'S') {
      Serial.write("<TS>");
    }
  }

  if(!lockFlow && threadState == 'G') {
    digitalWrite(MOTOR_GO_PIN, LOW);
    digitalWrite(MOTOR_BACK_PIN, LOW);
    lockFlow = true;
    threadState = 0;

    moveArm(elbow, 90, 150, 20);
    moveArm(fist, 0, 80, 10);
    moveArm(picker, 130, 180, 10);
    delay(1000);
  
    moveArm(fist, 80, 0, 10);
    moveArm(elbow, 150, 90, 20);
    moveArm(fist, 0, 90, 10);
    moveArm(picker, 180, 130, 1);
    moveArm(fist, 90, 0, 10);
    delay(1000);
    
    Serial.write("<TS>");
  }
}
