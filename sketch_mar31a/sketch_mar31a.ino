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

/*
  nRF24L01+ Joystick Receiver Demo
  nrf24l01-joy-rcv-demo.ino
  nRF24L01+ Receiver with Joystick Decode
  Use with Joystick Transmitter Demo
  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/
 
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
 
// Sets the radio driver to NRF24 and the server address to 2
RHReliableDatagram RadioManager(RadioDriver, SERVER_ADDRESS);
 
// Define a message to return if values received
uint8_t ReturnMessage[] = "JoyStick Data Received"; 
 
// Define the Message Buffer
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
 
void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600);
  Serial.println("init");

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.

  oled.setFont(Adafruit5x7);

  uint32_t m = micros();
  oled.clear();
  oled.println("Hello rx!");
  oled.println("A long line may be truncated");
  oled.println();
  oled.set2X();
  oled.println("2X demo");
  oled.set1X();
  oled.print("\nmicros: ");
  oled.print(micros() - m);

  
  // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  Serial.println("radio init");
  if (!RadioManager.init())
    Serial.println("init failed");
} 
 
void loop()
{
  
  if (RadioManager.available())
  {
    Serial.println("radio loop");
 // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (RadioManager.recvfromAck(buf, &len, &from))
 //Serial Print the values of joystick
    {
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": X = ");
      Serial.println(buf[0]);
      Serial.print(" Y = ");
      Serial.print(buf[1]);
      Serial.print(" Z = ");
      Serial.println(buf[2]);

      oled.clear();
      oled.print(from, HEX);
      oled.print(": X = ");
      oled.println(buf[0]);
      oled.print(" Y = ");
      oled.println(buf[1]);
      oled.print(" Z = ");
      oled.println(buf[2]);

      // Send a reply back to the originator client, check for error
      if (!RadioManager.sendtoWait(ReturnMessage, sizeof(ReturnMessage), from))
        Serial.println("sendtoWait failed");
    }
  }              
}
