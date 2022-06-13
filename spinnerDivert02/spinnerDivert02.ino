double delayTime = 2.2; //these value may change acc. to the length of the word in the POV display
double charBreak = 5.5; //these value may change acc. to the length of the word in the POV display
int LED1 = 8;
int LED2 = 7;
int LED3 = 6;
int LED4 = 5;
int LED5 = 4;


const byte interruptPin = 2;

void setup(){
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
pinMode(LED4, OUTPUT);
pinMode(LED5, OUTPUT);

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
if (myline>=16) {digitalWrite(LED1, HIGH); myline-=16;} else {digitalWrite(LED1, LOW);}
if (myline>=8)  {digitalWrite(LED2, HIGH); myline-=8;}  else {digitalWrite(LED2, LOW);}
if (myline>=4)  {digitalWrite(LED3, HIGH); myline-=4;}  else {digitalWrite(LED3, LOW);}
if (myline>=2)  {digitalWrite(LED4, HIGH); myline-=2;}  else {digitalWrite(LED4, LOW);}
if (myline>=1)  {digitalWrite(LED5, HIGH); myline-=1;}  else {digitalWrite(LED5, LOW);}
}


void loop()
{
  
      displayLine(characterI);

      int val = digitalRead(2);
      digitalWrite(LED1, val);

      characterI = characterI + 1;
      if (characterI >16)
        characterI = 0;
      
      delay(100); // gap between font lines
  
  
}
