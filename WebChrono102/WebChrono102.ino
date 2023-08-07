#include <WiFi.h>
#include <WebServer.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
/*Put your SSID & Password*/
const char* ssid = "cccc";  // Enter SSID here
const char* password = "ccccccc";  //Enter Password here
 
WebServer server(80);




// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   2



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool inFlight = false;
int inPin0 = 2;
int inPin1 = 3;

int tstart = 0;
int tstop = 0;
int tdelta = 0;

int roundCount = 0;

 
 
float Temperature = 411;
float Humidity= 111;
 
void setup() 
{
  Serial.begin(115200);
  delay(100);
  
  Serial.println("Connecting to ");
  Serial.println(ssid);
 
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
 
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
 
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
 
  server.begin();
  Serial.println("HTTP server started");


  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  pinMode(inPin0, INPUT);    // sets the digital pin 7 as input
  pinMode(inPin1, INPUT);    // sets the digital pin 7 as input
  attachInterrupt(digitalPinToInterrupt(inPin0),buttonStart,FALLING); 
  attachInterrupt(digitalPinToInterrupt(inPin1),buttonStop,FALLING); 

  int roundCount = 0;

  
 
}




void buttonStart()          
{    
  tstart = micros();
  inFlight = true;               
  roundCount++;
}


void buttonStop()          
{                                 
   tstop = micros();              
   tdelta = tstop -tstart;
}





void loop() {
  
  server.handleClient();
  textloop();
  
}




void textloop() {

  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);

  display.setCursor(60, 0);
  if (digitalRead(inPin0)){
    display.println(F("Z  On"));
    
  }else{
    display.println(F("Z Off")); 
  }

  display.setCursor(0, 0);
  if (digitalRead(inPin1)){
    display.println(F("O  On"));
    
  }else{
    display.println(F("O Off"));

  }

  display.setTextSize(1);
  display.setCursor(0, 20);

  display.setTextSize(1);
  display.println(WiFi.localIP());
  display.setTextSize(2);

  
  display.setCursor(10, 30);

  char strBuf[50];
  sprintf(strBuf, "%duS", tdelta);
  
 display.println(strBuf);


  int fps = 0;
  if (tdelta >100){

    fps = 328100 /tdelta;
  }
  display.setCursor(10, 50);
  sprintf(strBuf, "%dfps", fps);
  display.println(strBuf);
  
  display.display();      // Show initial text
}


 
void handle_OnConnect() {
 
 Temperature = 411; // Gets the values of the temperature
  Humidity = 333; // Gets the values of the humidity 

  int fps = 0;
  if (tdelta >100){

    fps = 328100 /tdelta;
  }

  
  server.send(200, "text/html", SendHTML((float)fps,(float)roundCount)); 
}
 
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
 
String SendHTML(float Temperaturestat,float Humiditystat)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta http-equiv=\"refresh\" content=\"1\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Webserver</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 Webserver</h1>\n";
  
  ptr +="<p>Feet Per Second: ";
  ptr +=(int)Temperaturestat;
  ptr +=" fps</p>";
  ptr +="<p>Round Count: ";
  ptr +=(int)Humiditystat;
  ptr +=" rounds</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
