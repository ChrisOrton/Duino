//www.diyusthad.com
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte interruptPin = 2;
volatile byte state = LOW;
volatile byte oldState = LOW;
volatile int counter = 0;
#define RED 6
#define irLED 5


const int irSensor = A0;
volatile int irSensorValue = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.print("hi PJ line");
  lcd.setCursor(0,1);
  lcd.print("bye bye line");

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink1, CHANGE);
  
  pinMode(RED, OUTPUT);
  pinMode(irLED, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(irLED, LOW);
}

void loop() {
  
  String numberString = String(counter);
  String lowString = String("Low display:"+numberString);

  irSensorValue = analogRead(irSensor);
  String irSensorString = String(irSensorValue);
  String hiString = String("IR:"+irSensorString);
  String clrHiString = String("IR:    ");
  
  if(state){
    lcd.setCursor(0,0);
    lcd.print("hi HIGH");

    digitalWrite(RED, HIGH);
  }else{
    lcd.setCursor(0,0);
    lcd.print("ayeLOW");

    digitalWrite(RED, LOW);
  }
  lcd.setCursor(0,1);
  lcd.print(lowString);
  lcd.setCursor(8,0);
  lcd.print(clrHiString);
  lcd.setCursor(8,0);
  lcd.print(hiString);

  if(state && (state != oldState)){
    flash();
  }
  oldState = state;

}

void blink1() {

  state = !state;
  counter++;
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
