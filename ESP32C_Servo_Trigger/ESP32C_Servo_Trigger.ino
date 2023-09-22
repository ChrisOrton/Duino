/*
  Controls servo position from 0-180 degrees and back
  https://wokwi.com/projects/350037178957431378
  by dlloydev, December 2022.


  Servo control that works on ESP32C
  https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
*/

#include <Servo.h>


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define NAMESTRING "MV TriggerBot"
#define SERVICE_UUID        "5f2e6606-845f-4844-a3a2-fb6f244897c9"
#define CONTROL_CHARACTERISTIC_UUID "5f2e6607-845f-4844-a3a2-fb6f244897c9"

BLECharacteristic *pControlCharacteristic;


Servo servo0 = Servo();
//Servo servo1 = Servo();

const int servoPin0 = D0;
const int servoPin1 = D1;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  
  BLEDevice::init(NAMESTRING);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pControlCharacteristic = pService->createCharacteristic(
                                         CONTROL_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  
  uint8_t bufd[]={0xAF,0x96};
  pControlCharacteristic->setValue(bufd,2);
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

void loop() {
/*
  for (int pos = 0; pos <= 180; pos++) {  // go from 0-180 degrees
    servo0.write(servoPin0, pos);        // set the servo position (degrees)
    servo1.write(servoPin1, pos);        // set the servo position (degrees)
    delay(15);
  }
  for (int pos = 180; pos >= 0; pos--) {  // go from 180-0 degrees
    servo0.write(servoPin0, pos);        // set the servo position (degrees)
    servo1.write(servoPin1, pos);        // set the servo position (degrees)
    delay(15);
  }
*/

    uint8_t* bufct = pControlCharacteristic->getData();

    int pwm_0 = bufct[0];
    int pwm_1 = bufct[1];


    Serial.print("pwm_0: ");
    Serial.print(pwm_0);
    Serial.print(" pwm_1: ");
    Serial.println(pwm_1);

    servo0.write(servoPin0, pwm_0);        // set the servo position (degrees)
    servo0.write(servoPin1, pwm_1);        // set the servo position (degrees)
    delay(15);


}
