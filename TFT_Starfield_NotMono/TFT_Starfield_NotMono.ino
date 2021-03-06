// Animates white pixels to simulate flying through a star field

#include <SPI.h>
#include <TFT_eSPI.h>

#define TFT_CS1 12
#define TFT_CS2 14

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// With 1024 stars the update rate is ~65 frames per second
#define NSTARS 1024
uint8_t sx[NSTARS] = {};
uint8_t sy[NSTARS] = {};
uint8_t sz[NSTARS] = {};

uint8_t za, zb, zc, zx;

// Fast 0-255 random number generator from http://eternityforest.com/Projects/rng.php:
uint8_t __attribute__((always_inline)) rng()
{
  zx++;
  za = (za^zc^zx);
  zb = (zb+za);
  zc = ((zc+(zb>>1))^za);
  return zc;
}

void setup() {
  za = random(256);
  zb = random(256);
  zc = random(256);
  zx = random(256);

  Serial.begin(115200);
  pinMode(TFT_CS1, OUTPUT);
  digitalWrite(TFT_CS1, LOW);

  pinMode(TFT_CS2, OUTPUT);
  digitalWrite(TFT_CS2, LOW);
  
  tft.init();

  
  digitalWrite(TFT_CS1, HIGH);
  digitalWrite(TFT_CS2, HIGH);
  
  digitalWrite(TFT_CS1, LOW);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  digitalWrite(TFT_CS1, HIGH);

  digitalWrite(TFT_CS2, LOW);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  digitalWrite(TFT_CS2, HIGH);

  // fastSetup() must be used immediately before fastPixel() to prepare screen
  // It must be called after any other graphics drawing function call if fastPixel()
  // is to be called again
  //tft.fastSetup(); // Prepare plot window range for fast pixel plotting
}

void loop()
{
  unsigned long t0 = micros();
  uint8_t spawnDepthVariation = 255;

  for(int i = 0; i < NSTARS; ++i)
  {
    if (sz[i] <= 1)
    {
      sx[i] = 160 - 120 + rng();
      sy[i] = rng();
      sz[i] = spawnDepthVariation--;
    }
    else
    {
      int old_screen_x = ((int)sx[i] - 160) * 256 / sz[i] + 160;
      int old_screen_y = ((int)sy[i] - 120) * 256 / sz[i] + 120;

      // This is a faster pixel drawing function for occassions where many single pixels must be drawn
      //tft.drawPixel(old_screen_x, old_screen_y,TFT_BLACK);
        if (old_screen_x>160){

          digitalWrite(TFT_CS2, LOW);
          tft.drawPixel(old_screen_x, old_screen_y, TFT_BLACK);
          digitalWrite(TFT_CS2, HIGH);
        }else{
          digitalWrite(TFT_CS1, LOW);
          tft.drawPixel(old_screen_x, old_screen_y, TFT_BLACK);
          digitalWrite(TFT_CS1, HIGH);
        }

      sz[i] -= 2;
      if (sz[i] > 1)
      {
        int screen_x = ((int)sx[i] - 160) * 256 / sz[i] + 160;
        int screen_y = ((int)sy[i] - 120) * 256 / sz[i] + 120;
  
        if (screen_x >= 0 && screen_y >= 0 && screen_x < 640 && screen_y < 240)
        {
          uint8_t r, g, b;
          r = g = b = 255 - sz[i];
          if (screen_x>160){

            digitalWrite(TFT_CS2, LOW);
            tft.drawPixel(screen_x, screen_y, tft.color565(r,g,b));
            digitalWrite(TFT_CS2, HIGH);
          }else{
            digitalWrite(TFT_CS1, LOW);
            tft.drawPixel(screen_x, screen_y, tft.color565(r,g,b));
            digitalWrite(TFT_CS1, HIGH);
          }
        }
        else
          sz[i] = 0; // Out of screen, die.
      }
    }
  }
  unsigned long t1 = micros();
  //static char timeMicros[8] = {};

 // Calcualte frames per second
  Serial.println(1.0/((t1 - t0)/1000000.0));
}
