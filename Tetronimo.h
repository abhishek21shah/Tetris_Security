#pragma once

#include "Display.h"

#define TETRIS_WIDTH (DISPLAY_WIDTH - 2) //The play board has edges, so the play area is actually 8 x 14
#define TETRIS_HEIGHT (DISPLAY_HEIGHT - 2)


int typeNumber = 0; //The type of tetronimo is on a cycle. It starts at 0.

enum Tetronimo_Type { //Tetronimo types.
	TShaped = 0,
	LShaped = 1,	
	IShaped = 2,
	SShaped = 3,
	OShaped = 4,
	ZShaped = 5,
};

 struct Tetronimo {
	volatile byte field[4]; //byte array that will store the tetronimo units.
	volatile char x = TETRIS_WIDTH / 2 - 1; //When the block comes out, it will always come out in the middle
	volatile char y = 1;  //It should come out at the top too.
	volatile char rotation = 0; //Hasn't been rotated yet, so the value is 0. 
	Tetronimo_Type type; //the type of tetronimo.
};

Tetronimo Tetronimo_Initial(); // initializing the tetronimo

bool Tetronimo_HitTest(Tetronimo t) { //Collision detection.
	for (char x = 0; x < 5; x++) {
		for (char y = 0; y < 4; y++) {
			if (bitRead(t.field[y], x) == 1) {
				char absoluteX = t.x + x;
				char absoluteY = t.y + y;
				if (absoluteX < 1 || absoluteX > TETRIS_WIDTH || absoluteY < 1 || absoluteY > TETRIS_HEIGHT){ //Checking if out of bounds
					/*Serial.print("HIT OUTWINDOW xT=");
					Serial.print(absoluteX, DEC);
					Serial.print("yT=");
					Serial.println(absoluteY, DEC);*/
					return true;
				}
				if (Display_GetPixel(absoluteX, absoluteY)) { //Checking if place is already full
					/*Serial.print("HIT Pixel xT=");
					Serial.print(absoluteX, DEC);
					Serial.print(" yT=");
					Serial.println(absoluteY, DEC);*/
					return true;
				}
			}
		}
	}
	return false;
}

void Tetronimo_Print(Tetronimo t, bool print) { //printing the tetronimo
	for (char x = 0; x < 5; x++) {
		for (char y = 0; y < 4; y++) {
			if (bitRead(t.field[y], x) == 1) {
				char absoluteX = t.x + x;
				char absoluteY = t.y + y;
				if (absoluteX < 1 || absoluteX > TETRIS_WIDTH || absoluteY < 1 || absoluteY > TETRIS_HEIGHT) //Checking if out of bounds
					continue;
				Display_SetPixel(absoluteX, absoluteY, print);
			}

		}
	}
}

void Tetronimo_Rotation(Tetronimo *t, char rotation) { //handling rotation
	(*t).rotation = rotation;
	switch ((*t).type) { //uses nested switch statements to handle rotation.
		case TShaped: //In the case of Tshaped blocks,
			{
			switch (rotation) //Depending on what the value of rotation is
				{
					case 0: //There's 4 possible orientations for the block to take.
				{
					(*t).field[0] = B00000;
					(*t).field[1] = B00100;
					(*t).field[2] = B01110;
					(*t).field[3] = B00000;
				} break;

					case 1:
				{
					(*t).field[0] = B00000;
					(*t).field[1] = B00100;
					(*t).field[2] = B00110;
					(*t).field[3] = B00100;
				} break;

					case 2:
				{
					(*t).field[0] = B00000;
					(*t).field[1] = B00000;
					(*t).field[2] = B01110;
					(*t).field[3] = B00100;
				} break;

				case 3:
				{
					(*t).field[0] = B00000;
					(*t).field[1] = B00100;
					(*t).field[2] = B01100;
					(*t).field[3] = B00100;
				} break;
			}
		} break; case LShaped:
			{
				switch (rotation)
				{
				case 0:
				{
					(*t).field[0] = B00000;
					(*t).field[1] = B00010;
					(*t).field[2] = B01110;
					(*t).field[3] = B00000;
				} break;

				case 1:
				{
					(*t).field[0] = B00000;
					(*t).field[1] = B00100;
					(*t).field[2] = B00100;
					(*t).field[3] = B00110;
				} break;

				case 2:
				{
					(*t).field[0] = B00000;
					(*t).field[1] = B00000;
					(*t).field[2] = B01110;
					(*t).field[3] = B01000;
				} break;

				case 3:
				{
					(*t).field[0] = B00000;
					(*t).field[1] = B01100;
					(*t).field[2] = B00100;
					(*t).field[3] = B00100;
				} break;
				}
			} break;

		case IShaped:
		{
			switch (rotation)
			{
			case 0:
			case 2:
			{
				(*t).field[0] = B00100;
				(*t).field[1] = B00100;
				(*t).field[2] = B00100;
				(*t).field[3] = B00100;
			} break;

			case 1:
			case 3:
			{
				(*t).field[0] = B00000;
				(*t).field[1] = B00000;
				(*t).field[2] = B01111;
				(*t).field[3] = B00000;
			} break;
			}
		} break;

		case SShaped:
		{
			switch (rotation)
			{
			case 0:
			{
				(*t).field[0] = B00000;
				(*t).field[1] = B01100;
				(*t).field[2] = B00110;
				(*t).field[3] = B00000;
			} break;

			case 1:
			{
				(*t).field[0] = B00000;
				(*t).field[1] = B00100;
				(*t).field[2] = B01100;
				(*t).field[3] = B01000;
			} break;

			case 2:
			{
				(*t).field[0] = B00000;
				(*t).field[1] = B01100;
				(*t).field[2] = B00110;
				(*t).field[3] = B00000;
			} break;

			case 3:
			{
				(*t).field[0] = B00000;
				(*t).field[1] = B00010;
				(*t).field[2] = B00110;
				(*t).field[3] = B00100;
			} break;
			}
		} break;

		case OShaped:
		{
			switch (rotation)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			{
				(*t).field[0] = B00000;
				(*t).field[1] = B01100;
				(*t).field[2] = B01100;
				(*t).field[3] = B00000;
			} break;
			}
		} break;
		case ZShaped:
    {
      switch (rotation)
      {
      case 0:
      {
        (*t).field[0] = B00000;
        (*t).field[1] = B00110;
        (*t).field[2] = B01100;
        (*t).field[3] = B00000;
      } break;

      case 1:
      {
        (*t).field[0] = B00000;
        (*t).field[1] = B00100;
        (*t).field[2] = B00110;
        (*t).field[3] = B00010;
      } break;

      case 2:
      {
        (*t).field[0] = B00000;
        (*t).field[1] = B00110;
        (*t).field[2] = B01100;
        (*t).field[3] = B00000;
      } break;

      case 3:
      {
        (*t).field[0] = B00000;
        (*t).field[1] = B00100;
        (*t).field[2] = B00110;
        (*t).field[3] = B00010;
      } break;
      }
    } break;

		default: break;
	}
}
void Tetronimo_Rotate(Tetronimo *t) { //Calls the rotation function. The ampersand just makes sure that rotation stays less than 4.
	Tetronimo_Rotation(t, (((*t).rotation + 1) % 4));
}

void Tetronimo_BackRotate(Tetronimo *t) //handles rotation when against the wall.
{
	char rotation = (*t).rotation - 1;
	if (rotation == -1)
		rotation = 3;
	Tetronimo_Rotation(t, rotation);
}

 Tetronimo Tetronimo_Initial() //initializing a tetronimo.
{
  typeNumber = typeNumber%6;  //theres only 6 type of tetronimo blocks
	Tetronimo t; //Creating the tetronimo
	t.type = (Tetronimo_Type)(typeNumber++); //setting its type.
	Tetronimo_Rotation(&t, 0); //Set the rotation to 0.
	return t;
}

