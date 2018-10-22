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
//######################################################    KONSTANTE    #########################################

#include <SoftwareSerial.h>
#include <QuickStats.h>
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
  char address[13];
  uint16_t rssi;
} bt_data;

bt_data DeviceList[BT_DEVICE_COUNT_MAX];



// VIDJETI KASNIJE bt_data PoljeUredajaBT[BT_DEVICE_COUNT_MAX];

SoftwareSerial BTSerial(BT_SOFT_UART_TX, BT_SOFT_UART_RX); //  TX,RX
bt_data tmp[BT_DEVICE_COUNT_MAX];
char ADDR[13] = "446EE5E5504D";
//######################################################    DEKLARACIJE FUNKCIJA    #########################################
String BTReadOK();
void setBT_AT_mode();
void setBT_CommunicationMode();
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
void Inquire(uint8_t index, char CmpAddr[12]);
//######################################################    DEKLARACIJE FUNKCIJA    #########################################

void setup()
{
  pinMode(KONTROLPIN, OUTPUT);
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
}

void loop()
{

  Inquire(0, ADDR);
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

  if (udaljenost >= 15) {
    tone(12, 1000);
  } else {
    noTone(12);
  }
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


void BTRead_INQ() {
  String output = "";
  char a;

  int i = 0;
  BTSerial.print("AT+INQ\r\n");
  do {
    while (!BTSerial.available());
    a = (char) BTSerial.read();
    output += a;
    i++;
  } while (output[i - 1] != 'K' && output[i - 2] != 'O');


  for (i = 0; i < BT_DEVICE_COUNT_MAX; i++) {

    String device_addr = output.substring(i * 33 + 7, i * 33 + 21);
    device_addr.remove(4, 1);
    device_addr.remove(6, 1);
    strncpy(DeviceList[i].address, device_addr.c_str(), 13);
    //Serial.println(device_addr.c_str());


    String rssi = output.substring(i * 33 + 29, i * 33 + 33);

    uint16_t signalStrength = strtol(rssi.c_str(), 0, 16);
    DeviceList[i].rssi = signalStrength;

  }


  for (i = 0; i < BT_DEVICE_COUNT_MAX; i++) {

    printDeviceInfo(&(DeviceList[i]));
  }
}

void printDeviceInfo(bt_data* data) {           //Printira informacije o trenutnom deviceu
  Serial.print("ADDR:");
  for (int k = 0; k < 12; ++k) {
    Serial.print((*data).address[k]);
  }
  Serial.println();
  Serial.print("RSSI:");
  Serial.println((long int) (*data).rssi);
}

bool je_blizu(bt_data* BT, String *known_device_address) {   //Vraca 1 ukoliko se detektirana adresa preklapa s trazenom. Zasada cemo imati samo 1 aktivnu adresu koju usporedujemo, kasnije nadograditi na vise
  String device_addr;
  for (int i = 0; i < 12; i++)
    device_addr += (*BT).address[i];
  Serial.println(*known_device_address);
  Serial.println(device_addr);
  return known_device_address->equals(device_addr);
}

void Inquire(uint8_t index, char CmpAddr[]) {
  BTRead_INQ();
  if (strcmp(DeviceList[index].address, CmpAddr)) {
    Serial.println("Nisam prava adresa!");
    return;
  }
  if (DeviceList[index].rssi < BAZA) {
    return;
  }
}

String BTReadOK() {
  String output;
  char a;

  int i = 0;
  do {
    while (!BTSerial.available());
    a = (char) BTSerial.read();
    output += a;
    i++;
  } while (output[i - 1] != 'K' && output[i - 2] != 'O');

  output += "\r\n";
  return output;
}

void setBT_AT_mode() {
  digitalWrite(BT_AT_SET_PIN, HIGH);
  delay(700);
  PrintWthFeed("AT+ROLE=1\r\n");
  BTReturn();
}

void halt() {
  while (1);
}

void PrintWthFeed(String a) {
  Serial.print('>');
  Serial.print(a);
  BTSerial.print(a);
}

void initBT() {
  pinMode(BT_AT_SET_PIN, OUTPUT);
  pinMode(BT_VCC_PIN, OUTPUT);
  digitalWrite(BT_VCC_PIN, HIGH);
  BTSerial.begin(BT_BAUDRATE);
  Serial.println("BTInit");

  setBT_AT_mode();
  //delay(2000);

  PrintWthFeed("AT\r\n");
  BTReturn();
  PrintWthFeed("AT+INIT\r\n");
  BTReturn();
  PrintWthFeed("AT+INQM=1," + String(BT_DEVICE_COUNT_MAX) + ",5\r\n");
  BTReturn();
}

void BTReturn() {
  Serial.print(BTReadOK());
}

void BTprintR(String a) {
  PrintWthFeed(a);
  BTReturn();
}

