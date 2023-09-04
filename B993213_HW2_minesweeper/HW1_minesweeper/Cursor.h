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

		// Ű���� �Է�
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
			isFlag = true;	// ��� ��ġ
		}
		else if (input.getKeyDown(0x20)) {	// SPACE
			isOpen = true;	// ĭ ����
		}

		setPos(pos, n_rows, n_cols);

		if (input.getMouseButtonDown(0)) {		// ���콺 ���� Ŭ��
			isOpen = true;	// ��� ��ġ
			setPos(ArraymousePos(), n_rows, n_cols);

		}
		if (input.getMouseButtonDown(1)) {		// ���콺 ������ Ŭ��
			isFlag = true;	// ĭ ����
			setPos(ArraymousePos(), n_rows, n_cols);

		}
	}

	// 2���� �迭�� ��ġ ���� 
	Position ArraymousePos() {
		auto tempPos = input.getMousePosition();
		Position mousePos = { tempPos.y, tempPos.x };

		return mousePos;
	}


public:
    bool isFlag;	// ��� ��ġ�� �������� Ȯ��
    bool isOpen;	// ĭ ���⸦ �������� Ȯ�� 
    
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

