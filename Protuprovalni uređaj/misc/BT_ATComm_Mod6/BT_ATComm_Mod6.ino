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

#include <SoftwareSerial.h>
#include <QuickStats.h>
#define BT_AT_SET_PIN 7
#define BT_VCC_PIN 5
#define BT_BAUDRATE 38400
#define BT_SOFT_UART_TX 10
#define BT_SOFT_UART_RX 11
#define BT_DEVICE_COUNT_MAX 9

SoftwareSerial BTSerial(10, 11); //  TX,RX

char c = ' ';
boolean NL = true;
char kar;
String output;
boolean ATStatus = false;

void initBT();
void setBT_AT_mode();
void setBT_CommunicationMode();
void halt();
void PrintWthFeed(String a);
void BTReturn();
void BTprintR(String a);
String RdSerial();
void PrntSerial(String input);

void setup()
{
  Serial.begin(9600);
  //Set to HC-05 default baud rate, found using AT+UART.  It is usually 38400.
  initBT();
  setBT_CommunicationMode();
}

void loop()
{
  //delay(5);
  output = RdSerial();

  if (output == "ATMode\r\n") {
    PrntSerial(output);
    setBT_AT_mode();
    ATStatus = true;
  } else if (output == "CommMode\r\n") {
    PrntSerial(output);
    setBT_CommunicationMode();
    ATStatus = false;
  } else {
    if (ATStatus)
      BTprintR(output);
    else{
      Serial.println("BT je u Comm modu, nema smisla slati AT komande!");
      PrintWthFeed(output);
      BTSerial.flush();
    }
  }

  /*
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (BTSerial.available())
    {
    c = BTSerial.read();
    Serial.write(c);
    }


    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
    c = Serial.read();
    BTSerial.write(c);

    // Echo the user input to the main window. The ">" character indicates the user entered text.
    if (NL) {
      Serial.print(">");
      NL = false;
    }
    Serial.write(c);
    if (c == 10) {
      NL = true;
    }
    }
  */
}

void setBT_CommunicationMode() {
  digitalWrite(BT_AT_SET_PIN, HIGH);
  delay(700);
  PrintWthFeed("AT+ROLE=0\r\n");
  BTReturn();
  digitalWrite(BT_AT_SET_PIN, LOW);
  digitalWrite (BT_VCC_PIN, LOW);
  delay(2000);
  digitalWrite (BT_VCC_PIN, HIGH);
}

void setBT_AT_mode() {
  digitalWrite(BT_AT_SET_PIN, HIGH);
  delay(700);
  PrintWthFeed("AT+ROLE=1\r\n");
  BTReturn();
}

void initBT() {
  pinMode(BT_AT_SET_PIN, OUTPUT);
  pinMode(BT_VCC_PIN, OUTPUT);
  digitalWrite(BT_VCC_PIN, HIGH);
  BTSerial.begin(BT_BAUDRATE);
  Serial.println("BTserial started at 38400");
  Serial.println("Sketch HC-05");
  Serial.println("Arduino with HC-05 is ready");
  Serial.println("Make sure Both NL & CR are set");
  Serial.println("");
}

void BTprintR(String a) {
  PrintWthFeed(a);
  BTReturn();
}

void BTReturn() {
  Serial.print(BTReadOK());
}

void PrintWthFeed(String a) {
  Serial.print('>');
  Serial.print(a);
  BTSerial.print(a);
}

/*
String BTReadOK() {
  String output = "";
  char a;

  int i = 0;
  
  do {
    while (!BTSerial.available());
    a = (char) BTSerial.read();
    output += a;
    i++;
  } while (!strstr((char*)output, "OK\r\n") && !strstr((char*)output, "ERROR") && !strstr((char*)output, "FAIL"));/*(output[i - 1] != 'K' && output[i - 2] != 'O') && (output[i - 1] != 'K' && output[i - 2] != 'O')*\/);
  output+=("\r\n");
  return output;
}
*/

String BTReadOK(){
  String output = "";
  char a;

  int i = 0;
  do{
    while(!BTSerial.available());
    a = (char) BTSerial.read();
    output += a;
    i++;
    delay(5);
  } while(output[i-1]!='K' && output[i-2]!='O');
  output+="\r\n";
  
  return output;
}

void halt() {
  while (1);
}

/*
  String RdSerial(){
  String output="";
  char kar;

  if(Serial.available()){
    while(Serial.available()){
      kar = (char) Serial.read();
      output += kar;
    }
  }

  return output;
  }
*/

String RdSerial() {
  String output = "";
  char kar;

  int i = 0;
  do {
    while (!Serial.available());
    kar = (char) Serial.read();
    output += kar;
    i++;
  } while (output[i - 1] != '\n' && output[i - 2] != '\r');

  return output;
}

void PrntSerial(String input) {

  Serial.print('>');
  int i = 0;
  do {
    Serial.print(input[i++]);
  } while (input[i - 1] != '\n');
}


