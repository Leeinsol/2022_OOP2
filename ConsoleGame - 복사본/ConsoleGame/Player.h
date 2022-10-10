#pragma once
#include "GameObject.h"


class Player :
    public GameObject
{
public:
	//Screen* screen_ = nullptr;
	//char& lines;

	bool isFlag = false;

    Player(int x, int y) :GameObject('>', x, y) {}

	/*void getScreen(char&& lines) {
		this->lines = lines;
	}*/
	
	void check(int x, int y) {
	
		//Screen.setFlag(x, y);
	}

	void processInput() {

		Borland::Initialize();
		char key;
		cin >> key;
		//cin.clear();
		Borland::GotoXY(0, 21);
		printf("key is %c %d\n", key, key);
		isFlag = false;
		Position pos = getPos();

		switch (key)
		{
		case 'w':
			pos.x--;
			break;
		case 'a':
			pos.y--;
			break;
		case 's':
			pos.x++;
			break;
		case 'd':
			pos.y++;
			break;
		case 'q': // ±ê¹ß
			//lines[pos.x][pos.y] = '1';
			//screen.setFlag(pos.x, pos.y);
			//if (isFlag)	isFlag = false;
			isFlag = true;
			break;
		}

		//Borland::GotoXY(pos.x, pos.y);
		setPos(pos);
		Borland::GotoXY(0, 7);
		printf("[%d][%d]", pos.x, pos.y);
	}
};

