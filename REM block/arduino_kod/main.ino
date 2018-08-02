int gumb = 12;
int zujalica = 4;
int led = 8;
int flag;
//int gas = A0;
//float brzina = 0.0;

void setup() {
  pinMode(gumb, INPUT_PULLUP);
  pinMode(zujalica, OUTPUT);
  pinMode(led, OUTPUT);
  //pinMode(gas, INPUT);
  Serial.begin(9600);
}

void loop() {
  //brzina = analogRead(gas)/10.23;
  //Serial.println(brzina);
  while(Serial.available() > 0){
  
  
   flag = Serial.read();
    
   if(flag){
    digitalWrite(led, HIGH);
    while(digitalRead(gumb)==HIGH)
    {
      tone(zujalica, 50, 100);
      delay(200);
      tone(zujalica, 700, 100);
    }
      digitalWrite(led, LOW);
  }
  
    
  }
  
}

