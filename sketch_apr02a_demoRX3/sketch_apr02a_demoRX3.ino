// servo
// Include the servo library:
#include <Servo.h>

// Create a new servo object:
Servo myservo1;
Servo myservo2;

// Define the servo pin:
#define servoPin1 3
#define servoPin2 4

// Create a variable to store the servo position:
int angle1 = 0;
int angle2 = 0;

bool select = false;

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




  // Attach the Servo variable to a pin:
  myservo1.attach(servoPin1);
  myservo2.attach(servoPin2);


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


      select = !select;

      angle1 = payload.data1;
      if (angle1 >180) angle1 = 180;
      if (angle1 <0) angle1 = 0;
      

      angle2 = payload.data2;
      if (angle2 >180) angle2 = 180;
      if (angle2 <0) angle2 = 0;
      
      //if (select)
        myservo1.write(angle1);
      //else
        myservo2.write(angle2);


    lastSignalMillis = currentMillis;
  }

  if (currentMillis - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST) {
    lostConnection();
    lastSignalMillis = currentMillis;
  }
}

void lostConnection()
{
  Serial.println("We have lost connection, preventing unwanted behavior");

  delay(INTERVAL_MS_SIGNAL_RETRY);
}
