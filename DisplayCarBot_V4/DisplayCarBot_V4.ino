/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <Adafruit_NeoPixel.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define NAMESTRING "MV CarBot"
#define SERVICE_UUID        "f0006900-110c-478b-b74b-6f403b364a9c"
#define MODE_CHARACTERISTIC_UUID "f0006901-110c-478b-b74b-6f403b364a9c"
#define CONTROL_CHARACTERISTIC_UUID "f0006903-110c-478b-b74b-6f403b364a9c"

#define DATA_CHARACTERISTIC_UUID "f0006902-110c-478b-b74b-6f403b364a9c"



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


#define PIXEL_PIN    D9  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 11  // Number of NeoPixels

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)




BLECharacteristic *pModeCharacteristic;
BLECharacteristic *pControlCharacteristic;

BLECharacteristic *pDataCharacteristic;


/* Setting PWM Properties */
const int PWMFreq = 50; /* 5 KHz */
const int PWMChannel = 0;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

// Define the control inputs
#define MOT_A1_PIN D0
#define MOT_A2_PIN D1
#define MOT_B1_PIN D2
#define MOT_B2_PIN D3

void setup() {
  Serial.begin(115200);




  Serial.println(F("SSD1306 allocation attempt"));
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  
  display.display();

  Serial.println(F("Neo pixel Strip Init"));
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'

  // Set all the motor control inputs to OUTPUT
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
  pinMode(MOT_B1_PIN, OUTPUT);
  pinMode(MOT_B2_PIN, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);
  digitalWrite(MOT_B1_PIN, LOW);
  digitalWrite(MOT_B2_PIN, LOW);

  ledcSetup(0, PWMFreq, PWMResolution);
  ledcSetup(1, PWMFreq, PWMResolution);
  ledcSetup(2, PWMFreq, PWMResolution);
  ledcSetup(3, PWMFreq, PWMResolution);  

  ledcAttachPin(MOT_A1_PIN, 0);
  ledcAttachPin(MOT_A2_PIN, 1);
  ledcAttachPin(MOT_B1_PIN, 2);
  ledcAttachPin(MOT_B2_PIN, 3);




  
  Serial.println("Starting BLE work!");

  BLEDevice::init(NAMESTRING);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pModeCharacteristic = pService->createCharacteristic(
                                         MODE_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  
  pControlCharacteristic = pService->createCharacteristic(
                                         CONTROL_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );


  pDataCharacteristic = pService->createCharacteristic(
                                         DATA_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );



  char buf2[]={0x48,0x01,0x00};
  pModeCharacteristic->setValue(buf2);
  
  uint8_t bufd[]={0x7F,0x7F};
  pControlCharacteristic->setValue(bufd,2);
  pDataCharacteristic->setValue(bufd,2);
  
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void myhexit(char strBuffer[], String str){
  char lbuf[64];
   str.toCharArray(lbuf, 64);
  for(int i = 0; i < strlen(lbuf); i++)
    sprintf(&strBuffer[i*3] , "%02x ",lbuf[i]);
}

byte xb=0;
byte yb=122;

uint8_t xt=0;
uint8_t yt=112;


void set_motor_pwm(int pwm, int IN1_PIN, int IN2_PIN)
{
  if (pwm < 0) {  // reverse speeds
    ledcWrite(IN1_PIN, -pwm);
    ledcWrite(IN2_PIN, 0);
  } else { // stop or forward
    ledcWrite(IN1_PIN, 0);
    ledcWrite(IN2_PIN, pwm);
  }
}

int pulse=0;
void loop() {
     char strBuf[64];
      
    uint8_t* bufct = pControlCharacteristic->getData();

    int pwm_A = bufct[0] - 127;
    int pwm_B = bufct[1] - 127;
    uint8_t inA = pwm_A;
    uint8_t inB = pwm_B;
  
    pwm_A = pwm_A*8;
    pwm_B = pwm_B*8;
  
/*
    set_motor_pwm(pwm_B, 0, 1);
    set_motor_pwm(pwm_A, 2, 3);
*/                     
    char bufd[]={xb++, yb++, 23 ,57 };

    //uint8_t bufdt[]={yt++, xt++};
    uint8_t bufdt[]={inA, inB};
    pDataCharacteristic->setValue(bufdt,2);
    
    pDataCharacteristic->notify();


    display.clearDisplay();
    //display.flipScreenVertically();

    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Angle"));
    display.setCursor(0,20);             // Start at top-left corner
    display.println(bufct[0]);
    display.setCursor(64,20);             // Start at top-left corner
    display.println(pwm_A);

    display.setCursor(0,40);             // Start at top-left corner
    display.println(bufct[1]);
    display.setCursor(64,40);             // Start at top-left corner
    display.println(pwm_B);
    display.display();
  
    colorWipeT(strip.Color(  0,   0,   0), bufct[0],bufct[1], pulse++);    // Black/off
    delay(20);
}

void clipLED(int pixel, uint32_t color){
    if ((pixel >=0) && (pixel <11)){
      strip.setPixelColor(pixel, color);
      strip.setPixelColor(21-pixel, color);
    }
}


void colorWipeT(uint32_t color, int wait, int fix, int pulse) {

  uint32_t  color0 = strip.Color(  255-fix, wait,  fix );
  uint32_t  color1 = strip.Color(  255-fix, wait,   fix);
  uint32_t  color2 = strip.Color(  255-fix, wait,  fix);
  uint32_t  color3 = strip.Color(  255-fix, wait,  fix);

  int modFactor =15;

  int i =  pulse % (strip.numPixels() * 10);
  //for(int i= 11; i<strip.numPixels() * 10; i++) { // For each pixel in strip...
    strip.clear();         //   Set all pixels in RAM to 0 (off)
    clipLED((i-3) % modFactor, color0);         //  Set pixel's color (in RAM)    
    clipLED((i-2) % modFactor, color1);         //  Set pixel's color (in RAM)    
    clipLED((i-1) % modFactor, color2);         //  Set pixel's color (in RAM)
    clipLED(i % modFactor, color3);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait/4);                           //  Pause for a moment
  //}
}


void colorWipeTO(uint32_t color, int wait, int fix, int pulse) {
/*  
  uint32_t  color0 = strip.Color(  255-fix,   fix, 7);
  uint32_t  color1 = strip.Color(  255-fix,   fix, 15);
  uint32_t  color2 = strip.Color(  255-fix,   fix, 31);
  uint32_t  color3 = strip.Color(  255-fix,   fix, 63);
*/
  uint32_t  color0 = strip.Color(  255-fix, 0,  fix );
  uint32_t  color1 = strip.Color(  255-fix, 0,   fix);
  uint32_t  color2 = strip.Color(  255-fix, 0,  fix);
  uint32_t  color3 = strip.Color(  255-fix, 0,  fix);

  int modFactor =15;

  int i =  pulse % (strip.numPixels() * 10);
  //for(int i= 11; i<strip.numPixels() * 10; i++) { // For each pixel in strip...
    strip.clear();         //   Set all pixels in RAM to 0 (off)
    clipLED((i-3) % modFactor, color0);         //  Set pixel's color (in RAM)    
    clipLED((i-2) % modFactor, color1);         //  Set pixel's color (in RAM)    
    clipLED((i-1) % modFactor, color2);         //  Set pixel's color (in RAM)
    clipLED(i % modFactor, color3);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait/4);                           //  Pause for a moment
  //}
}
