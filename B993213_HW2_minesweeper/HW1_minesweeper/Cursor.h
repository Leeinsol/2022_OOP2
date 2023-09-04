#pragma once
#include "GameObject.h"
#include <conio.h>

class Cursor :
    public GameObject
{
	void processInput(int n_rows, int n_cols)
	{
		isFlag = false;
		isOpen = false;

		// 키보드 입력
		auto pos = getPos();
		if (input.getKeyDown(0x57)) {		// W
			pos.x--;
		}
		else if (input.getKeyDown(0x41)) {	// A
			pos.y--;
		}
		else if (input.getKeyDown(0x53)) {	// S
			pos.x++;
		}
		else if (input.getKeyDown(0x44)) {	// D
			pos.y++;
		}
		else if (input.getKeyDown(0x51)) {	// F
			isFlag = true;	// 깃발 설치
		}
		else if (input.getKeyDown(0x20)) {	// SPACE
			isOpen = true;	// 칸 열기
		}

		setPos(pos, n_rows, n_cols);

		if (input.getMouseButtonDown(0)) {		// 마우스 왼쪽 클릭
			isOpen = true;	// 깃발 설치
			setPos(ArraymousePos(), n_rows, n_cols);

		}
		if (input.getMouseButtonDown(1)) {		// 마우스 오른쪽 클릭
			isFlag = true;	// 칸 열기
			setPos(ArraymousePos(), n_rows, n_cols);

		}
	}

	// 2차원 배열로 위치 변경 
	Position ArraymousePos() {
		auto tempPos = input.getMousePosition();
		Position mousePos = { tempPos.y, tempPos.x };

		return mousePos;
	}


public:
    bool isFlag;	// 깃발 설치를 눌렀는지 확인
    bool isOpen;	// 칸 열기를 눌렀는지 확인 
    
    Cursor(int x, int y, InputSystem& input) 
		:GameObject('>', x, y, input)
	{
		isFlag = false;
		isOpen = false;
	}

	void update(int n_rows, int n_cols) {
		processInput(n_rows, n_cols);
	}
};

