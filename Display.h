#include <LiquidCrystal.h>
#include <SPI.h>

#define DISPLAY_HEIGHT 16 //The display height. The display is 16 pixels high.
#define DISPLAY_WIDTH 10 //The display width we're using since we're only using 4 blocsk

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //the pins in the led keypad responsible for display
void Display_DrawLine(int x, int y, int length, bool hori, bool isSet); 

struct Area {  //The area that the game is played on. 5 x 8.
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

Area area[2][2]; //There's 4 blocks of area to make a 10 x 16 play zone.

void Display_Initial() {  
	lcd.createChar(0, area[0][0].grid);  //Creating the four zones.
	lcd.createChar(1, area[0][1].grid);
	lcd.createChar(2, area[1][0].grid);
	lcd.createChar(3, area[1][1].grid);
  Display_DrawLine(0,0,10,true,true);
  Display_DrawLine(0,15,10,true,true);
  Display_DrawLine(0,0,16,false,true); // Linie Links V
  Display_DrawLine(9,0,16,false,true); // Linie Rechts V
}
void Display_Draw() { //Drawing the display

	lcd.createChar(0, area[0][0].grid);
	lcd.createChar(1, area[0][1].grid);
	lcd.createChar(2, area[1][0].grid);
	lcd.createChar(3, area[1][1].grid);
	lcd.setCursor(0, 0);
	lcd.write(byte(0));

	lcd.setCursor(0, 1);
	lcd.write(byte(1));

	lcd.setCursor(1, 0);
	lcd.write(byte(2));

	lcd.setCursor(1, 1);
	lcd.write(byte(3));
}

void Display_SetPixel(int x, int y, bool isOn)  //Setting the blocks. If isOn is true, we set the bit at the place to 1. Else, we set the bit to 0(empty space) 
{
	if (isOn)
		bitSet(area[x / 5][y / 8].grid[y % 8], 4 - x % 5);
	else
		bitClear(area[x / 5][y / 8].grid[y % 8], 4 - x % 5);
}

bool Display_GetPixel(int x, int y) //Checking if the specific square is empty or full.
{
	return bitRead(area[x / 5][y / 8].grid[y % 8], 4 - x % 5) == 1 ? true : false;
}


void Display_DrawLine(int x, int y, int length, bool hori, bool isSet) 
{
	for (int offset = 0; offset < length; offset++)
	{
		if (hori)
			Display_SetPixel(x + offset, y, isSet);
		else
			Display_SetPixel(x, y + offset, isSet);
	}
}

