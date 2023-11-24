//
//    FILE: AS5600_demo.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//    DATE: 2022-05-28


#include "AS5600.h"
#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float OLEDTimer = 0; //Timer for the screen refresh

AS5600 as5600;   //  use default Wire

/* Setting PWM Properties */
const int PWMFreq = 50; /* 5 KHz */
const int PWMChannel = 0;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

// Define the control inputs
#define MOT_A1_PIN D0
#define MOT_A2_PIN D1
#define MOT_B1_PIN D2
#define MOT_B2_PIN D3


void setup()
{
  Serial.begin(115200);
  delay(500); // Pause for 0.5 second


  // Set all the motor control inputs to OUTPUT
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
  pinMode(MOT_B1_PIN, OUTPUT);
  pinMode(MOT_B2_PIN, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);
  digitalWrite(MOT_B1_PIN, LOW);
  digitalWrite(MOT_B2_PIN, LOW);

  ledcSetup(0, PWMFreq, PWMResolution);
  ledcSetup(1, PWMFreq, PWMResolution);
  ledcSetup(2, PWMFreq, PWMResolution);
  ledcSetup(3, PWMFreq, PWMResolution);  

  ledcAttachPin(MOT_A1_PIN, 0);
  ledcAttachPin(MOT_A2_PIN, 1);
  ledcAttachPin(MOT_B1_PIN, 2);
  ledcAttachPin(MOT_B2_PIN, 3);



  Serial.println(F("SSD1306 allocation attempt"));
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  
  
  
  Serial.println(__FILE__);
  Serial.print("AS5600_LIB_VERSION: ");
  Serial.println(AS5600_LIB_VERSION);
  delay(5000);
  //  ESP32
    as5600.begin(D4,D5);
  //  AVR
  //as5600.begin(D7);  //  set direction pin.
  //as5600.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
  int b = as5600.isConnected();
  Serial.print("Connect: ");
  Serial.println(b);

  display.display();
  
  delay(1000);

}


int i =0;
void loop()
{
  //  Serial.print(millis());
  //  Serial.print("\t");
  
  Serial.print(as5600.readAngle());
  //Serial.print("\t");
  //Serial.println(as5600.rawAngle());
  float fang = as5600.rawAngle() * AS5600_RAW_TO_DEGREES;
  //Serial.println(fang);

  //testdrawSegment((int)fang, i);
  i = i+1;

/**/
  float fArc = 90;
  float fAngModArc = fmod(fang, fArc);
  if (fAngModArc < (fArc/16)){

    int iPwm = (int)(255.0 * ((fArc/4) - fAngModArc) / (fArc/4));
    wrapLedCWrite(0, 0);
    wrapLedCWrite(1, 0);
    
    drawDebug(fang, fAngModArc, iPwm);
    
  }else if (fAngModArc < (fArc/4)){

    int iPwm = (int)(255.0 * ((fArc/4) - fAngModArc) / (fArc/4));
    wrapLedCWrite(0, iPwm);
    wrapLedCWrite(1, 0);
    
    drawDebug(fang, fAngModArc, iPwm);
    
  }else if (fAngModArc > (fArc*15/16)){
    //int iPwm = (int)(255.0 * (fArc - fAngModArc) / (fArc/4));
    int iPwm = (int)(255.0 * (fAngModArc - (fArc* 3/4)   ) / (fArc/4));

    wrapLedCWrite(0, 0);
    wrapLedCWrite(1, 0);
    drawDebug(fang, fAngModArc, -iPwm);
  }else if (fAngModArc > (fArc*3/4)){
    //int iPwm = (int)(255.0 * (fArc - fAngModArc) / (fArc/4));
    int iPwm = (int)(255.0 * (fAngModArc - (fArc* 3/4)   ) / (fArc/4));

    wrapLedCWrite(0, 0);
    wrapLedCWrite(1, iPwm);
    drawDebug(fang, fAngModArc, -iPwm);
  } else{

    int iPwm = 0;

    wrapLedCWrite(0, 0);
    wrapLedCWrite(1, 0);
    drawDebug(fang, fAngModArc, iPwm);
    
  }
/**/
  //delay(20);
}

void wrapLedCWrite(int channel, int iPwm){

    //ledcWrite(channel, iPwm);
  
}


void drawDebug(float angle, float modAngle,int  iPwm) {
    char str[80];
  if (millis() - OLEDTimer > 100) //chech if we will update at every 100 ms
  { 
    
    display.clearDisplay();
    
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Ang"));
    display.setCursor(56,0);             // Start at top-left corner
    sprintf(str, "%.2f", angle);
    display.println(str);
    
    display.setCursor(0,20);             // Start at top-left corner
    display.println(F("Mod"));
    display.setCursor(64,20);             // Start at top-left corner
    sprintf(str, "%.2f", modAngle);
    display.println(str);

    display.setCursor(0,40);             // Start at top-left corner
    display.println(F("PWM"));
    display.setCursor(64,40);             // Start at top-left corner
    display.println(iPwm);


    
    display.display();
    OLEDTimer = millis(); //reset timer   
  }
}



void testdrawSegment(int show, int pulse) {
  
    display.clearDisplay();
    //display.flipScreenVertically();

    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Angle"));
    display.setCursor(72,0);             // Start at top-left corner
    display.println(pulse);
    
    drawSegNumber(show);
    display.display();
}


void testdrawSegments(void) {
  for (int i=0 ; i<1000; i++){
    display.clearDisplay();
    //display.flipScreenVertically();

    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(10,0);             // Start at top-left corner
    display.println(F("Rounds"));
    
    drawSegNumber(i);
    display.display();
    delay(300);
  }
  delay(2);
}

int segCodes[10] = { 
  B01111110,          // 0
  B00110000,          // 1
  B01101101,          // 2
  B01111001,          // 3
  B00110011,          // 4
  B01011011,          // 5
  B01011111,          // 6
  B01110000,          // 7
  B01111111,          // 8
  B01111011,          // 9
};

void drawSegDigit(int d, int x){
    int lineWidth = 3;
    int segCode = segCodes[d];

    if (segCode & B01000000)
      display.fillRoundRect(x + 12,18, 16, lineWidth, 2, SSD1306_WHITE);    // A
    if (segCode & B00100000)
      display.fillRoundRect(x + 30,20, lineWidth, 16, 2, SSD1306_WHITE);    // B
    if (segCode & B00010000)
      display.fillRoundRect(x + 30,41, lineWidth, 16, 2, SSD1306_WHITE);    // C
    if (segCode & B00001000)
      display.fillRoundRect(x + 12,58, 16, lineWidth, 2, SSD1306_WHITE);    // D
    if (segCode & B00000100)
      display.fillRoundRect(x + 8,41, lineWidth, 16, 2, SSD1306_WHITE);     // E
    if (segCode & B00000010)
      display.fillRoundRect(x + 8,20, lineWidth, 16, 2, SSD1306_WHITE);     // F
    if (segCode & B00000001)
      display.fillRoundRect(x + 12,38, 16, lineWidth, 2, SSD1306_WHITE);    // G 
}

void drawSegNumber(int x){
    int y = x;
    int posx = 80;
    while (y > 0){
        drawSegDigit(y %10, posx);
        y = y /10;
        posx = posx -35;
    }
}

// -- END OF FILE --
