#pragma once
#include <iostream>
#include "Utils.h"
#include "InputSystem.h"

using namespace std;

class GameObject {
	Position pos;
	char* shape;
	Dimension dim;

protected:
	InputSystem& input;


public:
	GameObject(char shape, int x, int y, InputSystem& input)
		:pos(x, y),dim(1,1), shape(nullptr), input(input)
	{
		this->shape = (char*)new char[dim.size() + 1];
		this->shape[0] = shape;
		this->shape[dim.size()] = '\0';
	}


	virtual ~GameObject() {
		delete[] shape;
	}

	Position getPos() const { return pos; }

	void setPos(const Position& pos, int n_rows, int n_cols) {
		// 배열 밖으로 나가지 않도록 설정
		if (!(pos.x >= 0 && pos.x < n_cols
			&& pos.y >= 0 && pos.y < n_rows)) {
			return;
		}
		this->pos.x = pos.x, this->pos.y = pos.y;
	}
	char getShape() const { return shape[0]; }
};