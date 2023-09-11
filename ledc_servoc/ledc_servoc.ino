
#define SERVO_PIN D1

void setup()
{
   Serial.begin(115200);
    Serial.println("Starting BLE work!");
   pinMode(SERVO_PIN,OUTPUT);
   ledcSetup(0,50,16);
   ledcAttachPin(SERVO_PIN,0);
}

void loop()
{
   for(int x=0;x<180;x++)
   {
      float fx = (float )x;
      SetServoPos(fx);
      delay(10);
   }
   delay(100);
}

void SetServoPos(float pos)
{
    uint32_t duty = (((pos/180.0)*2000)/20000.0*65536.0) + 1634;
         // convert 0-180 degrees to 0-65536

         duty = ((pos/180.0)*65536.0);

      Serial.println(duty);
      ledcWrite(0,duty);
        // set channel to pos
}
