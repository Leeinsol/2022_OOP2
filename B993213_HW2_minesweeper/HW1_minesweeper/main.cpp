#define _CRT_SECURE_NO_WARNINGS_
#include <iostream>
#include "GameObject.h"
#include "Utils.h"
#include "MineSweeper.h"
#include "Cursor.h"
#include "InputSystem.h"

using namespace std;

int main() {
	InputSystem input;
	Cursor cursor{ 0, 0 , input};
	MineSweeper minesweeper(10, 10);

	while (1)
	{
		minesweeper.clearView();

		input.readEveryFrame();

		minesweeper.draw(cursor.getPos(), cursor.isFlag, cursor.isOpen);

		cursor.update(minesweeper.getNrows(), minesweeper.getNcols());
		
		minesweeper.render(cursor.getPos(), cursor.getShape());
		
		minesweeper.checkGame();
	}
	return 0;
}