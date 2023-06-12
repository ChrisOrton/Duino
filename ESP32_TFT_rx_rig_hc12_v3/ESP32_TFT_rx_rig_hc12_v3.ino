#include <SPI.h>


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;









#define I2C_SDA 23
#define I2C_SCL 18 




#define HC12SET 5  //(RX2)
#define RXD2 17  //(RX2)
#define TXD2 16 //(TX2)
#define HC12 Serial2  //Hardware serial 2 on the ESP32


char incomingByte;
String readBuffer = "";

int t;

TwoWire I2CBME = TwoWire(0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  I2CBME.begin(I2C_SDA, I2C_SCL, 100000);
  
  
  Serial.print("Looking for MPU6050 chip at: ");
  Serial.println(MPU6050_I2CADDR_DEFAULT);
  // Try to initialize!
  if (!mpu.begin(MPU6050_I2CADDR_DEFAULT, &I2CBME)) {
  //if (!mpu.begin(0x68, &I2CBME)) {
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

  t =0;

  pinMode(HC12SET, OUTPUT);
  digitalWrite(HC12SET, LOW);           //Normally HIGH, LOW for settings
  HC12.begin(9600, SERIAL_8N1, RXD2, TXD2);      // Serial port to HC12
  
}

void loop() {
  



    
      // How much time did rendering take (ESP8266 80MHz 271ms, 160MHz 157ms, ESP32 SPI 120ms, 8bit parallel 105ms
      t = millis() - t;

      Serial.print(t); Serial.println(" ms");
      



  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  Serial.println("get event");
  mpu.getEvent(&a, &g, &temp);
  Serial.println("got event");





  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

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


  readBuffer = "";
  boolean start = false;
  // Reads the incoming angle
  while (HC12.available()) {             // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    delay(5);
    // Reads the data between the start "s" and end marker "e"
    if (start == true) {
      if (incomingByte != 'e') {
        readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
      }
      else {
        Serial.print("end");
        start = false;
      }
    }
    // Checks whether the received message statrs with the start marker "s"
    else if ( incomingByte == 's') {
        Serial.print("start");
      start = true; // If true start reading the message
    }
  }
  // Converts the string into integer
  int currentValue1 = readBuffer.toInt();
  Serial.print("currentValue1: ");
  Serial.print(currentValue1);
  

  
  delay(500);

      
}