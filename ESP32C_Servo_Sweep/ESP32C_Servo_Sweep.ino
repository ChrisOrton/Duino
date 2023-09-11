/*
  Controls servo position from 0-180 degrees and back
  https://wokwi.com/projects/350037178957431378
  by dlloydev, December 2022.


  Servo control that works on ESP32C
  https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
*/

#include <Servo.h>

Servo myservo = Servo();

const int servoPin = D0;

void setup() {
}

void loop() {
  for (int pos = 0; pos <= 180; pos++) {  // go from 0-180 degrees
    myservo.write(servoPin, pos);        // set the servo position (degrees)
    delay(15);
  }
  for (int pos = 180; pos >= 0; pos--) {  // go from 180-0 degrees
    myservo.write(servoPin, pos);        // set the servo position (degrees)
    delay(15);
  }
}
