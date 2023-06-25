// Simple demonstration on using an input device to trigger changes on your
// NeoPixels. Wire a momentary push button to connect from ground to a
// digital IO pin. When the button is pressed it will change to a new pixel
// animation. Initial state has all pixels off -- press the button once to
// start the first animation. As written, the button does not interrupt an
// animation in-progress, it works only when idle.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   2

#define PIXEL_PIN    6  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 22  // Number of NeoPixels

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

boolean oldState = HIGH;
int     mode     = 0;    // Currently-active animation mode, 0-9

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
}

void loop() {
  // Get current button state.
  boolean newState = digitalRead(BUTTON_PIN);



  colorWipeT(strip.Color(  0,   0,   0), 50);    // Black/off

  // Check if state changed from high to low (button press).
  if((newState == LOW) && (oldState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if(newState == LOW) {      // Yes, still low
      if(++mode > 8) mode = 0; // Advance to next mode, wrap around after #8
      switch(mode) {           // Start the new animation...
        case 0:
          colorWipeC(strip.Color(  0,   0,   0), 50);    // Black/off
          break;
        case 1:
          colorWipeC(strip.Color(255,   0,   0), 50);    // Red
          break;
        case 2:
          colorWipeC(strip.Color(  0, 255,   0), 50);    // Green
          break;
        case 3:
          colorWipeC(strip.Color(  0,   0, 255), 50);    // Blue
          break;
        case 4:
          colorWipeC(strip.Color(  0,   0, 255), 50);    // Blue
          
          break;
        case 5:
          colorWipeC(strip.Color(  0,   0, 255), 50);    // Blue
          
          break;
        case 6:
          colorWipeC(strip.Color(  0,   0, 255), 50);    // Blue
          
          break;
        case 7:
          colorWipeC(strip.Color(  0,   0, 255), 50);    // Blue
          
          break;
        case 8:
          colorWipeC(strip.Color(  0,   0, 255), 50);    // Blue
          
          break;
      }
    }
  }

  // Set the last-read button state to the old state.
  oldState = newState;
}


void clipLED(int pixel, uint32_t color){
    if ((pixel >=0) && (pixel <11)){
      strip.setPixelColor(pixel, color);
      strip.setPixelColor(21-pixel, color);
    }
  
}


void colorWipeT(uint32_t color, int wait) {
  
  uint32_t  color0 = strip.Color(  7,   0, 7);
  uint32_t  color1 = strip.Color(  15,   0, 15);
  uint32_t  color2 = strip.Color(  31,   0, 31);
  uint32_t  color3 = strip.Color(  63,   0, 63);

  int modFactor =15;
  
  for(int i= 11; i<strip.numPixels() * 10; i++) { // For each pixel in strip...
    strip.clear();         //   Set all pixels in RAM to 0 (off)
    clipLED((i-3) % modFactor, color0);         //  Set pixel's color (in RAM)    
    clipLED((i-2) % modFactor, color1);         //  Set pixel's color (in RAM)    
    clipLED((i-1) % modFactor, color2);         //  Set pixel's color (in RAM)
    clipLED(i % modFactor, color3);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(50);                           //  Pause for a moment
  }
}


void colorWipeC(uint32_t color, int wait) {
  
  uint32_t  color0 = strip.Color(  7,   0, 7);
  uint32_t  color1 = strip.Color(  15,   0, 15);
  uint32_t  color2 = strip.Color(  31,   0, 31);
  uint32_t  color3 = strip.Color(  63,   0, 63);
  uint32_t  color4 = strip.Color(  31,   0, 31);
  uint32_t  color5 = strip.Color(  15,   0, 15);

/*
  uint32_t  color0 = strip.Color(255,   0, 7);
  uint32_t  color1 = strip.Color(127,   0, 15);
  uint32_t  color2 = strip.Color( 63,   0, 31);
  uint32_t  color3 = strip.Color( 31,   0, 63);
  uint32_t  color4 = strip.Color( 15,   0, 127);
  uint32_t  color5 = strip.Color(  7,   0, 255);
*/
/*
  uint32_t  color0 = strip.Color(255,   0, 0);
  uint32_t  color1 = strip.Color(127,   0, 0);
  uint32_t  color2 = strip.Color( 63,   0, 0);
  uint32_t  color3 = strip.Color( 31,   0, 0);
  uint32_t  color4 = strip.Color( 15,   0, 0);
  uint32_t  color5 = strip.Color(  7,   0, 0);
*/
  
  for(int i=strip.numPixels(); i<strip.numPixels() * 10; i++) { // For each pixel in strip...
    strip.clear();         //   Set all pixels in RAM to 0 (off)
    strip.setPixelColor((i-2) % strip.numPixels(), color0);         //  Set pixel's color (in RAM)    
    strip.setPixelColor((i-1) % strip.numPixels(), color1);         //  Set pixel's color (in RAM)    
    strip.setPixelColor(i % strip.numPixels(), color2);         //  Set pixel's color (in RAM)
    strip.setPixelColor((i+1) % strip.numPixels(), color3);         //  Set pixel's color (in RAM)
    //strip.setPixelColor((i+2) % strip.numPixels(), color4);         //  Set pixel's color (in RAM)
    //strip.setPixelColor((i+3) % strip.numPixels(), color5);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
