const int comPin1 = 2; //prvi pin za komunikaciju
const int comPin2 = 8; //drugi pin za komunikaciju
const int trigPin = 4; //ultrasonični senzor
const int echoPin = 5; //ultrasonični senzor
unsigned long x; //procesuirana vrijednost sa senzora

const int vel = 11; //duljina liste iz koje se uzima medijan

void setup() {
  pinMode(comPin1, OUTPUT);
  pinMode(comPin2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  x = med();
  //prepreka je udaljenija od 50 cm
  if (x > 50){
    digitalWrite(comPin1, LOW);
    digitalWrite(comPin2, LOW);
  }
  //prepreka je udaljena 20 - 50 cm
  else if (x > 20) {
      digitalWrite(comPin2, LOW);
      digitalWrite(comPin1, HIGH);}
  //prepreka je udaljena manje od 20 cm
  else {
    digitalWrite(comPin1, HIGH);
    digitalWrite(comPin2, HIGH);
  }
}

//vraća udaljenost senzora od prepreke u cm
int cm() {
    unsigned long duration;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    while(digitalRead(echoPin) == LOW);
    
    unsigned long pocetak = micros();
    while((digitalRead(echoPin) == HIGH) && ((micros() - pocetak) < 5500));
    duration = micros() - pocetak;
    
    unsigned long dcm = (duration/2) / 29.1;
    delayMicroseconds(65000-duration);
    return dcm;
}

//stvara listu od 'vel' očitanih vrijednosti, sortira ju i vraća srednji element, odnosno medijan te liste
int med() {
  int lista[vel];
  for (int i=0;i<vel;i++){
    lista[i]=cm();
  }
  sort(lista,vel);
  return lista[(vel-1)/2];
}
void sort(int a[], int vel) {
    for(int i=0; i<(vel-1); i++) {
        for(int o=0; o<(vel-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}
