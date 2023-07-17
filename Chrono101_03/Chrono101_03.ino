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




#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   2

#define PIXEL_PIN    12 // 6  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 22  // Number of NeoPixels


// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int inPin0 = 2;
int inPin1 = 3;

int tstart = 0;
int tstop = 0;
int tdelta = 0;

int roundCount = 0;

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.ssd1306_command(0xC8);
  display.ssd1306_command(0xA0);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  pinMode(13, INPUT_PULLUP);
//  pinMode(12, INPUT_PULLUP);



  pinMode(inPin0, INPUT);    // sets the digital pin 7 as input
  pinMode(inPin1, INPUT);    // sets the digital pin 7 as input
  attachInterrupt(digitalPinToInterrupt(inPin0),buttonStart,FALLING); 
  attachInterrupt(digitalPinToInterrupt(inPin1),buttonStop,FALLING); 

  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'

  int roundCount = 0;

}

bool inFlight = false;
int nowLoop = 0;
int lastLoop = 0;

int interval = 2000;
void loop() {
  if (inFlight){
    colorWipeT(strip.Color(  0,   0,   0), 5);    // Black/off
  }else{
    LCDTextLoop();
    //stylesTextLoop();
  }
}

void fancyloop() {
  if (inFlight){
    colorWipeT(strip.Color(  0,   0,   0), 5);    // Black/off
  }else{
    nowLoop = millis();
    if ((nowLoop - lastLoop) > interval){
      lastLoop = nowLoop;
      int modex = (nowLoop/interval ) % 3;
      if(modex==0){
        stylesTextLoopRound();
      } else if(modex==1){
        stylesTextLoop();
      }else{
        stylesTextLoopMPH();
      }
    }
  }
}

void simpleloop() {
  textloop();       
}


void stylesTextLoopRound(void) {
  display.clearDisplay();

  char strBuf[50];
  display.setCursor(10, 50);
  sprintf(strBuf, "%3d", roundCount);

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(10,0);             // Start at top-left corner
  display.println(F("Rounds"));

  display.setCursor(20,20);             // Start at top-left corner
  display.setTextSize(6);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.println(strBuf);

  display.display();
}



void stylesTextLoopMPH(void) {
  display.clearDisplay();

  char strBuf[50];
  int mph = 0;
  if (tdelta >100){
    mph = 223694 /tdelta;
  }
  display.setCursor(10, 50);
  sprintf(strBuf, "%3d", mph);

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(10,0);             // Start at top-left corner
  display.println(F("MPH"));

  display.setCursor(20,20);             // Start at top-left corner
  display.setTextSize(6);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.println(strBuf);

  display.display();
}


void stylesTextLoop(void) {
  display.clearDisplay();

  char strBuf[50];
  int fps = 0;
  if (tdelta >100){
    fps = 328100 /tdelta;
  }
  display.setCursor(10, 50);
  sprintf(strBuf, "%3d", fps);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(10,0);             // Start at top-left corner
  display.println(F("FPS"));

  display.setCursor(20,20);             // Start at top-left corner
  display.setTextSize(6);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.println(strBuf);

  display.display();
}




void LCDTextLoop(void) {
  display.clearDisplay();

  int fps = 0;
  if (tdelta >100){
    fps = 328100 /tdelta;
  }

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(10,0);             // Start at top-left corner
  display.println(F("FPS"));

  drawNum(fps);

  display.display();
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
