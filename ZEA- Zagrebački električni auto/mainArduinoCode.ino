unsigned long joyL, joyD, prog=1500; //raw očitane vrijednosti kanala
unsigned long  jacina, slabost; //mappane vrijednosti joyL (gas) 
const int FwdPin = 9; //H-most pin za skretanje
const int BwdPin = 11; //H-most pin za skretanje
const int PinF = 6; //H-most pin za gas
const int PinB = 10; //H-most pin za gas
const int dPin = 4; //desna LEDica
const int lPin = 7; //lijeva LEDica
boolean dva,osam; //komunikacija s drugim arduinom
unsigned int v = 0; //kontrola periodicnog citanja treceg kanala

void setup() {
  //inicijalizacija pinova 3,5,12 za RC kanale; 2 i 8 za komunikaciju arduina
  pinMode(3, INPUT); //prvi kanal - gas
  pinMode(5, INPUT); //treci kanal - odabir programa
  pinMode(12, INPUT); //drugi kanal - skretanje
  pinMode(2, INPUT);
  pinMode(8, INPUT);
  pinMode(FwdPin, OUTPUT);
  pinMode(BwdPin, OUTPUT);
  pinMode(PinF, OUTPUT);
  pinMode(PinB, OUTPUT);
  pinMode(dPin, OUTPUT);
  pinMode(lPin, OUTPUT);
  digitalWrite(dPin,LOW);
  izbor();
}

//cita raw vrijednosti joyD, joyL i prog; poziva funkcije skretanje i gas;
void loop() {
  while(digitalRead(12));
  joyD = pulseIn(12, HIGH);
  if(prog<1200){
  while(digitalRead(3));
  joyL = pulseIn(3, HIGH);
  }
  skretanje();
  gas();
  }

//ceka odabir nacina rada
void izbor(){
  digitalWrite(lPin,HIGH);
  delay(200);
  while(prog<1800 && prog>1200){
    digitalWrite(lPin,!digitalRead(lPin));
    digitalWrite(dPin,!digitalRead(dPin));
    prog = pulseIn(5, HIGH);
    delay(200);}
  digitalWrite(lPin,LOW);
  digitalWrite(dPin,LOW);
}

//provjerava u kojem je nacinu rada auto
void gas(){
  //autonomni nacin rada - prati objekt koji se krece po pravcu
  if (prog>1800){
    dva = digitalRead(2);
    osam = digitalRead(8);
    if(osam){analogWrite(PinF,0);analogWrite(PinB,45);}
    else if(!dva){analogWrite(PinF,50);analogWrite(PinB,0);}
    else {analogWrite(PinF,0);analogWrite(PinB,0);
      if(millis()-v>1000){prog = pulseIn(5, HIGH);v=millis();}}
  }
  //manualni nacin rada - upravljanje gasom pomocu daljinskog uz sprjecavanje sudara
  else if (prog<1200){
    if (joyL<=1500 && joyL>=1480){jacina=0;analogWrite(PinF,0);analogWrite(PinB,0);}
    else if (joyL >= 1500){
      if(digitalRead(2)==0){jacina = map(joyL, 1500, 2005, 0, 200);analogWrite(PinF,jacina);analogWrite(PinB,0);}
      else {stoj();}}
    else {slabost = map(joyL, 1480, 995, 0, 200);analogWrite(PinF,0);analogWrite(PinB,slabost);
      if(millis()-v>1000){prog = pulseIn(5, HIGH);v=millis();}}}
  else{izbor();}
  }

void skretanje(){
  if (joyD < 1300){desno();}
  else if (joyD > 1650) {lijevo();}
  else{ravno();}
  }
  
void desno (){
  analogWrite(FwdPin,0);
  analogWrite(BwdPin,255);
  digitalWrite(dPin,1);
  digitalWrite(lPin,0);
}
void lijevo (){
  analogWrite(BwdPin,0);
  analogWrite(FwdPin,255);
  digitalWrite(lPin,1);
  digitalWrite(dPin,0);
}
void ravno(){
  analogWrite(BwdPin,0);
  analogWrite(FwdPin,0);
  digitalWrite(lPin,0);
  digitalWrite(dPin,0);
}

//krizno zaustavljanje pred objektom te kratka voznja unatrag
void stoj(){
  analogWrite(PinF,0);
  analogWrite(PinB,50);
  digitalWrite(lPin,1);
  digitalWrite(dPin,1);
  analogWrite(A0,200);
  delay (600);
  joyD = pulseIn(12, HIGH);
  skretanje();
  digitalWrite(lPin,0);
  digitalWrite(dPin,0);
  analogWrite(A0,0);
  delay(600);
  joyD = pulseIn(12, HIGH);
  skretanje();
  digitalWrite(lPin,1);
  digitalWrite(dPin,1);
  analogWrite(A0,200);
  delay(600);
  digitalWrite(lPin,0);
  digitalWrite(dPin,0);
  analogWrite(A0,0);
}


