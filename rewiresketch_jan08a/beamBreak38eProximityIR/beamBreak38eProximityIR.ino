#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define PIN_SEND_IR 3
#define PIN_DETECT_IR 2
#define PIN_STATUS_LED 6


#define irLED 5

boolean sendState = false;


const int irSensor = A0;
volatile int irSensorValue = 0;

const int potSensor = A1;
volatile int potSensorValue = 0;


void setup()
{

  lcd.begin(16, 2);
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("Custom Flash");

  
  pinMode(PIN_DETECT_IR, INPUT);
  
  pinMode(PIN_STATUS_LED, OUTPUT);
  pinMode(irLED, OUTPUT);
}

void loop() {
  sendState = !sendState;
  //digitalWrite(PIN_SEND_IR, sendState);
  digitalWrite(PIN_SEND_IR, true);
  //digitalWrite(PIN_STATUS_LED, digitalRead(PIN_DETECT_IR));


  irSensorValue = analogRead(irSensor);
  String irSensorString = String(irSensorValue);
  String hiString = String("IR:"+irSensorString);
  String clrHiString = String("IR:    ");
  lcd.setCursor(8,0);
  lcd.print(clrHiString);
  lcd.setCursor(8,0);
  lcd.print(hiString);
  
  potSensorValue = analogRead(potSensor);
  String potSensorString = String(potSensorValue);
  String loString = String("PT:"+potSensorString);
  String clrLoString = String("PT:    ");
  lcd.setCursor(8,1);
  lcd.print(clrLoString);
  lcd.setCursor(8,1);
  lcd.print(loString);

  if (irSensorValue > potSensorValue){
    flash();
    delay(2000);
  }
  
  delay(5);
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
