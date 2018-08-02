
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {

  if(Serial.available()){
    char a = (char) Serial.read();
    if(a == '1'){
        Serial.write("OK");
        digitalWrite(13, HIGH);
      }
    else{
        Serial.write("OK");
        digitalWrite(13, LOW);
      }
    }  

}
