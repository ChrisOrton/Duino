// Simple I2C test for ebay 128x64 oled.
// Use smaller faster AvrI2c class in place of Wire.
// Edit AVRI2C_FASTMODE in SSD1306Ascii.h to change the default I2C frequency.
//
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiAvrI2c oled;

// Include RadioHead ReliableDatagram & NRF24 Libraries
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
 
// Include dependant SPI Library 
#include <SPI.h>
 
// Define addresses for radio channels
#define CLIENT_ADDRESS 1   
#define SERVER_ADDRESS 2
 
// Create an instance of the radio driver
RH_NRF24 RadioDriver;
 
// Sets the radio driver to NRF24 and the client address to 1
RHReliableDatagram RadioManager(RadioDriver, CLIENT_ADDRESS);

// Declare unsigned 8-bit joystick array
uint8_t joystick[3]; 

// Define the Message Buffer
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

long timer = 0;

int inputVal = 0;
long fixIn = 0;
long fixIn2 = 0;

void setup() {
  Serial.begin(9600);

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.

  oled.setFont(Adafruit5x7);
  
  
  

  delay(2000);         // wait for initializing
  uint32_t m = micros();
  oled.clear(); // clear display
  oled.println("Hello world!");
  oled.println("A long line may be truncated");
  oled.println();
  oled.set2X();
  oled.println("2X demo");
  oled.set1X();
  oled.print("\nmicros: ");
  oled.print(micros() - m);



 
  // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  Serial.println("TX Init");
  if (!RadioManager.init())
    Serial.println("TX init failed");
}


void loop() {
  
  oled.clear(); // clear display
  oled.println("Joy XY"); // text to display
  
  inputVal = analogRead (A0); // Analog Values 0 to 1023
  fixIn = 102300/inputVal - 100;
  oled.setCursor(0, 20);        // position to display
  oled.println(fixIn); // text to display
  inputVal = analogRead (A1); // Analog Values 0 to 1023
  fixIn2 = 102300/inputVal - 100;
  oled.setCursor(0, 30);        // position to display
  oled.println(fixIn2); // text to display

  // Read Joystick values and map to values of 0 - 255
  joystick[0] = map(fixIn, 0, 255, 0, 255);
  joystick[1] = map(fixIn2, 0, 255, 0, 255);
  joystick[2] = 100;


  //Send a message containing Joystick data to manager_server
  if (RadioManager.sendtoWait(joystick, sizeof(joystick), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (RadioManager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("No reply, is nrf24_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("TX sendtoWait failed");

  delay(100);  // Wait a bit before next transmission

}
