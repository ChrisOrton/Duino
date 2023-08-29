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

//#define SCREEN_WIDTH 64 // OLED display width, in pixels
//#define SCREEN_HEIGHT 48 // OLED display height, in pixels
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


/*
#include "soc/frc_timer_reg.h"

#ifdef CONFIG_ESP_TIMER_IMPL_FRC2
#define timer_u32() (REG_READ(0x3ff47004));     // FRC2
#elif CONFIG_ESP_TIMER_IMPL_TG0_LAC
#define timer_u32() (REG_READ(0x3ff5F078));     // TG0_LAC
#else
                                                // SYSTIMER
__attribute__((always_inline)) static inline uint32_t timer_u32(void) {
   REG_WRITE(0x3f423038,1UL << 31);             // Start synch
   while (!REG_GET_BIT(0x3f423038,1UL << 30));  // Wait synch
   return REG_READ(0x3f423040);                 // Read SYSTIMER_VALUE_LO_REG
}
#endif
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "f0006900-110c-478b-b74b-6f403b364a9c"
#define DATA1_CHARACTERISTIC_UUID "f0006902-110c-478b-b74b-6f403b364a9c"
#define DATA2_CHARACTERISTIC_UUID "f0006904-110c-478b-b74b-6f403b364a9c"



BLECharacteristic *pData1Characteristic;
BLECharacteristic *pData2Characteristic;




int inPin0 = D0;
int inPin1 = D1;

int mtstart = 0;
int mtstop = 0;
int mtdelta = 0;
int stopCount =0;

int roundCount = 0;
bool inFlight = false;
bool sendUpdate = false;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.ssd1306_command(SSD1306_SEGREMAP);

//ssd1306_command(SSD1306_COMSCANDEC);
  display.ssd1306_command(SSD1306_COMSCANINC);


  stopCount =0;

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  pinMode(inPin0, INPUT_PULLUP);    // sets the digital pin 7 as input
  pinMode(inPin1, INPUT_PULLUP);    // sets the digital pin 7 as input
  attachInterrupt(digitalPinToInterrupt(inPin0),buttonStart,RISING); 
  attachInterrupt(digitalPinToInterrupt(inPin1),buttonStop,RISING); 

  roundCount = 0;
  inFlight = false;
  sendUpdate = false;
/*
  Serial.println("Starting BLE work!");
  BLEDevice::init("Chronos");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pData1Characteristic = pService->createCharacteristic(
                                         DATA1_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pData2Characteristic = pService->createCharacteristic(
                                         DATA2_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  unsigned int myTime = micros();
  uint32_t value = myTime;
  pData2Characteristic->setValue((uint8_t*)&value, 4);
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
*/  
  Serial.println("Characteristic defined! Now yu can read it in your phone!");
}


 int64_t espstop =0;
  int64_t espstart =0;

void buttonStart()          
{    
  if (!inFlight){
    inFlight = true;
    espstart = esp_timer_get_time();
  }else{
    inFlight = false;
    espstop = esp_timer_get_time();
  }
    mtstart = micros();
    
    roundCount++;

    
  
}


void buttonStop()          
{ 
  if (!inFlight){
    inFlight = true;
    espstart = esp_timer_get_time();
  }else{
    inFlight = false;
    espstop = esp_timer_get_time();
  }
  //if(inFlight){                                
   mtstop = micros();              
   mtdelta = mtstop - mtstart;
   stopCount++;
   sendUpdate = true;
  //}
  inFlight = false;
}


void loop() {
  //testdrawSegments();
  //simpleTextLoopFPS();
  //testdrawPattern();
  textloop2();
}

void textloop2() {

  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);

  display.setCursor(60, 0);
  if (digitalRead(inPin0)){
    display.println(F("On"));
    
  }else{
    display.println(F("Off")); 
  }

  display.setCursor(0, 0);
  if (digitalRead(inPin1)){
    display.println(F("On"));
    
  }else{
    display.println(F("Off"));

  }


/*
  int fps = 0;
  if (mtdelta >100){
    fps = 328100 /mtdelta;
  }
  drawSegNumber(fps);
*/
  display.setCursor(0, 20);
  display.println(mtdelta);
  display.display();      // Show initial text

  /*
   //if(sendUpdate){
    sendUpdate = false;
    uint32_t value1 = mtdelta;
    uint32_t value2 = fps;
    
    pData1Characteristic->setValue((uint8_t*)&value1, 4);
    pData1Characteristic->notify();
    
    pData2Characteristic->setValue((uint8_t*)&value2, 4);
    pData2Characteristic->notify();
   //}
   */

    int64_t espt =0;
    espt = espstop - espstart;
    display.setCursor(0, 40);
    display.println(espt);
    display.display();      // Show initial text
    
   inFlight = false;
   
     delay(1000);
}



void textloop() {

  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);

  display.setCursor(30, 0);
  if (digitalRead(inPin0)){
    display.println(F("On"));
    
  }else{
    display.println(F("Off")); 
  }

  display.setCursor(0, 0);
  if (digitalRead(inPin1)){
    display.println(F("On"));
    
  }else{
    display.println(F("Off"));

  }
  
  display.setCursor(10, 16);
  char strBuf[50];
  sprintf(strBuf, "%duS", mtdelta);
  display.println(strBuf);


  int fps = 0;
  if (mtdelta >100){

    fps = 328100 /mtdelta;
  }
  display.setCursor(10, 32);
  sprintf(strBuf, "%dfps", fps);
  display.println(strBuf);
  
  display.display();      // Show initial text
}





void simpleTextLoopFPS(void) {
  display.clearDisplay();

  char strBuf[50];
  int fps = 0;
  if (mtdelta >100){
    fps = 328100 /mtdelta;
  }

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("FPS"));

/*
  Serial.println(tstart);
Serial.println(tstop);
*/
  Serial.println(roundCount);
   Serial.println(stopCount);

  drawSegNumber(fps);
  //drawSegNumber(22);

  display.display();
  delay(2000);
}


void testdrawPattern(void) {
  
    display.clearDisplay();
    //display.flipScreenVertically();
    
    int y =21;
    for (int x=31; x<95; x++)
      display.drawPixel(x,y,SSD1306_WHITE);
    y =61;
    for (int x=31; x<95; x++)
      display.drawPixel(x,y,SSD1306_WHITE);
    int x =35;
    for (int y=16; y<64; y++)
      display.drawPixel(x,y,SSD1306_WHITE);
    x = 92;
    for (int y=16; y<79; y++)
      display.drawPixel(x,y,SSD1306_WHITE);
    
    display.display();
  delay(2);
}

void testdrawPatternOld(void) {
  
    display.clearDisplay();
    //display.flipScreenVertically();
    
    int y =5;
    for (int x=0; x<64; x++)
      display.drawPixel(x,y,SSD1306_WHITE);
    y =44;
    for (int x=0; x<64; x++)
      display.drawPixel(x,y,SSD1306_WHITE);
    int x =4;
    for (int y=0; y<48; y++)
      display.drawPixel(x,y,SSD1306_WHITE);
    x = 61;
    for (int y=0; y<48; y++)
      display.drawPixel(x,y,SSD1306_WHITE);
    
    display.display();
  delay(2);
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
    do{
        drawSegDigit(y %10, posx);
        y = y /10;
        posx = posx -20;
    }while (y > 0);
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
