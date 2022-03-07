#include <LiquidCrystal.h>
#include <IRremote.h>


const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define PIN_IR 3
#define PIN_DETECT 2
#define PIN_STATUS_LED 6


#define irLED 5

IRsend irsend;
void setup()
{

  lcd.begin(16, 2);
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("Custom Flash");

  
  pinMode(PIN_DETECT, INPUT);
  pinMode(PIN_STATUS_LED, OUTPUT);
  irsend.enableIROut(38);
  irsend.mark(0);
}

void loop() {
  irsend.space(0);
  delay(1);
  irsend.mark(0);
  digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_DETECT));
  delay(1);
}

void flash(){
  for(int i=0; i<16; i++) 
  { 
    digitalWrite(irLED, HIGH);
    delayMicroseconds(11);
    digitalWrite(irLED, LOW);
    delayMicroseconds(11);
  } 
  delayMicroseconds(7330); 

  for(int i=0; i<16; i++) 
  { 
    digitalWrite(irLED, HIGH);
    delayMicroseconds(11);
    digitalWrite(irLED, LOW);
    delayMicroseconds(11);
  }   
}
