
#include "Tetronimo.h"
volatile long seconds = 0;

volatile bool BUTTON_STORE = false;
volatile bool BUTTON_LEFT = false;
volatile bool BUTTON_ROTATE = false;
volatile bool BUTTON_DOWN = false;
volatile bool BUTTON_RIGHT = false;
volatile bool BUTTON_RELOAD = false;

volatile bool reload = false;
volatile bool store = false;
volatile bool solve = false;


struct password { //The same idea as the Area from the Display.h, but this is to store the state of the current board.
  byte grid[8]{
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  };
};

ISR(TIMER2_OVF_vect)
{
  seconds++;
}


password tetrisArea[2][2];

Tetronimo currentBlock; //The block that is being played right now.


void Tetris_Initialize() { //Creating the tetronimo block.
	currentBlock = Tetronimo_Initial();
}



void Tetris_Draw() //Drawing the tetris block
{
	Tetronimo_Print(currentBlock, true);
	Display_Draw();
	Tetronimo_Print(currentBlock, false);
}

void Tetris_StoreOrSolve() //prints whether the user is solving or storing
{
  lcd.setCursor(10, 0);
  if (store)
	  lcd.print("Store");
  else if (solve)
    lcd.print("Solve");
}

/*void Tetris_DeleteRow(int row) //Deleting row if the row is full.
{
	bool sw = true;
	for (int i = 0; i < 15; i++)
	{
		Display_DrawLine(1, row, 8, true, sw);
		Display_DrawLine(0, 0, 10, true, !sw); // Linie Oben H
		Display_DrawLine(0, 15, 10, true, !sw); // Linie Unten H
		Display_DrawLine(0, 0, 16, false, !sw); // Linie Links V
		Display_DrawLine(9, 0, 16, false, !sw); // Linie Rechts V
		Display_Draw();
		delay(90);
		sw = sw ? false : true;
	}
	Display_DrawLine(1, row, 8, true, false);
	Display_DrawLine(0, 0, 10, true, true); // Linie Oben H
	Display_DrawLine(0, 15, 10, true, true); // Linie Unten H
	Display_DrawLine(0, 0, 16, false, true); // Linie Links V
	Display_DrawLine(9, 0, 16, false, true); // Linie Rechts V
	Display_Draw();
	for (int row1 = row; row1 > 2; row1--)
	{
		for (int x = 1; x < TETRIS_WIDTH + 1; x++)
		{
			Display_SetPixel(x, row1, Display_GetPixel(x, row1 - 1));
		}
	}
}*/

/*void Tetris_CheckRows() //Checking rows to see if they're full
{
	for (int row1 = TETRIS_HEIGHT; row1 > 0; row1--) //Going through the rows
	{
		bool lineFull = true;
		for (int x = 1; x <= TETRIS_WIDTH; x++) //going through all the pieces in the row
		{
			if (!Display_GetPixel(x, row1)) //if one of the pieces is empty,
			{
				lineFull = false; //we determine that the line is not full
				break;
			}
		}
		if (lineFull) // Is the line actually full?
		{
			Tetris_DeleteRow(row1); //if so delete the row
			row1++; //Since the row is deleted, the next row moves to the current row. So we need row1 to stay the same value.
		}
	}
}*/

void Tetris_Store(){ //Stores the state of the current board, given that the user has set the mode to store.
  for (int row = TETRIS_HEIGHT; row > 0; row--){
    for (int x = 1; x <= TETRIS_WIDTH; x++){
      if (Display_GetPixel(x, row)){
         bitSet(tetrisArea[x / 5][row / 8].grid[row % 8], 4 - x % 5);
      } else {
         bitClear(tetrisArea[x / 5][row / 8].grid[row % 8], 4 - x % 5);
      }
    }
  }
}

bool Tetris_CheckPW(){ //Checking the state of the current board, vs the stored board, and determine if the password is solved.
  for (int row = TETRIS_HEIGHT; row>0; row--){
    for (int x = 1; x<= TETRIS_WIDTH; x++){
        if (bitRead(tetrisArea[x/5][row/8].grid[row % 8], 4 - x % 5) != bitRead(area[x / 5][row / 8].grid[row % 8], 4 - x % 5))
          return false;
    }
  }
  return true;
}

void Tetris_Reload() //Reloading the tetris, remember the part where the board goes down and up
{
	for (int row = 1; row < TETRIS_HEIGHT + 1; row++) //This scrolls down
	{
		Display_DrawLine(1, row, 8, true, true);
		Display_Draw();
		delay(60);
	}
	delay(400);
	for (int row = TETRIS_HEIGHT; row >= 1; row--) //This scrolls up 
	{
		Display_DrawLine(1, row, 8, true, false);
		Display_Draw();
		delay(60);
	}
  typeNumber = 0;  //Reset the type of tetronimo back to t.
	currentBlock = Tetronimo_Initial(); //Create a tetronimo block.
	lcd.setCursor(10, 0);
	lcd.print("     ");
	lcd.setCursor(3, 1);
	lcd.print("             "); //Erase any info
}
void Tetris_ReloadState(){ //Calls the reload function
  reload = true;
  do {
     Tetris_Reload();
  } while (reload);        //if the user is not holding select or right, keep reloading.
}
void Tetris_Update(long framecount) { //Updating the tetris board.
	if (framecount % 40 == 0) //every 40 frames
	{
    Tetris_StoreOrSolve(); //Print whether the user is storing or solving.
		currentBlock.y++; //move the current block down.
		if (Tetronimo_HitTest(currentBlock)) { //Check if the block hit anything
			currentBlock.y--; //if it did move it back up.
			for (int i = 0; i < 5; i++) {  //making the block flash
				Tetronimo_Print(currentBlock, true); 
				Display_Draw();
				delay(60);
				Tetronimo_Print(currentBlock, false);
				Display_Draw();
				delay(60);
			}
			Tetronimo_Print(currentBlock, true); //print the block
      
      if (store){ //if the user wanted to store, store as soon as the block touches the ground
         Tetris_Store();
      }
      
      if (solve){ //if the user wanted to solve, check if the state matches the pw.
          if (Tetris_CheckPW()){
            Serial.println("AYYY YOU GOT IT");
            lcd.setCursor(3, 1);
            lcd.print("You got it   ");
            Tetris_ReloadState();
            typeNumber = 0;
          }
      }
      
			currentBlock = Tetronimo_Initial(); //Create a new block
			Display_Draw(); //draw the display again
			if (Tetronimo_HitTest(currentBlock) || seconds >= 120000)  //if the new block touches something as soon as it spawned, the game is over.
			{
				lcd.setCursor(3, 1);
				lcd.write("Try Again!");
				delay(1000);
			  Tetris_ReloadState();
        seconds = 0;
        TCNT2 = 0;
			}
			//Tetris_CheckRows(); //Check for any full rows.
			return;
		}
   
	}
  
	 //The button stuff is going to be replaced with interrupts.
	/*if (BUTTON_STORE){
      BUTTON_STORE = false;
      store = true;
      solve = false;
      reload = false;
    
	}*/
 
	if (BUTTON_RIGHT) { //if right move the button right
      BUTTON_RIGHT = false;
      if   (reload){
       
      }  else{
			currentBlock.x++;
			if (Tetronimo_HitTest(currentBlock))
				currentBlock.x--;
		}
	}
	
	if (BUTTON_ROTATE) {
	 BUTTON_ROTATE = false;
   Tetronimo_Rotate(&currentBlock);
			if (Tetronimo_HitTest(currentBlock)) 
				Tetronimo_BackRotate(&currentBlock);
	}
 
	if (BUTTON_DOWN) {
			BUTTON_DOWN = false;
			currentBlock.y++;
			if (Tetronimo_HitTest(currentBlock))
				currentBlock.y--;
		}
	
	if (BUTTON_LEFT) {
    BUTTON_LEFT = false;
			currentBlock.x--;
			if (Tetronimo_HitTest(currentBlock))
				currentBlock.x++;
		
	}

 if (BUTTON_RELOAD) {
    if (!solve) {
      BUTTON_RELOAD = false;
      Tetris_ReloadState();
      seconds = 0;
      TCNT2 = 0;
    }
  }
}

/*
void Tetris_MoveLeft(){
    currentBlock.x--;
      if (Tetronimo_HitTest(currentBlock))
        currentBlock.x++;
}
void Tetris_MoveRotate(){
     Tetronimo_Rotate(&currentBlock);
      if (Tetronimo_HitTest(currentBlock)) 
        Tetronimo_BackRotate(&currentBlock);
}
void Tetris_MoveDown(){
    currentBlock.y++;
      if (Tetronimo_HitTest(currentBlock))
        currentBlock.y--;
}
void Tetris_MoveRight(){
   currentBlock.x++;
   if (Tetronimo_HitTest(currentBlock))
        currentBlock.x--;
}
*/
