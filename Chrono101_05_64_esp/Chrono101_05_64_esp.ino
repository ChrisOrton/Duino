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


#define SCREEN_WIDTH 64 // OLED display width, in pixels
#define SCREEN_HEIGHT 48 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int inPin0 = D0;
int inPin1 = D1;

int tstart = 0;
int tstop = 0;
int tdelta = 0;

int roundCount = 0;

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  //display.ssd1306_command(SSD1306_SEGREMAP);

//ssd1306_command(SSD1306_COMSCANDEC);
//  display.ssd1306_command(SSD1306_COMSCANINC);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  pinMode(inPin0, INPUT_PULLUP);    // sets the digital pin 7 as input
  pinMode(inPin1, INPUT_PULLUP);    // sets the digital pin 7 as input
  attachInterrupt(digitalPinToInterrupt(inPin0),buttonStart,FALLING); 
  attachInterrupt(digitalPinToInterrupt(inPin1),buttonStop,FALLING); 

  int roundCount = 0;

}

bool inFlight = false;
int nowLoop = 0;
int lastLoop = 0;

int interval = 2000;
void loop() {

  simpleTextLoopFPS();       
  /*
  if (inFlight){
    //colorWipeT(strip.Color(  0,   0,   0), 5);    // Black/off
  }else{
    nowLoop = millis();
    if ((nowLoop - lastLoop) > interval){
      lastLoop = nowLoop;
      int modex = (nowLoop/interval ) % 3;
      if(modex==0){
        stylesTextLoopRound();
      } else if(modex==1){
        stylesTextLoopFPS();
      }else{
        stylesTextLoopMPH();
      }
    }
  }
  */
}

//textloop();       
void simpleTextLoopFPS(void) {
  display.clearDisplay();

  char strBuf[50];
  int fps = 0;
  if (tdelta >100){
    fps = 328100 /tdelta;
  }

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("FPS"));

  drawSegNumber(fps);

  display.display();
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


//    x += 31;
//    int y =16;
    x += 0;
    int y = 10;

    if (segCode & B01000000)
      display.fillRoundRect(x + 6,9+y, 8, lineWidth, 2, SSD1306_WHITE);    // A
    if (segCode & B00100000)
      display.fillRoundRect(x + 15,10+y, lineWidth, 8, 2, SSD1306_WHITE);    // B

    if (segCode & B00010000)
      display.fillRoundRect(x + 15,21+y, lineWidth, 8, 2, SSD1306_WHITE);    // C

    if (segCode & B00001000)
      display.fillRoundRect(x + 6,29+y, 8, lineWidth, 2, SSD1306_WHITE);    // D

    if (segCode & B00000100)
      display.fillRoundRect(x + 4,21+y, lineWidth, 8, 2, SSD1306_WHITE);     // E

    if (segCode & B00000010)
      display.fillRoundRect(x + 4,10+y, lineWidth, 8, 2, SSD1306_WHITE);     // F

    if (segCode & B00000001)
      display.fillRoundRect(x + 6,19+y, 8, lineWidth, 2, SSD1306_WHITE);    // G 
}


void drawSegNumber(int x){
    int y = x;
    int posx = 40;
    while (y > 0){
        drawSegDigit(y %10, posx);
        y = y /10;
        posx = posx -20;
    }
}

void testdrawSegments(void) {
  for (int i=0 ; i<1000; i++){
    display.clearDisplay();
    //display.flipScreenVertically();

    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Shots"));
    
    drawSegNumber(i);
    display.display();
    delay(300);
  }
  delay(2);
}

void stylesTextLoopRound(void) {
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(10,0);             // Start at top-left corner
  display.println(F("Rounds"));

/*
  char strBuf[50];
  display.setCursor(10, 50);
  sprintf(strBuf, "%3d", roundCount);

  display.setCursor(20,20);             // Start at top-left corner
  display.setTextSize(6);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.println(strBuf);

*/
  drawSegNumber(roundCount);


  display.display();
}



void stylesTextLoopMPH(void) {
  display.clearDisplay();

  char strBuf[50];
  int mph = 0;
  if (tdelta >100){
    mph = 223694 /tdelta;
  }


  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(10,0);             // Start at top-left corner
  display.println(F("MPH"));

/*  
  display.setCursor(10, 50);
  sprintf(strBuf, "%3d", mph);

  display.setCursor(20,20);             // Start at top-left corner
  display.setTextSize(6);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.println(strBuf);
*/

  drawSegNumber(mph);


  display.display();
}


void stylesTextLoopFPS(void) {
  display.clearDisplay();

  char strBuf[50];
  int fps = 0;
  if (tdelta >100){
    fps = 328100 /tdelta;
  }

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(10,0);             // Start at top-left corner
  display.println(F("FPS"));

/*  
  display.setCursor(10, 50);
  sprintf(strBuf, "%3d", fps);

  display.setCursor(20,20);             // Start at top-left corner
  display.setTextSize(6);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.println(strBuf);

*/
  drawSegNumber(fps);

  display.display();
}








void textloop() {

  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);

  display.setCursor(60, 0);
  if (digitalRead(inPin0)){
    display.println(F("Z  On"));
    
  }else{
    display.println(F("Z Off")); 
  }

  display.setCursor(0, 0);
  if (digitalRead(inPin1)){
    display.println(F("O  On"));
    
  }else{
    display.println(F("O Off"));

  }
  
  display.setCursor(10, 30);

  char strBuf[50];
  sprintf(strBuf, "%duS", tdelta);
  
 display.println(strBuf);


  int fps = 0;
  if (tdelta >100){

    fps = 328100 /tdelta;
  }
  display.setCursor(10, 50);
  sprintf(strBuf, "%dfps", fps);
  display.println(strBuf);
  
  display.display();      // Show initial text
}
/*
void clipLED(int pixel, uint32_t color){
    if ((pixel >=0) && (pixel <11)){
      strip.setPixelColor(pixel, color);
      strip.setPixelColor(21-pixel, color);
    }
  
}

uint32_t  color0 = strip.Color(  7,   0, 7);
uint32_t  color1 = strip.Color(  15,   0, 15);
uint32_t  color2 = strip.Color(  31,   0, 31);
uint32_t  color3 = strip.Color(  63,   0, 63);
int modFactor =15;

void colorWipeT(uint32_t color, int wait) {
  for(int i=0; i<modFactor; i++) { // For each pixel in strip...
    strip.clear();         //   Set all pixels in RAM to 0 (off)
    clipLED((i-3) % modFactor, color0);         //  Set pixel's color (in RAM)    
    clipLED((i-2) % modFactor, color1);         //  Set pixel's color (in RAM)    
    clipLED((i-1) % modFactor, color2);         //  Set pixel's color (in RAM)
    clipLED(i % modFactor, color3);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
  inFlight = false;
}
*/

void buttonStart()          
{    
  tstart = micros();
  inFlight = true;               
  roundCount++;
}


void buttonStop()          
{                                 
   tstop = micros();              
   tdelta = tstop -tstart;
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
