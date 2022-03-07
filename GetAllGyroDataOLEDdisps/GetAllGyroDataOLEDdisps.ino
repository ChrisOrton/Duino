
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


MPU6050 mpu6050(Wire);

long timer = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();



  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(2);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 0);        // position to display
  oled.println("Pls WAIT"); // text to display
  oled.setTextSize(1);          // text size
  oled.setCursor(0, 20);        // position to display
  oled.println("Hola Mundo!"); // text to display
  oled.setCursor(0, 30);        // position to display
  oled.println("Dia duit ar domhan!"); // text to display
  oled.setCursor(0, 40);        // position to display
  oled.println("Hola kilo!"); // text to display
  oled.setCursor(0, 50);        // position to display
  oled.println("Hello World!"); // text to display
  oled.display();               // show on OLED



  mpu6050.begin();
  
  mpu6050.calcGyroOffsets(true);
}

void loop() {
    loopSpeed();

}

void loopSpeed() {
  float gyroX;
  float gyroY;
  float gyroZ;
  
  mpu6050.update();

  if(millis() - timer > 500){
   
    

    
    oled.clearDisplay(); // clear display
    oled.setTextSize(2);          // text size
    oled.setTextColor(WHITE);     // text color


    gyroX = mpu6050.getGyroX();
    gyroY = mpu6050.getGyroY();
    gyroZ = mpu6050.getGyroZ();

    float mpsf = sqrt((gyroX * gyroX) + (gyroX * gyroX) + (gyroX * gyroX));
    int mps = mpsf;
    float kphf = mpsf * 60.0 * 60.0 /1000.0;
    int kph = kphf;
        
    oled.setCursor(0, 0);        // position to display
    oled.println("m ps"); // text to display
    oled.setCursor(64, 0);        // position to display
    oled.println(mps); // text to display

    oled.setCursor(0, 20);        // position to display
    oled.println("KPH"); // text to display
    oled.setCursor(64, 20);        // position to display
    oled.println(kph); // text to display
    
    oled.setTextSize(1);          // text size
    oled.setCursor(0, 56);        // position to display
    oled.println(gyroX); // text to display
    oled.setCursor(40, 56);        // position to display
    oled.println(gyroY); // text to display
    oled.setCursor(80, 56);        // position to display
    oled.println(gyroZ); // text to display
    
    oled.display();               // show on OLED    
    timer = millis();


    
  }
}
void loopBig() {
  float angleX;
  float angleY;
  float angleZ;
  
  mpu6050.update();

  if(millis() - timer > 500){
   
    

    
    oled.clearDisplay(); // clear display
    oled.setTextSize(2);          // text size
    oled.setTextColor(WHITE);     // text color

    oled.setCursor(0, 0);        // position to display
    oled.println("Pitch"); // text to display
    oled.setCursor(64, 0);        // position to display
    oled.println(mpu6050.getAngleX()); // text to display


    oled.setTextSize(1);          // text size
    
    oled.setCursor(0, 20);        // position to display
    oled.println("Roll"); // text to display
    oled.setCursor(38, 20);        // position to display
    oled.println(mpu6050.getAngleY()); // text to display
    
    
    oled.setCursor(0, 32);        // position to display
    oled.println("Hdg"); // text to display
    oled.setCursor(38, 32);        // position to display
    oled.println(mpu6050.getAngleZ()); // text to display
    
    
    oled.setCursor(0, 45);        // position to display
    oled.println(mpu6050.getAccX()); // text to display
    oled.setCursor(43, 45);        // position to display
    oled.println(mpu6050.getAccY()); // text to display
    oled.setCursor(86, 45);        // position to display
    oled.println(mpu6050.getAccZ()); // text to display
    
    oled.setCursor(0, 55);        // position to display
    oled.println(mpu6050.getGyroX()); // text to display
    oled.setCursor(43, 55);        // position to display
    oled.println(mpu6050.getGyroY()); // text to display
    oled.setCursor(86, 55);        // position to display
    oled.println(mpu6050.getGyroZ()); // text to display
    
    oled.display();               // show on OLED    
    timer = millis();


    
  }

}
