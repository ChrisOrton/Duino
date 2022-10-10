/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  Improved Keyboard example

  Shows how to use the new Keyboard API.

  See HID Project documentation for more information.
  https://github.com/NicoHood/HID/wiki/Keyboard-API#improved-keyboard
*/

#include "HID-Project.h"

const int pinLed = LED_BUILTIN;
const int pinButton = 2;

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);

  // Sends a clean report to the host. This is important on any Arduino type.
  Keyboard.begin();
}



const String exitScript[]  = { //Type your commands here, one command per line.
                                    
//Example script to create a hidden backdoor in a Windows PC and enable Administrative shares
"exit"

                                   }; 

//Don't forget to update this line
const int numeLines = 1;


const String myScript[]  = { //Type your commands here, one command per line.
                                    
//Example script to create a hidden backdoor in a Windows PC and enable Administrative shares
"echo hello",
"echo world"
                                   }; 

//Don't forget to update this line
const int numLines = 2;

//Append all commands into one line of code each separated with an "&"
String constructCommandString(const String * str_arr, int sz){
  String finalString = "";

  for (int i = 0 ; i < sz - 1; i++){
    finalString += (str_arr[i] + " ; ");
  }

  //return finalString + str_arr[sz - 1] + " ; exit";
  return finalString + str_arr[sz - 1] + " ";
}

void hitEnter(){                    // Send Enter
  Keyboard.releaseAll();
  Keyboard.press(0x0A);
  delay(20);
  Keyboard.release(0x0A);
}

void openAdminCMD(){ //Works for Windows 8, 8.1 and 10 only
  Keyboard.releaseAll();
  Keyboard.press(KEY_LEFT_GUI);     //
  delay(20);                        // 
  Keyboard.press('x');              // Sends a WinKey + X key combination
  delay(20);                        //
  Keyboard.releaseAll();            //
  delay(100);
  
  Keyboard.press('a');              //
  delay(20);                        // Shortcut letter for Administrator CMD
  Keyboard.release('a');            //
  
  delay(3000);                      // Wait until Administrator Prompt shows up

  Keyboard.press(KEY_LEFT_ARROW);   //
  delay(50);                        // Automatically selects Yes
  hitEnter();                       //
}

void sendCommand(String cmd){       // Send a command to an active console
  Keyboard.releaseAll();
  delay(100);
  Keyboard.print(cmd);
  delay(200);
  hitEnter();
}

bool runNow = true;

void loop() {
  // Trigger caps lock manually via button
  if ((!digitalRead(pinButton)) && runNow) {

  
    digitalWrite(pinLed, HIGH);


    openAdminCMD();

    delay(2000);                       // Delays are necessary to avoid typing before the Window appears

    sendCommand(constructCommandString(myScript, numLines));

    delay(2000);                       // Delays are necessary to avoid typing before the Window appears

    sendCommand(constructCommandString(exitScript, numeLines));

    // Use the default print functions
    //Keyboard.println("echo Hello World!");

    // Press a single character, special non ascii characters wont work.
    //Keyboard.write('a');

    // Write single keys, do not use a number here!
    //Keyboard.write(KEY_ENTER);


    // If you really wish to press a RAW keycode without the name use this:
    //Keyboard.write(KeyboardKeycode(40));

    // Use (a limited number of) consumer keys.
    // Only works with the lower 255 keys and on linux only.
    //Keyboard.write(MEDIA_PLAY_PAUSE);

    // Linux also supports several system function via consumer report.
    //Keyboard.write(CONSUMER_POWER);
    //Keyboard.write(CONSUMER_SLEEP);

    // You can also use some special keyboard keys on linux.
    //Keyboard.write(KEY_POWER);
    //Keyboard.write(KEY_F13);

    // You can wakeup you PC from sleep.
    // This might be not supported on all hardware, but on all OS types.
    //Keyboard.wakeupHost();

    // Simple debounce
    delay(300);
    digitalWrite(pinLed, LOW);
    runNow = false;
  }
}
