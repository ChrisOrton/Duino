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


void setup() {
  Serial.begin(115200);

  delay(500); // Pause for 0.5 second

  Serial.println(F("SSD1306 allocation attempt"));
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  //display.ssd1306_command(SSD1306_SEGREMAP);

//ssd1306_command(SSD1306_COMSCANDEC);
  display.ssd1306_command(SSD1306_COMSCANINC);




  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

}

void loop() {
  testdrawSegments();
}

int segCodes[10] = { 
  B01111110,          // 0
  B00110000,          // 1
  B01101101,          // 2
  B01111001,          // 3
  B00110011,          // 4
  B01011011,          // 5
  B01011111,          // 6
  B01110000,          // 7
  B01111111,          // 8
  B01111011,          // 9
};

void drawSegDigit(int d, int x){
    int lineWidth = 3;
    int segCode = segCodes[d];

    if (segCode & B01000000)
      display.fillRoundRect(x + 12,18, 16, lineWidth, 2, SSD1306_WHITE);    // A
    if (segCode & B00100000)
      display.fillRoundRect(x + 30,20, lineWidth, 16, 2, SSD1306_WHITE);    // B
    if (segCode & B00010000)
      display.fillRoundRect(x + 30,41, lineWidth, 16, 2, SSD1306_WHITE);    // C
    if (segCode & B00001000)
      display.fillRoundRect(x + 12,58, 16, lineWidth, 2, SSD1306_WHITE);    // D
    if (segCode & B00000100)
      display.fillRoundRect(x + 8,41, lineWidth, 16, 2, SSD1306_WHITE);     // E
    if (segCode & B00000010)
      display.fillRoundRect(x + 8,20, lineWidth, 16, 2, SSD1306_WHITE);     // F
    if (segCode & B00000001)
      display.fillRoundRect(x + 12,38, 16, lineWidth, 2, SSD1306_WHITE);    // G 
}

void drawSegNumber(int x){
    int y = x;
    int posx = 80;
    while (y > 0){
        drawSegDigit(y %10, posx);
        y = y /10;
        posx = posx -35;
    }
}


void testdrawSegments(void) {
  for (int i=0 ; i<1000; i++){
    display.clearDisplay();
    //display.flipScreenVertically();

    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(10,0);             // Start at top-left corner
    display.println(F("Rounds"));
    
    drawSegNumber(i);
    display.display();
    delay(300);
  }
  delay(2);
}
