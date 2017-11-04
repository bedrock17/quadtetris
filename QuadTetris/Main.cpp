#include "Game.h"

void cursor(bool visible)    // Console.CursorVisible = false;
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}


int main(void) {
	
	cursor(false);
	system("mode con cols=65 lines=19");
	
	Game game;
	
	game.start();

	return 0; 
}