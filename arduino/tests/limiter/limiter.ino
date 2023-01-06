#define START_OF_THREAD_PIN 2
#define END_OF_THREAD_PIN 4

void setup() {
  Serial.begin(9600);

  pinMode(START_OF_THREAD_PIN, INPUT);
  pinMode(END_OF_THREAD_PIN, INPUT);
}

void loop() {

  Serial.print("START_OF_THREAD_PIN: ");
  Serial.println(!digitalRead(START_OF_THREAD_PIN));
  Serial.print("END_OF_THREAD_PIN: ");
  Serial.println(!digitalRead(END_OF_THREAD_PIN));
  Serial.println("---");
  delay(2000);
}
