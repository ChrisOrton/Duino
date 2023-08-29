/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/


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
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


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

void setup() {
  Serial.begin(115200);

  Serial.println("Starting Display!");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

    // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  
  Serial.println("Starting BLE work!");

  BLEDevice::init("Chronos");

  Serial.println("Starting BLE work! 3");

  
  BLEServer *pServer = BLEDevice::createServer();
  
  Serial.println("Starting BLE work!  5");
  
  BLEService *pService = pServer->createService(SERVICE_UUID);

  Serial.println("Starting BLE work  7");
  pData1Characteristic = pService->createCharacteristic(
                                         DATA1_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

Serial.println("Starting BLE work 9");

                                       
  //pData1Characteristic->setValue("Hello World says Neil");                                       
  //pData1Characteristic->notify();

Serial.println("Starting BLE work 10");

  
  pData2Characteristic = pService->createCharacteristic(
                                         DATA2_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );


  Serial.println("Starting BLE work 11");
  
  unsigned int myTime = micros();
  uint32_t value = myTime;
  pData2Characteristic->setValue((uint8_t*)&value, 4);

  Serial.println("Starting BLE work 12");
  
  Serial.println(myTime);

  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now yu can read it in your phone!");
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int myTime = micros();
  uint32_t value1 = myTime;
  uint32_t value2 = random(0, 255);;
  
  pData1Characteristic->setValue((uint8_t*)&value1, 4);
  pData1Characteristic->notify();
  
  pData2Characteristic->setValue((uint8_t*)&value2, 4);
  pData2Characteristic->notify();
  

  Serial.println(myTime);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Count"));
    display.setCursor(0,20);             // Start at top-left corner
    display.println( myTime);
    display.display();
 
  delay(2000);
}
