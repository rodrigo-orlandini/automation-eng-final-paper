#include <Servo.h>

#define START_OF_THREAD_PIN 2
#define ELBOW_PIN 3
#define END_OF_THREAD_PIN 4
#define FIST_PIN 5
#define PICKER_PIN 6
#define MOTOR_BACK_PIN 7
#define MOTOR_GO_PIN 8
#define START_INDICATOR A0
#define PROCESS_INDICATOR A1

const int ELBOW_START = 90;
const int FIST_START = 0;
const int PICKER_START = 130;

Servo elbow;
Servo fist;
Servo picker;

int serialBuffer[14];
int serialData;
boolean readable = false;

int pickerState = PICKER_START;
int fistDeg = FIST_START;
int elbowDeg = PICKER_START;
int threadState = 0;

void pickerSetup() {
  elbow.write(ELBOW_START);
  fist.write(FIST_START);
  picker.write(PICKER_START);
}

void motorSetup() {
  digitalWrite(MOTOR_GO_PIN, LOW);
  digitalWrite(MOTOR_BACK_PIN, LOW);
  
  while(!digitalRead(START_OF_THREAD_PIN)) {
    digitalWrite(MOTOR_BACK_PIN, HIGH);
  }
}

void moveArm(Servo knuckle, int startPoint, int endPoint, int timeInterval) {  
  if(startPoint > endPoint) {
    for(int deg = startPoint; deg > endPoint; deg--){
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

  pinMode(START_OF_THREAD_PIN, INPUT_PULLUP);
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

void servos() {
  moveArm(elbow, 90, 120, 20);
  moveArm(fist, 0, 45, 10);
  moveArm(picker, 130, 180, 2);
  delay(2000);

  moveArm(elbow, 120, 90, 20);
  moveArm(fist, 45, 0, 10);
  moveArm(picker, 180, 130, 2);
  delay(2000);
}

void motor() {
  digitalWrite(MOTOR_GO_PIN, HIGH);
  digitalWrite(MOTOR_BACK_PIN, LOW);
  delay(2000);

  digitalWrite(MOTOR_GO_PIN, LOW);
  digitalWrite(MOTOR_BACK_PIN, HIGH);
  delay(2000);
}

void endOfThread() {
  if(digitalRead(START_OF_THREAD_PIN)) {
    digitalWrite(MOTOR_GO_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_GO_PIN, LOW);
  }
}

void serialCommunication() {
  if(Serial.available() > 0) {
    serialData = Serial.read();
    
    for(int index = 0; index < 13; index++){
      serialBuffer[index] = serialBuffer[index + 1];
    }
    serialBuffer[13] = serialData;
    
    readable = true;
  }

  if(readable && 
    (serialBuffer[0] == '<' && serialBuffer[13] == '>') &&
    (serialBuffer[1] == 'P' && serialBuffer[3] == 'F' && serialBuffer[7] == 'E' && serialBuffer[11] == 'T')) 
  {
    pickerState = serialBuffer[2] - 48;
    fistDeg = (serialBuffer[4] - 48) * 100 + (serialBuffer[5] - 48) * 10 + (serialBuffer[6] - 48);
    elbowDeg = (serialBuffer[8] - 48) * 100 + (serialBuffer[9] - 48) * 10 + (serialBuffer[10] - 48);
    threadState = serialBuffer[12] - 48;

    if(pickerState == 0) {
      Serial.println("GARRA: 130 (ABERTA)");
    } else {
      Serial.println("GARRA: 180 (FECHADA)");
    }

    Serial.print("PUNHO: ");
    Serial.println(fistDeg);

    Serial.print("COTOVELO: ");
    Serial.println(elbowDeg);

    if(threadState == 1) {
      Serial.println("MOTOR: 1 (VAI)");
    } else if(threadState == 2) {
      Serial.println("MOTOR: 2 (VOLTA)");
    } else {
      Serial.println("MOTOR: 0 (DESLIGA)");
    }
    
    readable = false;
  }
}

void loop() {
  serialCommunication();
}
