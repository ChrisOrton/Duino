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

// RC stuff follows
#include "SPI.h"
#include "RF24.h"
#include "nRF24L01.h"

#define CE_PIN 9
#define CSN_PIN 10

#define INTERVAL_MS_SIGNAL_LOST 2000
#define INTERVAL_MS_SIGNAL_RETRY 250

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "17001";

//NRF24L01 buffer limit is 32 bytes (max struct size)
struct payload {
  byte data1;
  byte data2;
  byte data3;
  byte data4;
};

payload payload;

unsigned long lastSignalMillis = 0;

void setup()
{
  Serial.begin(115200);
  Serial.print("Setup");


  #if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.

  oled.setFont(Adafruit5x7);
  oled.clear(); // clear display
  oled.println("Setup!");

  //RC stuff Follows
  
  radio.begin();

  //Append ACK packet from the receiving radio back to the transmitting radio
  radio.setAutoAck(false); //(true|false)
  //Set the transmission datarate
  //radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS)
  radio.setDataRate(RF24_2MBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS)
  //Greater level = more consumption = longer distance
  radio.setPALevel(RF24_PA_MAX); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX)
  //Default value is the maximum 32 bytes1
  radio.setPayloadSize(sizeof(payload));
  //Act as receiver
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop()
{
  unsigned long currentMillis = millis();

  if (radio.available() > 0) {
    radio.read(&payload, sizeof(payload));

    Serial.println("Received");

    Serial.print("Data1:");
    Serial.println(payload.data1);

    Serial.print("Data2:");
    Serial.println(payload.data2);

      oled.clear(); // clear display
      oled.println("RX!");
      oled.print("Data1:");
      oled.println(payload.data1);
      oled.print("Data2:");
      oled.println(payload.data2);
      oled.print("Data3:");
      oled.println(payload.data3);
      oled.print("Data4:");
      oled.println(payload.data4);

    lastSignalMillis = currentMillis;
  }

  if (currentMillis - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST) {
    lostConnection();
  }
}

void lostConnection()
{
  //Serial.println("We have lost connection, preventing unwanted behavior");

  delay(INTERVAL_MS_SIGNAL_RETRY);
}
