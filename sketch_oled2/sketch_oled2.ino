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


// Setup timers and temp variables
long loop_timer;
int temp;
 
// Display counter
int displaycount = 0;

void setup() {
  //Serial.begin(9600);


  //Start I2C
  //Wire.begin();
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  //Serial.begin(9600);           // start serial for output
  
  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    //Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(2);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 0);        // position to display
  oled.println("Hello Wld!"); // text to display
  oled.setTextSize(1);          // text size
  oled.setCursor(0, 20);        // position to display
  oled.println("Hola Mundo!"); // text to display
  
  oled.setCursor(0, 30);        // position to display
  oled.println("Dia duit ar domhan!"); // text to display
  
  oled.setCursor(0, 40);        // position to display
  oled.println("Hola kilo!"); // text to display
  oled.display();               // show on OLED
  
  // Init Timer 
  loop_timer = micros();                              
  
}

void loop() {
  
  // Increment the display counter
  displaycount = displaycount +1;
  
  if (displaycount > 100) {
    //oled.clearDisplay(); // clear display
    oled.setTextSize(1);          // text size
    oled.setTextColor(WHITE);     // text color
    oled.setCursor(0, 8);        // position to display
    oled.println("Pitch"); // text to display
    displaycount = 0;
    oled.display();               // show on OLED
  }
  

}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  int s = Wire.read();    // receive byte as an integer
  int x = Wire.read();    // receive byte as an integer
  int y = Wire.read();    // receive byte as an integer
  String str = "";
  while(0 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    str += c;
  }
  oled.clearDisplay(); // clear display
  oled.setTextSize(s);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(x, y);        // position to display
  oled.println(str); // text to display
    
  oled.display();               // show on OLED
}
 
