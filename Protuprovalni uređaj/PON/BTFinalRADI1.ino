//  by Brandon Plumbo AKA Riftliger
//  original version from Martyn Currey http://www.martyncurrey.com/
//  HC-05 universal test sketch to enter AT-commands
//  Uses hardware serial to talk to the host computer and software serial for communication with the bluetooth module
//
//  Pins
//  BT VCC to Arduino 5V out.
//  BT GND to GND
//  Arduino D11 to BT RX
//  Arduino D10 BT TX
//
//  When a command is entered in the serial monitor on the computer
//  the Arduino will relay it to the bluetooth module and display the result.
//
//  The HC-05 modules require both CR and NL

//######################################################    KONSTANTE    #########################################
#define BT_AT_SET_PIN 7
#define BT_VCC_PIN 5
#define BT_BAUDRATE 38400
#define BT_SOFT_UART_TX 10
#define BT_SOFT_UART_RX 11
#define BT_DEVICE_COUNT_MAX 3
//KONSTANTE ZA RSSI
#define KONTROLPIN 13
#define BAZA 65470
#define TRIG 6
#define ECHO 7
#define SERIAL_BAUD 2000000
#define ADDRSIZE 12
#define INQM_PAR 1
//######################################################    KONSTANTE    #########################################
#include <EEPROM.h>
#include <SoftwareSerial.h>
//#include <QuickStats.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
uint32_t vrijeme, udaljenost, y;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, 2);


typedef struct {
  char address[12];
  uint16_t rssi;
} bt_data;

typedef struct{
  char address[12];
} atom;

bt_data INQCheck;
atom DeviceList[BT_DEVICE_COUNT_MAX];

// VIDJETI KASNIJE bt_data PoljeUredajaBT[BT_DEVICE_COUNT_MAX];

SoftwareSerial BTSerial(BT_SOFT_UART_TX, BT_SOFT_UART_RX); //  TX,RX

//######################################################    DEKLARACIJE FUNKCIJA    #########################################
String BTReadOK();
void setBT_AT_mode();
void halt();
void PrintWthFeed(String a);
void initBT();
void BT_AT_INQ();
void BT_AT_INQData();
void BTReturn();
void BTprintR(String a);
void BTRead_INQ();
bool je_blizu(bt_data* BT, String* known_device_address);
void printDeviceInfo(bt_data* data);
bool Inquire(uint8_t index, char CmpAddr[]);
void PrintAllDevices();
//######################################################    DEKLARACIJE FUNKCIJA    #########################################

void setup()
{
  Serial.begin(SERIAL_BAUD);
  initBT();
  //###################################   INIT ZAVRSEN - ISPOD OVOGA DIJELA PISATI PRAVE PROGRAME   ############################################
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(12, OUTPUT);
  strip.begin();
  strip.show();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Mjerenje udaljenost s HC-SR04 senzorom");
  display.println("");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(3);

  BTSerial.flush();
  BTRead_INQ();
  PrintAllDevices();
  for(int i = 0; i < 12; i++){    
    EEPROM.update(i, INQCheck.address[i]);
  }
  //Adresa zapisana u EEPROM!
//  Serial.println(DeviceList[0].rssi);
//  for(int i=0;i<BT_DEVICE_COUNT_MAX;i++){
//    Serial.print("nekaj");
//    EEPROMCharWr(DeviceList[i].address, ADDRSIZE);
//  }
}

void loop()
{
  uint8_t NL = 2;
//    BTRead_INQ();
//    PrintAllDevices();
  char ADDR[13];
  for(int i = 0; i < 12; i++){
    char a = EEPROM.read(i);
    ADDR[i] = a;
  }
  ADDR[12] = '\0';
  Serial.print(ADDR);
  
  display.clearDisplay();
  display.setCursor(0, 0);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  vrijeme = pulseIn(ECHO, HIGH);
  udaljenost = (vrijeme / 2) * 0.0340;
  display.print(udaljenost);
  display.println(" cm");
  display.display();
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  delay(50);


  //Dio gdje mjerimo udaljenost
  if (udaljenost >= 15) {
    //NL = (NL+1)%3;
    if((NL == 2 && !Inquire(ADDR))){
      tone(12, 1000);
      Serial.println("Pistim");
    }
  } else {
    noTone(12);
  }
  NL = (NL+1)%3;
  
  //printDeviceInfo(DeviceList);

  
  if (udaljenost < 40) {
    y = map(udaljenost, 2, 40, 0, 255);
    strip.setPixelColor(1, 0, 0, 0);
    strip.setPixelColor(2, 0, 0, 0);
    strip.show();
    strip.setPixelColor(0, y, 0, 0);
  } else if (udaljenost < 80) {
    y = map(udaljenost, 40, 80, 0, 255);
    strip.setPixelColor(2, 0, 0, 0);
    strip.show();
    strip.setPixelColor(1, y, 0, 0);
  }
  else if (udaljenost < 120) {
    y = map(udaljenost, 80, 120, 0, 255);
    strip.setPixelColor(2, y, 0, 0);
  }
  strip.show();

}
//####################################################################        FUNKCIJE        ########################################################################################

//void EEPROMCharWr(char zapisati[], int n){
//    char a[12];
//    int i,y;
//    uint8_t DataIndex=EEPROM.read(0);
//        for(y=0;y<=DataIndex;y++){
//                for(i = DataIndex*n+1; i <= n*DataIndex; i++){
//                    a[i]=(char)EEPROM.read(i);
//                  
//                }
//                if(strncmp(a,zapisati,12)) return;
//                Serial.println();
//        }
//            Serial.print("yay");
//    for(i = DataIndex*n+1; i <= n*DataIndex; i++){
//        EEPROM.update(i, zapisati[i-n*DataIndex-1]);
//    }
//    EEPROM.update(0, ++DataIndex);
//}


//OVA FJA OKEJ
void BTRead_INQ() {
  String output;
  char a;

  int i = 0;
  output = "AT+INQ\r\n";
  //PrintWthFeed(&output);
  BTSerial.print(output);
  output="";

  do {
    while (!BTSerial.available());
    a = (char) BTSerial.read();
    String help = "Ispis a: ";
    //Serial.println(help + int(a));
    
    if(i != 0){
      if (a != ':'){
        output += a;
        i++;
      }
    } else{
      if(a == '\r' || a == '\n'){
        continue;
      } else{
        output += a;
        i++;    
      }
    }
  } while (output[i - 1] != 'K' && output[i - 2] != 'O');

    
    const char *output_c = output.c_str();
    strncpy(INQCheck.address, &(output_c[4]), 12);
    INQCheck.rssi = strtol(&(output_c[24]), 0, 16);
//    for (i = 0; i < BT_DEVICE_COUNT_MAX; i++) {
//        strncpy(INQCheck.address, &(output_c[i * 30 + 8]), 12);
//        //strncpy(DeviceList[i].rssi, &(output_c[i * 30 + 26], 4);
//        INQCheck.rssi = strtol(&(output_c[i * 30 + 26]), 0, 16);
//    }
}

//OVA FJA OKEJ
void PrintAllDevices(){
    for (int i = 0; i < BT_DEVICE_COUNT_MAX; i++) {
        printDeviceInfo(&INQCheck);
  }
}

//OVA FJA OKEJ
void printDeviceInfo(bt_data* data) {           //Printira informacije o trenutnom deviceu
  Serial.print("ADDR:");
  for (int k = 0; k < 12; ++k) {
    Serial.print((*data).address[k]);
  }
  Serial.println();
  Serial.print("RSSI:");
  Serial.println((long int) (*data).rssi);
}

//OVA FJA MOZDA OKEJ (???)
bool je_blizu(bt_data* BT, String *known_device_address) {   //Vraca 1 ukoliko se detektirana adresa preklapa s trazenom. Zasada cemo imati samo 1 aktivnu adresu koju usporedujemo, kasnije nadograditi na vise
  String device_addr;
  for (int i = 0; i < 12; i++)
    device_addr += (*BT).address[i];
  Serial.println(*known_device_address);
  Serial.println(device_addr);
  return known_device_address->equals(device_addr);
}

//OVA FJA OKEJ
bool Inquire(char CmpAddr[]) {
  BTRead_INQ();
  if (strncmp(INQCheck.address, CmpAddr, 12)){
    Serial.println("Nisam prava adresa!");
    return false;
  }
  if (INQCheck.rssi < BAZA) {
    return false;
  }
  return true;
}


String BTReadOK() {
  String output;
  char a;//, prev = 0;

  //int i = 0;
  while(!BTSerial.available());
  
  while(BTSerial.available()){
    //prev = a;
    a = (char) BTSerial.read();
//    Serial.print("Printam a: ");
//    Serial.println(a);
    output += a;
    //i++;
  }//while((output[i - 1] != 'K' && output[i - 2] != 'O') && (output[i-1] != ')'));

  //output += "\r\n";
  return output;
}


void setBT_AT_mode() {
  digitalWrite(BT_AT_SET_PIN, HIGH);
  delay(700);
  String cmd = "AT+ROLE=1\r\n";
  BTprintR(&cmd);
}

//OVA FJA OKEJ
void PrintWthFeed(String *a) {
  Serial.print('>');
  Serial.print(*a);
  BTSerial.print(*a);
}

void initBT() {
  pinMode(BT_AT_SET_PIN, OUTPUT);
  pinMode(BT_VCC_PIN, OUTPUT);
  digitalWrite(BT_VCC_PIN, HIGH);
  BTSerial.begin(BT_BAUDRATE);
  Serial.println("BTInit");

  setBT_AT_mode();
  //delay(2000);

  String cmd = "AT\r\n";
  BTprintR(&cmd);
  
  cmd = "AT+INIT\r\n";
  BTprintR(&cmd);
  
  cmd = "AT+INQM=1," + String(INQM_PAR) + ",5\r\n";
  BTprintR(&cmd);

  Serial.println("Init gotov!");
  Serial.println();
}

//void BTReturn() {
//  Serial.print(BTReadOK());
//}


void BTprintR(String* a) {
  PrintWthFeed(a);
  Serial.print(BTReadOK());
}

