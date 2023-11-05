/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

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


void loop() {
     char strBuf[64];
      
    uint8_t* bufct = pControlCharacteristic->getData();

    int pwm_A = bufct[0] - 127;
    int pwm_B = bufct[1] - 127;
    uint8_t inA = pwm_A;
    uint8_t inB = pwm_B;
  
    pwm_A = pwm_A*8;
    pwm_B = pwm_B*8;
  
    set_motor_pwm(pwm_B, 0, 1);
    set_motor_pwm(pwm_A, 2, 3);

    char bufd[]={xb++, yb++, 23 ,57 };

    //uint8_t bufdt[]={yt++, xt++};
    uint8_t bufdt[]={inA, inB};
    pDataCharacteristic->setValue(bufdt,2);
    
    pDataCharacteristic->notify();

    delay(20);
}
