//www.diyusthad.com
#include <LiquidCrystal.h>


#include <MPU6050_tockn.h>
#include <Wire.h>


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


MPU6050 mpu6050(Wire);
long timer = 0;

int inputVal = 0;
long fixIn = 0;

void setup() {

  Wire.begin();
  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("First line");
  lcd.setCursor(0,1);
  lcd.print("Second line");


  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  
}

void loop() {
  mpu6050.update();
  
  if(millis() - timer > 500){

      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print(mpu6050.getAngleX());
      lcd.setCursor(6,0);
      lcd.print(mpu6050.getAngleY());
      lcd.setCursor(12,0);
      lcd.print(mpu6050.getAngleZ());

      lcd.setCursor(0,1);
      lcd.print("                ");
      inputVal = analogRead (A0); // Analog Values 0 to 1023
      lcd.setCursor(0,1);
      fixIn = 102300/inputVal - 100;
      lcd.print(fixIn);
      inputVal = analogRead (A1); // Analog Values 0 to 1023
      lcd.setCursor(8,1);
      fixIn = 102300/inputVal - 100;
      lcd.print(fixIn);

      
      timer = millis();
      //lcd.setCursor(8,1);
      //lcd.print(timer);
      
  }
}
