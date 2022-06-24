/********************************************************************
* tgx library example : displaying a 3D mesh...
*
*                        EXAMPLE FOR ESP32
*
* Uses Bodmer's TFT_eSPI library : https://github.com/Bodmer/TFT_eSPI
*
* Tested with an ESP32 dev module and an ILI9341 screen
********************************************************************/

// screen driver library
#include <TFT_eSPI.h>
#define CS_1 14
#define CS_2 12

// graphic library
#include <tgx.h>

// the mesh to draw
#include "naruto.h"

// let's not burden ourselves with the tgx:: prefix
using namespace tgx;

// the screen driver
TFT_eSPI tft = TFT_eSPI();

// size of the drawing framebuffer
// (limited by the amount of memory in the ESP32). 
//#define SLX 140
//#define SLY 200
//#define SLX 270
//#define SLY 200
//#define SLX 300
//#define SLY 200
#define SLX 320
#define SLY 240

// the framebuffer we draw onto
//uint16_t fb[SLX * SLY];
uint16_t* fb;

// second framebuffer used by eSPI_TFT for DMA update
// allocated via malloc
uint16_t* fb2;

// the z-buffer in 16 bits precision
uint16_t* zbuf;

// the image that encapsulate framebuffer fb
//Image<RGB565> imfb(fb, SLX, SLY);
Image<RGB565>* imfbp;


// only load the shaders we need.
const int LOADED_SHADERS = TGX_SHADER_PERSPECTIVE | TGX_SHADER_ZBUFFER | TGX_SHADER_FLAT | TGX_SHADER_GOURAUD | TGX_SHADER_NOTEXTURE | TGX_SHADER_TEXTURE_NEAREST |TGX_SHADER_TEXTURE_WRAP_POW2;

// the renderer object that performs the 3D drawings
Renderer3D<RGB565, LOADED_SHADERS, uint16_t> renderer;


void logMemory() {
  Serial.printf("PSRAM Size: %d\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());
  Serial.printf("Used PSRAM: %d\n", ESP.getPsramSize() - ESP.getFreePsram());
}


// the setup function runs once when you press reset or power the board
void setup()
{
    Serial.begin(115200);
    Serial.println("Set up");

    logMemory();

    // allocate the second framebuffer
    fb = (uint16_t*)ps_malloc(SLX * SLY * sizeof(uint16_t));
    while (fb == nullptr)
    {
        Serial.println("Error: cannot allocate memory for fb");
        delay(1000);
    }
    logMemory();


    imfbp = new Image<RGB565>(fb, SLX, SLY);

    
    // allocate the second framebuffer
    fb2 = (uint16_t*)ps_malloc(SLX * SLY * sizeof(uint16_t));
    while (fb2 == nullptr)
    {
        Serial.println("Error: cannot allocate memory for fb2");
        delay(1000);
    }
    logMemory();
    // allocate the zbuffer
    zbuf = (uint16_t*)ps_malloc(SLX * SLY * sizeof(uint16_t));
    while (zbuf == nullptr)
    {
        Serial.println("Error: cannot allocate memory for zbuf");
        delay(1000);
    }
    logMemory();
    // direct control CS
//    pinMode(12, OUTPUT);
//    digitalWrite(12, LOW);
    
    //pinMode(12, OUTPUT);
    //digitalWrite(12, LOW);    
    pinMode(14, OUTPUT);
    digitalWrite(14, LOW);    
    
    // initialize the screen driver
    tft.init();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED);

    tft.initDMA();
    tft.startWrite();

    // setup the 3D renderer.
    renderer.setViewportSize(SLX,SLY);
    renderer.setOffset(0, 0);    
    //renderer.setImage(&imfb); // set the image to draw onto (ie the screen framebuffer)
    renderer.setImage(imfbp); // set the image to draw onto (ie the screen framebuffer)
    renderer.setZbuffer(zbuf); // set the z buffer for depth testing
    renderer.setPerspective(45, ((float)SLX) / SLY, 1.0f, 100.0f);  // set the perspective projection matrix.     

    renderer.setLookAt(0.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f);
    
    renderer.setMaterial(RGBf(0.85f, 0.55f, 0.25f), 0.2f, 0.7f, 0.8f, 64); // bronze color with a lot of specular reflexion. 
    renderer.setCulling(1);
    renderer.setTextureQuality(TGX_SHADER_TEXTURE_NEAREST);
    renderer.setTextureWrappingMode(TGX_SHADER_TEXTURE_WRAP_POW2);        

    pinMode(CS_1, OUTPUT);
    digitalWrite(CS_1, HIGH);    
    pinMode(CS_2, OUTPUT);
    digitalWrite(CS_2, HIGH);    

    Serial.println("Setted up");
}





/** Compute the model matrix according to the current time */
tgx::fMat4 moveModel(int& loopnumber)
{
    const float end1 = 6000;
    const float end2 = 2000;
    const float end3 = 6000;
    const float end4 = 2000;

    int tot = (int)(end1 + end2 + end3 + end4);
    int m = millis();

    loopnumber = m / tot;
    float t = m % tot;

    const float dilat = 9; // scale model
    const float roty = 360 * (t / 4000); // rotate 1 turn every 4 seconds        
    float tz, ty;
    if (t < end1)
    { // far away
        tz = -25;
        ty = 0;
    }
    else
    {
        t -= end1;
        if (t < end2)
        { // zooming in
            t /= end2;
            tz = -25 + 18 * t;
            ty = -6.5f * t;
        }
        else
        {
            t -= end2;
            if (t < end3)
            { // close up
                tz = -7;
                ty = -6.5f;
            }
            else
            { // zooming out
                t -= end3;
                t /= end4;
                tz = -7 - 18 * t;
                ty = -6.5 + 6.5 * t;
            }
        }
    }

    fMat4 M;
    M.setScale({ dilat, dilat, dilat }); // scale the model
    M.multRotate(-roty, { 0,1,0 }); // rotate around y
    M.multTranslate({ 0,ty, tz }); // translate
    return M;
}





int loopnumber = 0;
int prev_loopnumber = -1;
int cs_on = CS_1;
int cs_off = CS_2;

/** Main loop */
void loop()
{
    //Serial.println("Loop");
    uint32_t t = millis();
    //cs_on  = cs_on  - cs_off;
    //cs_off = cs_off + cs_on;
    //cs_on = cs_off - cs_on;


    //if (cs_on == 12){
    //  renderer.setLookAt(-0.1f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f);
    //}else{
    //  renderer.setLookAt(0.1f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f);
    //}


    digitalWrite(cs_on, LOW);    
    digitalWrite(cs_off, HIGH);    
    // compute the model position
    fMat4  M = moveModel(loopnumber);
    renderer.setModelMatrix(M);

    // draw the 3D mesh
    //imfb.fillScreen(RGB565_Cyan);              // clear the framebuffer (black background)
    imfbp->fillScreen(RGB565_Cyan);              // clear the framebuffer (black background)
    renderer.clearZbuffer();                    // clear the z-buffer

    // choose the shader to use
    switch (loopnumber % 4)
        {
        case 0: renderer.setShaders(TGX_SHADER_GOURAUD | TGX_SHADER_TEXTURE);
                renderer.drawMesh(&naruto_1, false); 
                break;
                
        case 1: renderer.drawWireFrameMesh(&naruto_1, true);
                break;
        
        case 2: renderer.setShaders(TGX_SHADER_FLAT);
                renderer.drawMesh(&naruto_1, false); 
                break;
                
        case 3: renderer.setShaders(TGX_SHADER_GOURAUD);
                renderer.drawMesh(&naruto_1, false); 
                break;        
        }

    if (prev_loopnumber != loopnumber)
        {
        prev_loopnumber = loopnumber;
        tft.fillRect(0, 300, 240, 20, TFT_BLACK);
        tft.setCursor(5, 305);
        switch (loopnumber % 4)
            {
            case 0: tft.print("Gouraud shading / texturing"); break;
            case 1: tft.print("Wireframe"); break;
            case 2: tft.print("Flat Shading"); break;
            case 3: tft.print("Gouraud shading"); break;
            }
        }

    // upload the framebuffer to the screen (async. via DMA)
    tft.dmaWait();
    if (cs_on == 14){
    //   tft.pushImageDMA(30, (tft.height() - SLY) / 2, SLX, SLY, fb, fb2);
    }else{
    //   tft.pushImageDMA(170, (tft.height() - SLY) / 2, SLX, SLY, fb, fb2);
    }
    //tft.pushImageDMA((tft.width() - SLX) / 2, (tft.height() - SLY) / 2, SLX, SLY, fb, fb2);

    int offset = (SLX * SLY /2);
    tft.pushImageDMA(0,       0, SLX, SLY/2,        fb,        fb2);
    //tft.dmaWait();  //added this to wait for DMA to finish before 
    tft.pushImageDMA(0, SLY / 2, SLX, SLY/2, fb+offset, fb2+offset);
    
    
    //tft.dmaWait();  //added this to wait for DMA to finish before 
    //digitalWrite(cs_on, HIGH);    
    //digitalWrite(cs_off, HIGH);    
    t = millis() - t;
    Serial.print(t); Serial.println(" ms");

    //Serial.println("Looped");
}



/** end of file */
