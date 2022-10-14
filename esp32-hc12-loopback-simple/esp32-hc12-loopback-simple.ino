#define RXD2 16  //(RX2)
#define TXD2 17 //(TX2)
#define HC12 Serial2  //Hardware serial 2 on the ESP32

void setup() 
{
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);           //Normally HIGH, LOW for settings
  Serial.begin(115200);           // Serial port to computer
  HC12.begin(9600, SERIAL_8N1, RXD2, TXD2);      // Serial port to HC12
  digitalWrite(5, HIGH);           //Normally HIGH, LOW for settings
}

void loopBack() 
{
  while (HC12.available()) 
  {        
    // If HC-12 has data
    HC12.write(HC12.read());      // Send the data to Serial monitor
  }
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
