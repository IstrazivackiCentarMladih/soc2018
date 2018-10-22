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
#define BT_AT_SET_PIN 7
#define BT_VCC_PIN 5
#define BT_BAUDRATE 38400
#define BT_SOFT_UART_TX 10
#define BT_SOFT_UART_RX 11
#define BT_DEVICE_COUNT_MAX 9
#define LEDPIN 13
#define BAZA 65466

#include <SoftwareSerial.h>
#include <QuickStats.h>

typedef struct {
  char address[12];
  uint16_t rssi;
} bt_data;

bt_data DeviceList[BT_DEVICE_COUNT_MAX];



// VIDJETI KASNIJE bt_data PoljeUredajaBT[BT_DEVICE_COUNT_MAX];

SoftwareSerial BTSerial(BT_SOFT_UART_TX, BT_SOFT_UART_RX); //  TX,RX
char c = ' ';
boolean NL = true;
bt_data tmp[BT_DEVICE_COUNT_MAX];

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
//void IzvuciRSSI();
void BTRead_INQ();


void setup()
{
  Serial.begin(9600);
  initBT();
  Serial.println("Sketch HC-05");
  //###################################   INIT ZAVRSEN - ISPOD OVOGA DIJELA PISATI PRAVE PROGRAME   ############################################

  //PrintWthFeed("AT+INQ\r\n"); // get list of nearby devices  -  to radi
  // BTReturn();

  //  tmp = BTRead_INQ();
  //  Serial.println(tmp);

  //Serial.println(1);
  BTRead_INQ();
  je_blizu(DeviceList, "3CF7A49B1DF2");  

//  long int SignalStatus;
//  long int BazniSignal = 
//  for(i = 0; i < BT_DEVICE_COUNT_MAX; i++){
//    if(DeviceList[i].rssi >= "  
//  }
//  
  //Serial.println(100);
  // BT_AT_INQ();

  Serial.println("Gotov odsjecak koda!");
  //setBT_CommunicationMode();

  
  halt();
}

void loop()
{

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
}
//####################################################################        FUNKCIJE        ########################################################################################

//Treba mi funkcija koja odvaja podatke u strukturu, a zatim ju ispisuje
void BTRead_INQ(){
  String output = "";
  char a;
  bt_data HelpArray[BT_DEVICE_COUNT_MAX];


  Serial.println(2);
  int i = 0;
  BTSerial.print("AT+INQ\r\n");
  do{
    //Serial.println(2.1);
    while(!BTSerial.available());
    //Serial.println(2.2);
    a = (char) BTSerial.read();
    //Serial.print(a);
    output += a;
    i++;
    //Serial.print(a);
    //Serial.println(output);
  }while(output[i-1] != 'K' && output[i-2] != 'O');
  
  Serial.println(output);
  /*SADA IMAMO KOMPLETAN STRING, POTREBNO ODVOJITI
  U STRUKTURU, SVE DOK NE DODEMO DO OK(END OF TRANSMISSION ZNAK DE FACTO)
  TO NAPRAVITI POMOCU POLJA, KOJE ZATIM VRACAMO, I ISPISUJEMO REDAK PO
  REDAK U GL. PROGRAMU ZA TEST
  */
  Serial.println(4);
    
   
    
    for(i = 0; i < BT_DEVICE_COUNT_MAX; i++){
        int j = 0;

        String device_addr = output.substring(i*33+7, i*33+21);
        device_addr.remove(4, 1);
        device_addr.remove(6, 1);
        strncpy(DeviceList[i].address, device_addr.c_str(), 12);
        
//        for(int k = 0; k < 12; ++k){
//            Serial.print(DeviceList[i].address[k]);
//        }
//        Serial.println(" ");
//        
//        Serial.print("device no.");
//        Serial.print(i+1);
//        Serial.print(" :");
//        Serial.println(device_addr);

        String rssi = output.substring(i*33+29, i*33+33);
        //Serial.println(rssi);

        uint16_t signalStrength = strtol(rssi.c_str(), 0, 16);
        DeviceList[i].rssi = signalStrength;
//        do{
//            DeviceList[i].address[j-6] = output[j++];
//        } while(j >= 6 && j <= 19);
//        
//        do{
//            DeviceList[i].rssi[j-28] = output[j++];
//        } while(j >= 28 && output[j]!='\n');
    }


    
//  for(i = 0; i < BT_DEVICE_COUNT_MAX; i++){
//    int j = 0;
//    do{
//      HelpArray[i].address[j-6] = output[j++];
//    } while(j >= 6 && j <= 19);
//    do{
//      HelpArray[i].rssi[j-28] = output[j++];
//    } while(j >= 28 && output[j]!='\n');
//  }
  Serial.println(6);
  //return HelpArray;
  for(i = 0; i < BT_DEVICE_COUNT_MAX; i++){
    //Serial.println("Adresa: %s. Jacina signala: %s", HelpArray[i].address, HelpArray[i].rssi);
    printDeviceInfo(&(DeviceList[i]));
    }
   Serial.println(7);
}

void printDeviceInfo(bt_data* data){
    Serial.print("device address: ");
    for(int k = 0; k < 12; ++k){
            Serial.print((*data).address[k]);
        }
    Serial.println();
    Serial.print("device signal strength: ");
    Serial.println((long int) (*data).rssi);
}

bool je_blizu(bt_data* BT, String known_device_address){
   String device_addr;
   for(int i = 0; i < 12; i++)
    device_addr += (*BT).address[i];
   Serial.println(known_device_address);
   Serial.println(device_addr);
   return known_device_address.equals(device_addr);
}


String BTReadOK(){
  String output;
  char a;

  int i = 0;
  do{
    while(!BTSerial.available());
    a = (char) BTSerial.read();
    output += a;
    i++;
  }while(output[i-1] != 'K' && output[i-2] != 'O');
  
  output+="\r\n";
  return output;
}

void setBT_AT_mode(){
  digitalWrite(BT_AT_SET_PIN, HIGH);
  delay(700);
  PrintWthFeed("AT+ROLE=1\r\n");
  BTReturn();
}

void setBT_CommunicationMode(){
  digitalWrite(BT_AT_SET_PIN, HIGH);
  delay(700);
  PrintWthFeed("AT+ROLE=0\r\n");
  BTReturn();
  digitalWrite(BT_AT_SET_PIN, LOW);  
  digitalWrite (BT_VCC_PIN, LOW);
  digitalWrite (BT_VCC_PIN, HIGH);
}

void halt(){
  while(1);  
}

void PrintWthFeed(String a){
  Serial.print('>');
  Serial.print(a);
  BTSerial.print(a);
}

void initBT(){
  pinMode(BT_AT_SET_PIN, OUTPUT);
  pinMode(BT_VCC_PIN, OUTPUT);
  digitalWrite(BT_VCC_PIN, HIGH);
  BTSerial.begin(BT_BAUDRATE);
  Serial.println("BTserial started at 38400");
  
  setBT_AT_mode();
  delay(2000);
  
  PrintWthFeed("AT\r\n");
  BTReturn();
  PrintWthFeed("AT+INIT\r\n");
  BTReturn();
  PrintWthFeed("AT+INQM=1,9,5\r\n");
  BTReturn();
}

/* FJ-A PRIMA STRUKTURU, ISPISUJE NA SERIJU PRIMLJENE PODATKE*/
void BT_AT_INQ(){
  String input;
  
  PrintWthFeed("AT+INQ\r\n");
  input = BTReadOK();
  Serial.print(input);
}

/*
void BT_AT_INQData{
  bt_data HelpArray[BT_DEVICE_COUNT_MAX];

  &TmpArray = BT_ReadINQ();
  for(i = 0; i < BT_DEVICE_COUNT_MAX; i++){
    Serial.println("%s  %s", TmpArray.address, TmpArray.rssi);
  }
  
}
*/

void BTReturn(){
  Serial.print(BTReadOK());
}

void BTprintR(String a){
  PrintWthFeed(a);
  BTReturn();
}

/* NAPOMENE
--Kasnije promijeniti funkciju za print, da radi sa characterima
*/

