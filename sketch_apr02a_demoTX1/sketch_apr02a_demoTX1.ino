#include "SPI.h"
#include "RF24.h"
#include "nRF24L01.h"
#define CE_PIN 9
#define CSN_PIN 10
// #define INTERVAL_MS_TRANSMISSION 250
#define INTERVAL_MS_TRANSMISSION 10

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "17001";
//NRF24L01 buffer limit is 32 bytes (max struct size)
struct payload {
  byte data1;
  byte data2;
  byte data3;
  byte data4;
};


byte bNum = 0;

payload payload;
void setup()
{
  Serial.begin(115200);
  radio.begin();
  //Append ACK packet from the receiving radio back to the transmitting radio
  radio.setAutoAck(false); //(true|false)
  //Set the transmission datarate
  //radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS)
  radio.setDataRate(RF24_2MBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS)
  //Greater level = more consumption = longer distance
  radio.setPALevel(RF24_PA_MAX); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX)
  //Default value is the maximum 32 bytes
  radio.setPayloadSize(sizeof(payload));
  //Act as transmitter
  radio.openWritingPipe(address);
  radio.stopListening();
}
void loop()
{
  long fix0;
  long fix1;
  
  payload.data1 = bNum++;
  payload.data2 = bNum++;
  payload.data3 = bNum++;
  payload.data4 = bNum++;

  int inputVal = analogRead (A0); // Analog Values 0 to 1023
  fix0 = 102300/inputVal - 100;
  inputVal = analogRead (A1); // Analog Values 0 to 1023
  fix1 = 102300/inputVal - 100;
  payload.data1 = fix0;
  payload.data2 = fix1;
  
  radio.write(&payload, sizeof(payload));
  Serial.print("Data1:");
  Serial.println(payload.data1);
  Serial.print("Data2:");
  Serial.println(payload.data2);
  Serial.println("Sent");
  delay(INTERVAL_MS_TRANSMISSION);
}
