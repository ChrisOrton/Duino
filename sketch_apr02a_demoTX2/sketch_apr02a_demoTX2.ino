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

  //pinMode(2, INPUT);    // sets the digital pin 7 as input
  //pinMode(3, INPUT);    // sets the digital pin 7 as input

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  
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
  long fix2;
  long fix3;
  
  payload.data1 = bNum++;
  payload.data2 = bNum++;
  payload.data3 = bNum++;
  payload.data4 = bNum++;

  int inputVal0 = analogRead (A0); // Analog Values 0 to 1023
  int inputVal1 = analogRead (A1); // Analog Values 0 to 1023
  int inputVal2 = analogRead (A2); // Analog Values 0 to 1023
  int inputVal3 = analogRead (A3); // Analog Values 0 to 1023
/*
  fix0 = 102300/inputVal0 - 100;
  fix1 = 102300/inputVal1 - 100;
  fix2 = 102300/inputVal2 - 100;
  fix3 = 102300/inputVal3 - 100;
*/
  fix0 = 12000/inputVal0 - 20;
  fix1 = 12000/inputVal1 - 10;
  fix2 = 70000/inputVal2 - 80;
  fix3 = 70000/inputVal3 - 80;


  if (fix0 < 0) fix0 = 0;
  if (fix1 < 0) fix1 = 0;
  if (fix0 >255) fix0 = 255;
  if (fix1 >255) fix1 = 255;

  
  payload.data1 = fix0;
  payload.data2 = fix1;
  payload.data3 = digitalRead(2);
  payload.data4 = digitalRead(3);

  
  radio.write(&payload, sizeof(payload));
  //Serial.print("Data1:");
  //Serial.println(payload.data1);
  //Serial.print("Data2:");
  //Serial.println(payload.data2);
  //Serial.print("Data3:");
  //Serial.println(payload.data3);
  //Serial.print("Data4:");
  //Serial.println(payload.data4);
  Serial.println("Sent");
  Serial.println(fix0);
  Serial.println(fix1);
  Serial.println(fix2);
  Serial.println(fix3);
  delay(INTERVAL_MS_TRANSMISSION);
}
