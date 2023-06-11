/*********
  Complete project details at https://randomnerdtutorials.com
  
  This is an example for our Monochrome OLEDs based on SSD1306 drivers. Pick one up today in the adafruit shop! ------> http://www.adafruit.com/category/63_98
  This example is for a 128x32 pixel display using I2C to communicate 3 pins are required to interface (two I2C and one reset).
  Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries, with contributions from the open source community. BSD license, check license.txt for more information All text above, and the splash screen below must be included in any redistribution. 
*********/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };


int inPin0 = 2;
int inPin1 = 3;
int output = 0;

int tstart = 0;
int tstop = 0;
int tdelta = 0;


void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();


  pinMode(inPin0, INPUT);    // sets the digital pin 7 as input
  pinMode(inPin1, INPUT);    // sets the digital pin 7 as input
  //attachInterrupt(digitalPinToInterrupt(2),buttonPressed1,RISING); 
  attachInterrupt(digitalPinToInterrupt(inPin0),buttonStart,FALLING); 
  attachInterrupt(digitalPinToInterrupt(inPin1),buttonStop,FALLING); 

/*
  testfillrect();      // Draw rectangles (filled)


  testfillcircle();    // Draw circles (filled)


  testscrolltext();    // Draw scrolling text


  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
*/
}



void loop() {

  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  if (digitalRead(inPin0)){
    display.println(F("Zero"));
    
  }else{
    display.println(F("Not Zero")); 
  }

  display.setCursor(0, 15);
  if (digitalRead(inPin1)){
    display.println(F("One"));
    
  }else{
    display.println(F("Not One"));

  }
  
  display.setCursor(10, 30);
 display.println(tdelta);


  int fps = 0;
  if (tdelta >100){

    fps = 328100 /tdelta;
  }
  display.setCursor(10, 50);
 display.println(fps);

  
  display.display();      // Show initial text
}



void oldloop() {

  if (output == 1){
    testfillrect();      // Draw rectangles (filled)
  }else{
    testfillcircle();      // Draw rectangles (filled)
  }

  //if (digitalRead(inPin0)){
    //testfillrect();      // Draw rectangles (filled)
  //}else{
    //testfillcircle();      // Draw rectangles (filled)
  //}
}


void buttonStart()          
{                   
   output = 1; 
   tstart = micros();              
    //testfillcircle();      // Draw rectangles (filled)
  //testscrolltext();    // Draw scrolling text
}


void buttonStop()          
{                   
   output = 1;               
   tstop = micros();              
   tdelta = tstop -tstart;
    //testfillcircle();      // Draw rectangles (filled)
  //testscrolltext();    // Draw scrolling text
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}


void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}
