#define RXD2 16  //(RX2)
#define TXD2 17 //(TX2)
#define HC12 Serial2  //Hardware serial 2 on the ESP32

#include <ESP32Servo.h>
 
Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32
 
int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 13;

typedef enum _byteCodes
{
 none = ' ',
  Servo  = 's',
  End    = 'e',
  Value  = 'v',
  RunCheck = 'c',
  RunLoop  = 'l',
  RunTerminal  ='t' 
} ByteCodes;

void emptyLoop(void);
void getServo(void);
void getValue(void);
void servoTest(void);
void loopBack(void);
void loopTerminal(void);

typedef enum _stateModes
{
  none = 0,
  GetServo  = 1,
  GotEnd    = 2,
  GetValue  = 3,
  ServoTest = 4,
  Loopback  = 5,
  Terminal  = 6
} StateMode;

void (*loopPointer[])(void) = {
  emptyLoop,
  getServo,
  getValue,
  servoTest,
  loopBack,
  loopTerminal
};

StateMode modeNow = none;

void setup() 
{
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);           //Normally HIGH, LOW for settings
  Serial.begin(115200);           // Serial port to computer
  HC12.begin(9600, SERIAL_8N1, RXD2, TXD2);      // Serial port to HC12
  digitalWrite(5, HIGH);           //Normally HIGH, LOW for settings


  modeNow = Terminal
}


StateMode newMode( ByteCode code){
  
  StateMode nextMode = none;
  switch( code )
  {
    case Servo:
        nextMode = GetServo;
        Break;
    case End:
        nextMode = GotEnd;
        Break;
    case Value:
        nextMode = GetValue;
        Break;
    case RunCheck:
        nextMode = ServoTest;
        Break;
    case RunLoop:
        nextMode = LoopBack;
        Break;
    case RunTerminal:
        nextMode = Teminal;
        Break;
    default:
        nextMode = none;
        Break;
  }
  return nextMode;
  
}



void emptyLoop(){
  while (HC12.available()) 
  {        
    // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    delay(5);
    modeNow = newMode(incomingByte);
    return;
  }
}


int servoNumber = 0;
void getServo(){
    while (HC12.available()) 
    {        
      // If HC-12 has data
      incomingByte = HC12.read();          // Store each icoming byte from HC-12
      delay(5);
      // Reads the data between the start "s" and end marker "e"
        if ((incomingByte >= '0') &&  (incomingByte <= '9')) {
          readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
        } else {
          servoNumber = readBuffer.toInt();
          modeNow = newMode(incomingByte);
          return;
        }
    }
}

int servoValue = 0;
void getValue(){
    while (HC12.available()) 
    {        
      // If HC-12 has data
      incomingByte = HC12.read();          // Store each icoming byte from HC-12
      delay(5);
      // Reads the data between the start "s" and end marker "e"
        if ((incomingByte >= '0') &&  (incomingByte <= '9')) {
          readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
        } else {
          servoValue = readBuffer.toInt();
          modeNow = newMode(incomingByte);
          return;
        }
    }
}


currentValue = 0;
void servoTest(){
	int minValue = -2000;
	int maxValue =  2000;
	int servoValue=  0;
	if (currentvalue <0)
		servoValue = map(currentvalue, minValue, 0, 180, 0);
	else
		servoValue = map(currentvalue, 0,maxvalue, 180,0);

	/// NOW WRITE OUT servoValue

	if (++servoValue > maxValue)
		servoValue = minValue;

	delay(1);
}


void loopBack() 
{
  while (HC12.available()) 
  {        
    // If HC-12 has data
    HC12.write(HC12.read());      // Send the data to Serial monitor
  }
}


void loopTerminal() 
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
void loop() 
{
	// loopTerminal();
	loopPointer[modeNow]();
}
