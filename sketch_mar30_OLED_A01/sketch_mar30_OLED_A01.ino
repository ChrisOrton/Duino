//www.diyusthad.com


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



long timer = 0;

int inputVal = 0;
long fixIn = 0;

void setup() {


  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(2);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 0);        // position to display
  oled.println("Pls WAIT"); // text to display
  oled.setTextSize(1);          // text size
  oled.setCursor(0, 20);        // position to display
  oled.println("Hola Mundo!"); // text to display
  oled.setCursor(0, 30);        // position to display
  oled.println("Dia duit ar domhan!"); // text to display
  oled.setCursor(0, 40);        // position to display
  oled.println("Hola kilo!"); // text to display
  oled.setCursor(0, 50);        // position to display
  oled.println("Hello Trig!"); // text to display
  oled.display();               // show on OLED
  
}

void loop() {
  
  if(millis() - timer > 500){
      oled.clearDisplay(); // clear display

      oled.setTextSize(2);          // text size
      oled.setTextColor(WHITE);     // text color
      oled.setCursor(0, 0);        // position to display
      oled.println("Joy XY"); // text to display
      
      oled.setTextSize(1);          // text size
      inputVal = analogRead (A0); // Analog Values 0 to 1023
      fixIn = 102300/inputVal - 100;
      oled.setCursor(0, 20);        // position to display
      oled.println(fixIn); // text to display
      inputVal = analogRead (A1); // Analog Values 0 to 1023
      fixIn = 102300/inputVal - 100;
      oled.setCursor(0, 30);        // position to display
      oled.println(fixIn); // text to display
      
      timer = millis();
  }
}
