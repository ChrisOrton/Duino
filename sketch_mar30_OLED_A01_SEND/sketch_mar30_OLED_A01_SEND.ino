//www.diyusthad.com

#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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
  Wire.begin();
    // Setup Serial Monitor
  Serial.begin(9600);
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
  oled.println("Hello Joy!"); // text to display
  oled.display();               // show on OLED



 
  // Initialize RadioManager with defaults - 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!RadioManager.init())
    Serial.println("init failed");
}


void loop() {
  
  if(millis() - timer > 500){
      oled.clearDisplay(); // clear display

      oled.setTextSize(2);          // text size
      oled.setTextColor(WHITE);     // text color
      oled.setCursor(0, 0);        // position to display
      oled.println("Joy XY"); // text to display
      
      oled.setTextSize(1);          // text size
      inputVal = analogRead (A0); // Analog Values 0 to 1023
      fixIn = 102300/inputVal - 100;
      oled.setCursor(0, 20);        // position to display
      oled.println(fixIn); // text to display
      inputVal = analogRead (A1); // Analog Values 0 to 1023
      fixIn2 = 102300/inputVal - 100;
      oled.setCursor(0, 30);        // position to display
      oled.println(fixIn2); // text to display
      oled.display();

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
    Serial.println("sendtoWait failed");

      
      timer = millis();
  }
}
