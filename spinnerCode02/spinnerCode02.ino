double delayTime = 2.2; //these value may change acc. to the length of the word in the POV display
double charBreak = 5.5; //these value may change acc. to the length of the word in the POV display
int LED1 = 8;
int LED2 = 7;
int LED3 = 6;
int LED4 = 5;
int LED5 = 4;

void setup(){
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
pinMode(LED4, OUTPUT);
pinMode(LED5, OUTPUT);

pinMode(2,INPUT);
attachInterrupt(digitalPinToInterrupt(2),irSpotted,RISING);
}

int characterI=0;
int ichar=0;
void irSpotted(){
  characterI = 0;
  ichar = 0;
}

int a[] = {1, 6, 26, 6, 1};
int b[] = {31, 21, 21, 10, 0};
int c[] = {14, 17, 17, 10, 0};
int d[] = {31, 17, 17, 14, 0};
int e[] = {31, 21, 21, 17, 0};
int f[] = {31, 20, 20, 16, 0};
int g[] = {14, 17, 19, 10, 0};
int h[] = {31, 4, 4, 4, 31};
int i[] = {0, 17, 31, 17, 0};
int j[] = {0, 17, 30, 16, 0};
int k[] = {31, 4, 10, 17, 0};
int l[] = {31, 1, 1, 1, 0};
int m[] = {31, 12, 3, 12, 31};
int n[] = {31, 12, 3, 31, 0};
int o[] = {14, 17, 17, 14, 0};
int p[] = {31, 20, 20, 8, 0};
int q[] = {14, 17, 19, 14, 2};
int r[] = {31, 20, 22, 9, 0};
int s[] = {8, 21, 21, 2, 0};
int t[] = {16, 16, 31, 16, 16};
int u[] = {30, 1, 1, 30, 0};
int v[] = {24, 6, 1, 6, 24};
int w[] = {28, 3, 12, 3, 28};
int x[] = {17, 10, 4, 10, 17};
int y[] = {17, 10, 4, 8, 16};
int z[] = {19, 21, 21, 25, 0};

int X[] = {17, 10, 4, 10, 17};
int spc[] = {0, 0, 0, 0, 0};
int eos[] = {0, 1, 0, 0, 0};
int excl[] = {0, 29, 0, 0, 0};
int ques[] = {8, 19, 20, 8, 0};

void displayLine(int line){
int myline;
myline = line;
if (myline>=16) {digitalWrite(LED1, HIGH); myline-=16;} else {digitalWrite(LED1, LOW);}
if (myline>=8)  {digitalWrite(LED2, HIGH); myline-=8;}  else {digitalWrite(LED2, LOW);}
if (myline>=4)  {digitalWrite(LED3, HIGH); myline-=4;}  else {digitalWrite(LED3, LOW);}
if (myline>=2)  {digitalWrite(LED4, HIGH); myline-=2;}  else {digitalWrite(LED4, LOW);}
if (myline>=1)  {digitalWrite(LED5, HIGH); myline-=1;}  else {digitalWrite(LED5, LOW);}
}

void displayChar(char ch){
  for (ichar = 0; ichar <5; ichar++){
      if (ch == 'a') displayLine(a[ichar]);
      if (ch == 'b') displayLine(b[ichar]);
      if (ch == 'c') displayLine(c[ichar]);
      if (ch == 'd') displayLine(d[ichar]);
      if (ch == 'e') displayLine(e[ichar]);
      if (ch == 'f') displayLine(f[ichar]);
      if (ch == 'g') displayLine(g[ichar]);
      if (ch == 'h') displayLine(h[ichar]);
      if (ch == 'i') displayLine(i[ichar]);
      if (ch == 'j') displayLine(j[ichar]);
      if (ch == 'k') displayLine(k[ichar]);
      if (ch == 'l') displayLine(l[ichar]);
      if (ch == 'm') displayLine(m[ichar]);
      if (ch == 'n') displayLine(n[ichar]);
      if (ch == 'o') displayLine(o[ichar]);
      if (ch == 'p') displayLine(p[ichar]);
      if (ch == 'q') displayLine(q[ichar]);
      if (ch == 'r') displayLine(r[ichar]);
      if (ch == 's') displayLine(s[ichar]);
      if (ch == 't') displayLine(t[ichar]);
      if (ch == 'u') displayLine(u[ichar]);
      if (ch == 'v') displayLine(v[ichar]);
      if (ch == 'w') displayLine(w[ichar]);
      if (ch == 'x') displayLine(x[ichar]);
      if (ch == 'y') displayLine(y[ichar]);
      if (ch == 'z') displayLine(z[ichar]);
      if (ch == '!') displayLine(excl[ichar]);
      if (ch == '?') displayLine(ques[ichar]);
      if (ch == '.') displayLine(eos[ichar]);
      if (ch == ' ') displayLine(spc[ichar]);
      if (ch == 'X') displayLine(X[ichar]);

      delay(delayTime); // gap between font lines
  }
  
  
}

void displayString(char* s){
  for (characterI = 0; characterI<=strlen(s); characterI++){
    
      displayChar(s[characterI]);
      delay(charBreak); // gap between font lines
  }
}

void loop()
{
  displayString("party  XXX  ");
}
