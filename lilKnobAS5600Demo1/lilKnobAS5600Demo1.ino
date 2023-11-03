#include <Wire.h>
#include <AS5600.h>

//AMS_5600 ams5600;
AS5600 ams5600;
#define DIR_PIN 23

int angle_in = 0;

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  Serial.begin(115200);
  Wire.begin();
}

int Angle() {
  //digitalWrite(DIR_PIN, HIGH);
  int in;
  in = map(ams5600.getRawAngle(),0,4095,0,360);
  return in;
}

void loop() {
  if (ams5600.detectMagnet() == 1 ) {
    if (angle_in != Angle()) {
      angle_in = Angle();
      Serial.println(angle_in);
    }
  }
}
