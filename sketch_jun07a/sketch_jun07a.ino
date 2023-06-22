#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 /*128 width of OLED in pixels*/

#define SCREEN_HEIGHT 64 /*64 height of OLED in pixels*/

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); /*OLED display connected at I2C pins (SDA, SCL)*/

void setup() {

  Serial.begin(115200);  /*Baud rate UART communication */
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { /*I2C Address at which OLED will communicate*/
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();  /*Clear display*/
  display.setTextSize(2);  /*OLED screen text size defined*/
  display.setTextColor(WHITE); /*OLED screen text color*/
  display.setCursor(0, 0); /*Display static text*/
  display.println("Cool Spice");  /*String to represent on OLED display*/
  display.setCursor(0, 20); /*Display static text*/
  display.println("Hard Mint");  /*String to represent on OLED display*/

  
  display.setCursor(0, 50); /*Display static text*/
  display.println("Macho Peas");  /*String to represent on OLED display*/

  
  
  display.display();


}

void loop() {

}
