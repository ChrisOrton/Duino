/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // invert mirror display
  //display.ssd1306_command(0xC8);
  //display.ssd1306_command(0xA0);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...


}

void loop() {
  //drawChars();
  drawOrig();
}
  

void drawOrig(void) {

  for(int16_t i=0; i<100; i++) {
    display.clearDisplay();

    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(4,0);             // Start at top-left corner
    display.println(F("FPS"));

    display.setTextSize(5);
    display.setCursor(4,20);             // Start at top-left corner
    char strBuf[50];
    
    sprintf(strBuf, "%3d", i);
    
    
    display.println(strBuf);
        
    display.display();
    delay(1000);
  }

}



bool charset[10][7] ={
  {true, true, true, true,true,true,false},
  {false,true,true,false,false,false,false},
  {true,true,false,true,true,false,true},
  {true,true,true,true,false,false,true},
  {false,true,true,false,false,true,true},
  {true,false,true,true,false,true,true},
  {true,false,true,true,true,true,true},
  {true,true,true,false,false,false,false},
  {true,true,true,true,true,true,true},
  {true,true,true,true,false,true,true}
};

void drawDigit(int number, int x) {
  int lineWidth = 3;
  int lineLength = 16;

  if (charset[number][0])
    display.fillRoundRect(x+12,16, lineLength, lineWidth, 2, SSD1306_WHITE);
  if (charset[number][1])
    display.fillRoundRect(x+30,18, lineWidth, lineLength, 2, SSD1306_WHITE);
  if (charset[number][2]) 
    display.fillRoundRect(x+30,39, lineWidth, lineLength, 2, SSD1306_WHITE);
  if (charset[number][3])
    display.fillRoundRect(x+12,56, lineLength, lineWidth, 2, SSD1306_WHITE);
  if (charset[number][4])
    display.fillRoundRect(x+8,39, lineWidth, lineLength, 2, SSD1306_WHITE);
  if (charset[number][5])
    display.fillRoundRect(x+8,18, lineWidth, lineLength, 2, SSD1306_WHITE);
  if (charset[number][6])
    display.fillRoundRect(x+12,36, lineLength, lineWidth, 2, SSD1306_WHITE);
   
}

void drawNum(int number) {

    int x =72;
    do{
        drawDigit((number % 10),x);

        number = number /10;
        x = x - 36;
    }while (number >0);


}

void drawChars(void) {

  for(int16_t i=0; i<100; i++) {
    display.clearDisplay();

    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(4,0);             // Start at top-left corner
    display.println(F("FPS"));
    
    
    
    drawNum(i);
    display.display();
    delay(1000);
  }

}


void testdrawroundline(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(12,16, 16, 4, 2, SSD1306_WHITE);

    display.fillRoundRect(8,18, 4, 16, 2, SSD1306_WHITE);

    display.fillRoundRect(30,18, 4, 16, 2, SSD1306_WHITE);



    display.fillRoundRect(12,36, 16, 4, 2, SSD1306_WHITE);

    display.fillRoundRect(8,39, 4, 16, 2, SSD1306_WHITE);

    display.fillRoundRect(30,39, 4, 16, 2, SSD1306_WHITE);

    display.fillRoundRect(12,56, 16, 4, 2, SSD1306_WHITE);

      
    display.display();
    delay(1000);
  }

  delay(222000);
}
