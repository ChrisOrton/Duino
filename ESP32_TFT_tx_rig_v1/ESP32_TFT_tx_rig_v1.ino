#include <SPI.h>



#include <TFT_eSPI.h>





#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;







TFT_eSPI tft = TFT_eSPI();         // Invoke custom library


#define TFT_CS1 12
#define TFT_CS2 14




#define I2C_SDA 23
#define I2C_SCL 18 

int t;

TwoWire I2CBME = TwoWire(0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  //pinMode(TFT_CS1, OUTPUT);
  pinMode(TFT_CS2, OUTPUT);
  //digitalWrite(TFT_CS1, LOW);
  digitalWrite(TFT_CS2, LOW);

    
  tft.begin();
  tft.setRotation(3);
  tft.setTextColor(0xFFFF, TFT_BLUE);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLUE);
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  
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

  
  digitalWrite(TFT_CS2, HIGH);
}

void loop() {
  
  digitalWrite(TFT_CS2, LOW);



    
      // How much time did rendering take (ESP8266 80MHz 271ms, 160MHz 157ms, ESP32 SPI 120ms, 8bit parallel 105ms
      t = millis() - t;

      Serial.print(t); Serial.println(" ms");
      
       //tft.setCursor(10, 10);
      //tft.print("Fudge");




  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  Serial.println("get event");
  mpu.getEvent(&a, &g, &temp);
  Serial.println("got event");



  /* Print out the values */
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.print("Acceleration: ");
  tft.setCursor(10, 40);
  tft.print(a.acceleration.x);
  tft.setCursor(90, 40);
  tft.print(a.acceleration.y);
  tft.setCursor(180, 40);
  tft.print(a.acceleration.z);
  tft.setCursor(250, 50);
  tft.setTextSize(1);
  tft.println("m/s^2");


  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  /* Print out the values */
  tft.setTextSize(2);
  tft.setCursor(10, 60);
  tft.print("Rotation: ");
  tft.setCursor(10, 80);
  tft.print(g.gyro.x);
  tft.setCursor(90, 80);
  tft.print(g.gyro.y);
  tft.setCursor(180, 80);
  tft.print(g.gyro.z);
  tft.setCursor(250, 90);
  tft.setTextSize(1);
  tft.println("rad/s");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  tft.setTextSize(2);
  tft.setCursor(10, 100);
  tft.print("Temperature: ");
  tft.setCursor(10, 120);
  tft.print(temp.temperature);
  tft.setCursor(250, 130);
  tft.setTextSize(1);
  tft.println("degC");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);

      
}
