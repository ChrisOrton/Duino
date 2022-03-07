#include <LiquidCrystal.h>
#include <IRremote.h>


const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define PIN_IR 3
#define PIN_DETECT 2
#define PIN_STATUS_LED 6


int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define irLED 5

// ms that beam will be disabled
#define BEAM_OFF_TIME 160

IRsend irsend;


boolean detected = false;

unsigned long timeStamp = 0;

void setup()
{
  //Serial.begin(57600);

  lcd.begin(16, 2);
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("Custom Flash");



  pinMode(PIN_IR, OUTPUT);
  pinMode(PIN_DETECT, INPUT);
  pinMode(PIN_STATUS_LED, OUTPUT);
  

  irrecv.enableIRIn(); // Start the receiver
  //Serial.println("set up");
}

void loop() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);

    
    if (results.value != -1){
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(String(results.value,HEX));
    }
    
    irrecv.resume(); // Receive the next value
  }


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
