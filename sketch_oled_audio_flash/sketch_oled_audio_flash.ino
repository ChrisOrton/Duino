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


#define audioIn 2
#define irLED 7

volatile byte state = LOW;
volatile byte flashing = false;
volatile byte triggered = false;

void setup() {
  Serial.begin(9600);

    pinMode(irLED, OUTPUT);

    //pinMode(audioIn, INPUT_PULLUP);
    pinMode(audioIn, INPUT);
    //attachInterrupt(digitalPinToInterrupt(audioIn), blink, CHANGE);
    attachInterrupt(digitalPinToInterrupt(audioIn), blink, RISING);

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
  
 
}

void loop() {
  Serial.println("SSD1306 allocation failed");
  oled.clearDisplay(); // clear display
  if(triggered){
    oled.setCursor(0, 50);        // position to display
    oled.println("       "); // text to display
    oled.setCursor(0, 50);        // position to display
    oled.println("LOUD!"); // text to display
  }else{
    oled.setCursor(0, 50);        // position to display
    oled.println("       ");      // text to display
    oled.setCursor(0, 50);        // position to display
    oled.println("quiet!");       // text to display
    
  }

  oled.display();               // show on OLED
  if(triggered){
    triggered =false;
    delay(2000);
  }else{
    triggered =false;
    delay(100);    
  }
}

void blink() {
  state = !state;
  triggered =   true;
  if(!flashing){
    flash();
  }
}

void flash(){
  flashing = true;
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
  flashing =false;
}
