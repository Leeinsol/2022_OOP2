#define _CRT_SECURE_NO_WARNINGS_
#include <iostream>
#include "GameObject.h"
#include "Utils.h"
#include "Screen.h"
#include "Player.h"

using namespace std;

//Player player{ 3, 2 };
//Screen screen(5, 5);


//void processInput() {
//
//	Borland::Initialize();
//	char key;
//	cin >> key;
//
//	Borland::GotoXY(0, 21);
//	printf("key is %c %d\n", key, key);
//
//	Position pos = player.getPos();
//
//	switch (key)
//	{
//	case 'w':
//		pos.x--;
//		break;
//	case 'a':
//		pos.y--;
//		break;
//	case 's':
//		pos.x++;
//		break;
//	case 'd':
//		pos.y++;
//		break;
//	case 'q':
//		//lines[pos.x][pos.y] = '1';
//		screen.setFlag(pos.x,pos.y);
//
//		break;
//	}
//
//	player.setPos(pos);
//}

int main() {
	
	Player player{ 0, 0 };
	Screen screen(7, 7);

	//char&& lines_ = screen.getlines();
	//Screen* screen_pointer = &screen;

	//player.getScreen(lines_);

	while (1)
	{
		char oldState;
		
		screen.draw(player.getPos(), player.getShape(),player.isFlag);
		//screen.draw(enemy.getPos(), enemy.getShape());

		screen.render(player.getPos(),player.getShape());
		//processInput();
		player.processInput();
		
		//screen.clear();
		
	}
	return 0;
}