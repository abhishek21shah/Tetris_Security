#include <LiquidCrystal.h>
#include <SPI.h>
#include "Tetris.h"

void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}


void setup() {
  // initialize Timer2
  cli();         // disable global interrupts
  TCCR2A = 0;    // set entire TCCR2A register to 0
  TCCR2B = 0;    // set entire TCCR2B register to 0 
               // (as we do not know the initial  values) 
  TCCR2B |= (1<<2); //set frequency to clk/64 250khz
  TCNT2 = 0;
  TIMSK2 |= (1<<0);
  sei();
  int i;
   for (i=A0; i<=A5; i++){
   pciSetup(i);
   digitalWrite(i,HIGH);  //pull up resistor
   }
      
  lcd.begin(16, 2);  //setting the lcd character at position 16,2. We print the words Choose at the cursor.
  lcd.print("   Choose");
  Serial.begin(9600);
  randomSeed(analogRead(0));
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Loading ...");
  Display_Initial(); //initializing display, these functions are in the respective header files
  Tetris_Initialize(); //initializing tetris
  Tetris_ReloadState(); //We start at the reload state where we can choose between solving and storing.
  seconds = 0;
  TCNT2 = 0;
}
byte xC = 0;
byte yC = 0;
long frameCount = 0; //counting frames 

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(seconds);
  frameCount++;   //frame 
  Tetris_Draw();    //Drawing the tetris board every frame,
  Tetris_Update(frameCount);    //and updating the data as necessary.
  Tetronimo_Print(currentBlock, true); 
  Display_Draw();
  lcd.setCursor(3, 1);
  if ((120 - seconds/1000) >= 0)
  lcd.print(120 - seconds/1000);
  else 
  lcd.print(0);
  delay(1);   // fps maxes out at 1 frame per millisecond, which is 1000 fps
}



ISR (PCINT1_vect) {
  if (digitalRead(A0) == HIGH){
  BUTTON_STORE = true;
  if (reload){
     store = true;
     solve = false;
     reload = false;
    } 
  }

  else if (digitalRead(A1) == HIGH) {
  BUTTON_LEFT = true;
  }

  else if (digitalRead(A2) == HIGH) {
  BUTTON_ROTATE = true;
  }
  else if (digitalRead(A3) == HIGH){
  BUTTON_DOWN = true;
  }
  else if (digitalRead(A4) == HIGH) { //if right move the button right
  BUTTON_RIGHT = true;
   if (reload){
     store = false;
     solve = true;
     reload = false;
    } 
  }
  else if (digitalRead(A5) == HIGH && !solve){
  BUTTON_RELOAD = true;  
     /* Serial.println("A5");
      reload = true;
      Tetris_ReloadState();*/
  }
}

