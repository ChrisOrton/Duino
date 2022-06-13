double delayTime = 2.2; //these value may change acc. to the length of the word in the POV display
double charBreak = 5.5; //these value may change acc. to the length of the word in the POV display
int LED1 = 8;
int LED2 = 7;
int LED3 = 6;
int LED4 = 5;
int LED5 = 4;

int LED6 = 13;
int LED7 = 12;
int LED8 = 11;
int LED9 = 10;
int LED10 = 9;


const byte interruptPin = 2;

void setup(){

pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
pinMode(LED4, OUTPUT);
pinMode(LED5, OUTPUT);
pinMode(LED6, OUTPUT);
pinMode(LED7, OUTPUT);
pinMode(LED8, OUTPUT);
pinMode(LED9, OUTPUT);
pinMode(LED10, OUTPUT);

pinMode(2,INPUT);
attachInterrupt(digitalPinToInterrupt(interruptPin), irSpotted, RISING);
}

volatile int characterI=0;
int ichar=0;
void irSpotted(){
  characterI = 0;
  
}

void displayLine(int line){
int myline;
myline = line;

if (myline>=512) {digitalWrite(LED6, HIGH); myline-=512;} else {digitalWrite(LED6, LOW);}
if (myline>=256)  {digitalWrite(LED7, HIGH); myline-=256;}  else {digitalWrite(LED7, LOW);}
if (myline>=128)  {digitalWrite(LED8, HIGH); myline-=128;}  else {digitalWrite(LED8, LOW);}
if (myline>=64)  {digitalWrite(LED9, HIGH); myline-=64;}  else {digitalWrite(LED9, LOW);}
if (myline>=32)  {digitalWrite(LED10, HIGH); myline-=32;}  else {digitalWrite(LED10, LOW);}


if (myline>=16) {digitalWrite(LED1, HIGH); myline-=16;} else {digitalWrite(LED1, LOW);}
if (myline>=8)  {digitalWrite(LED2, HIGH); myline-=8;}  else {digitalWrite(LED2, LOW);}
if (myline>=4)  {digitalWrite(LED3, HIGH); myline-=4;}  else {digitalWrite(LED3, LOW);}
if (myline>=2)  {digitalWrite(LED4, HIGH); myline-=2;}  else {digitalWrite(LED4, LOW);}
if (myline>=1)  {digitalWrite(LED5, HIGH); myline-=1;}  else {digitalWrite(LED5, LOW);}
}


void chase_loop()
{
  
      displayLine(characterI);

      characterI = characterI * 2;
      if (characterI <1)
        characterI = 1;
      if (characterI >512)
        characterI = 0;
      
      delay(100); // gap between font lines
}


// binaryCount_
void loop()
{
  
      displayLine(characterI);

      characterI = characterI + 1;
      if (characterI >512)
        characterI = 0;
      
      delay(100); // gap between font lines
  
  
}
