#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int trig = 6;
const int echo = 7;
long vrijeme, udaljenost,y;

SoftwareSerial bt(10, 11);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, 2);

void setup() {
   
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(12, OUTPUT);
  strip.begin();
  strip.show();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  bt.begin(38400);
  Serial.begin(9600);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Mjerenje udaljenost s HC-SR04 senzorom");
  display.println("");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(3);
  
}
 
void loop() {
    display.clearDisplay();
    display.setCursor(0,0);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
   
    vrijeme = pulseIn(echo, HIGH); 
    udaljenost = (vrijeme/2) * 0.0340;
    display.print(udaljenost);   
    display.println(" cm");
    display.display();
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    
    delay(50);
   
    if(udaljenost>=15){
      bt.println("provalnik");
      tone(12,1000);
    }else{
      noTone(12);
    }
    if(udaljenost<40){
       y=map(udaljenost,2,40,0,255);
      strip.setPixelColor(1, 0, 0,0);
      strip.setPixelColor(2, 0, 0,0);
       strip.show();
      strip.setPixelColor(0, y, 0,0);
    }else if(udaljenost<80){
       y=map(udaljenost,40,80,0,255);
       strip.setPixelColor(2, 0, 0,0);
       strip.show();;
      strip.setPixelColor(1, y, 0,0);
    }
      else if(udaljenost<120){
         y=map(udaljenost,80,120,0,255);
        strip.setPixelColor(2, y, 0,0);
      }
      strip.show();
     
}


