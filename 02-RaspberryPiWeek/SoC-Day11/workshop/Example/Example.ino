void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  pinMode(13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:


    if(Serial.available() > 0){
        char procitaniZnak = (char) Serial.read();

        if(procitaniZnak == '1'){
            digitalWrite(13, HIGH); // palimo LED-icu
            Serial.write("LED-ica upaljena");
        }
        else{
            digitalWrite(13, LOW); // gasimo LED-icu
            Serial.write("LED-ica ugasena");
        }
    }
}
