// Basic demo for accelerometer readings from Adafruit MPU6050

// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
// ESP8266 Guide: https://RandomNerdTutorials.com/esp8266-nodemcu-mpu-6050-accelerometer-gyroscope-arduino/
// Arduino Guide: https://RandomNerdTutorials.com/arduino-mpu-6050-accelerometer-gyroscope/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


#define I2C_SDA 23
#define I2C_SCL 18


#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

int motorPin = 15;
int motorSpeed = 190;


// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

BluetoothSerial SerialBT;

TwoWire I2CBME = TwoWire(0);

bool startSending = 0;

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens


  pinMode(motorPin, OUTPUT);
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(motorPin, pwmChannel);

    

  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");


  Serial.println("Adafruit MPU6050 test!");

  I2CBME.begin(I2C_SDA, I2C_SCL, 100000);


  // Try to initialize!
  if (!mpu.begin(MPU6050_I2CADDR_DEFAULT, &I2CBME, 0  )  ) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float sum = 0;
  /* Print out the values */
  /*
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
*/

  sum = a.acceleration.x * a.acceleration.x;
  sum += a.acceleration.y * a.acceleration.y;
  sum += a.acceleration.z * a.acceleration.z;

/*
  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
*/
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {

     int fred = SerialBT.read();
     if ((fred >96 ) && (fred<123)){
        fred = fred - 32;
     }
      startSending = 1;


    if (fred==80){
      motorSpeed+=5;
    }
    if (fred==77){
      motorSpeed-=5;
    }
    if (fred==83){
      motorSpeed = 200;
    }
    if (fred==90){
      motorSpeed = 0;
    }
    if(motorSpeed<0){
      motorSpeed=0;
    }
    if(motorSpeed>255){
      motorSpeed=255;
    }
    
    SerialBT.write(fred);
  }

  if(startSending){
/*
    SerialBT.print("Acceleration X: ");
    SerialBT.print(a.acceleration.x);
    SerialBT.print(", Y: ");
    SerialBT.print(a.acceleration.y);
    SerialBT.print(", Z: ");
    SerialBT.print(a.acceleration.z);
    SerialBT.println(" m/s^2");
*/

    //analogWrite(motorPin, motorSpeed);
    //ledcWrite(pwmChannel, dutyCycle);
    ledcWrite(pwmChannel, motorSpeed);

    SerialBT.print("Acceleration Squared: ");
    SerialBT.println(sum);
    SerialBT.print("motorSpeed: ");
    SerialBT.println(motorSpeed);

  }

  
  delay(500);
}
