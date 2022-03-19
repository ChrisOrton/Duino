
#include <Wire.h>

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop()
{
  
  
  
  Wire.beginTransmission(4); // transmit to device #4
  
    Wire.write(2);              // sends one byte  
    Wire.write(0);              // sends one byte  
      Wire.write(8);              // sends one byte  
  
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(500);
}
