/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-oled
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);

  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

    oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 8);        // position to display
  oled.println("Hello World!"); // text to display
  oled.display();               // show on OLED
  oled.setCursor(0, 20);        // position to display
  oled.println("Hola Mundo!"); // text to display
  oled.display();               // show on OLED
  oled.setCursor(0, 30);        // position to display
  oled.println("Dia duit ar domhan!"); // text to display
  oled.display();               // show on OLED
  oled.setCursor(0, 40);        // position to display
  oled.println("Hola món!"); // text to display
  oled.display();               // show on OLED
  oled.setCursor(0, 50);        // position to display
  oled.println("Hallo Welt!"); // text to display
  oled.display();               // show on OLED
}

void loop() {
  Serial.println("SSD1306 allocation failed");
}
