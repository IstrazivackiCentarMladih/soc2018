#define TRIG_RIGHT 2
#define ECHO_RIGHT 3

#define TRIG_LEFT  8
#define ECHO_LEFT  9

unsigned long ultraSensorLeft, ultraSensorRight;

void setup()
{
  Serial.begin (115200);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
}

void loop() {
  if (Serial.available() > 0) {
    ultraSensorRight = SonarSensor(TRIG_RIGHT, ECHO_RIGHT);
    delay(10);
    ultraSensorLeft  = SonarSensor(TRIG_LEFT , ECHO_LEFT );
    
    // Send data to RPi
    Serial.write(ultraSensorRight);
    Serial.write(ultraSensorRight >> 8);
    
    Serial.write(ultraSensorLeft);
    Serial.write(ultraSensorLeft >> 8);
    
    delay(20); // Delay read from reference manual
  }
}

unsigned long SonarSensor(int trigPin, int echoPin)
{
  unsigned long duration;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  return duration / 58;
}
