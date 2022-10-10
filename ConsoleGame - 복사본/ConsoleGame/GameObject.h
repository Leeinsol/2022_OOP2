#pragma once
#include <iostream>
#include "Utils.h"

using namespace std;

class GameObject {
	Position pos;
	char* shape;
	Dimension dim;


public:
	GameObject(char shape, int x, int y) 
		:pos(x, y),dim(1,1), shape(nullptr)
	{
		this->shape = (char*)new char[dim.size() + 1];
		this->shape[0] = shape;
		this->shape[dim.size()] = '\0';
	}


	virtual ~GameObject() {
		delete[] shape;
	}

	Position getPos() const { return pos; }

	void setPos(const Position& pos) { this->pos.x = pos.x, this->pos.y = pos.y; }
	char getShape() const { return shape[0]; }
};