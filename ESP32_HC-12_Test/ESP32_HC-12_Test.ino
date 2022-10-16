#define RXD2 17  //(RX2)
#define TXD2 16 //(TX2)


#include <SoftwareSerial.h>
//SoftwareSerial HC12(TXD2,RXD2); // HC-12 TX Pin, HC-12 RX Pin
SoftwareSerial HC12;




void setup() 
{
  Serial.begin(9600);           // Serial port to computer
  delay(1000);
  
  Serial.println("Initilise");

  
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);           //Normally HIGH, LOW for settings
  delay(1000);
  
  Serial.println("Initilised");
  //HC12.begin(9600);      // Serial port to HC12
  HC12.begin(9600, SWSERIAL_8N1, RXD2, TXD2, false);
  //HC12.begin(9600, SERIAL_8N1, RXD2, TXD2);      // Serial port to HC12


  Serial.println("Initilised");
}

void loop() 
{
  while (HC12.available()) 
  {        
    // If HC-12 has data
    Serial.write(HC12.read());      // Send the data to Serial monitor
  }
  while (Serial.available()) 
  {      
    // If we have data from Serial monitor
    HC12.write(Serial.read());      // Send that data to HC-12
  }
}
