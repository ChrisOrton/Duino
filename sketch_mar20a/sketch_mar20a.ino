//www.diyusthad.com
#include <LiquidCrystal.h>


#include <MPU6050_tockn.h>
#include <Wire.h>


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


MPU6050 mpu6050(Wire);
long timer = 0;


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
      lcd.print("Pitch   ");
      lcd.setCursor(8,0);
      lcd.print(mpu6050.getAngleX());

      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(mpu6050.getAngleY());
      lcd.setCursor(8,1);
      lcd.print(mpu6050.getAngleZ());


      
      timer = millis();
      //lcd.setCursor(8,1);
      //lcd.print(timer);
      
  }
}
