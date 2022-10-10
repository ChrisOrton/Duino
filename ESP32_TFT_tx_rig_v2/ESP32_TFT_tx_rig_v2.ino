#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>


TFT_eSPI tft = TFT_eSPI();         // Invoke custom library


#define TFT_CS2 14

#define I2C_SDA 23
#define I2C_SCL 18 

int t;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  pinMode(TFT_CS2, OUTPUT);
  digitalWrite(TFT_CS2, LOW);

    
  tft.begin();
  tft.setRotation(3);
  tft.setTextColor(0xFFFF, TFT_BLUE);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLUE);
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)
  

  t =0;
 
  digitalWrite(TFT_CS2, HIGH);
}

int xl=0;
int yl=0;
int xr=0;
int yr=0;

void loop() {
  
  digitalWrite(TFT_CS2, LOW);
  // How much time did rendering take (ESP8266 80MHz 271ms, 160MHz 157ms, ESP32 SPI 120ms, 8bit parallel 105ms
  t = millis() - t;

  int value1 = analogRead(13);
  int value2 = analogRead(15);
  int value3 = 4096 - analogRead(2);
  int value4 = analogRead(4);

  /* Print out the values */
  tft.setTextSize(2);
  tft.setCursor(25, 20);
  tft.print("13");
  tft.setCursor(85, 20);
  tft.print("15");
  tft.setCursor(145, 20);
  tft.print("2");
  tft.setCursor(205, 20);
  tft.print("4");
  

  tft.setCursor(10, 40);
  tft.print("                        ");
  tft.setCursor(10, 40);
  tft.print(value1);
  tft.setCursor(70, 40);
  tft.print(value2);
  tft.setCursor(130, 40);
  tft.print(value3);
  tft.setCursor(190, 40);
  tft.print(value4);
  tft.setCursor(250, 50);
  tft.setTextSize(1);


  int cx = 80;
  int cy = 150;
  
  tft.drawCircle(cx+xl, cy+yl, 5, TFT_BLUE);
  xl = (value2 - 2048) * 25;
  xl = xl /1000;
  yl = (2048 - value1) * 25;
  yl = yl /1000;
  tft.drawCircle(cx+xl, cy+yl, 5, TFT_YELLOW);
  
  tft.drawCircle(80, 150, 50, TFT_YELLOW);

  cx = 240;
  cy = 150;
  
  tft.drawCircle(cx+xr, cy+yr, 5, TFT_BLUE);
  xr = (value4 - 2048) * 25;
  xr = xr /1000;
  yr = (2048 - value3) * 25;
  yr = yr /1000;
  tft.drawCircle(cx+xr, cy+yr, 5, TFT_YELLOW);
  tft.drawCircle(240, 150, 50, TFT_YELLOW);

  
  //Serial.println("");
  //delay(500);
  delay(10);

      
}
