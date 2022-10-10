#pragma once
#include <iostream>
#include "Utils.h"
#include "time.h"

using namespace std;

enum ScreenState {
	Close = 'C',
	Open = 'O',
	Flag = 'F'
};

class Screen
{
	char** lines;
	int** map;

	int n_rows;
	int n_cols;
	int n_mines;

public:
	Screen(int n_rows, int n_cols)
		: n_rows(n_rows), n_cols(n_cols)
	{
		n_mines = 10;
		
		// lines 2차원 배열 동적할당
		lines = new char* [n_rows];
		for (int i = 0; i < n_rows; i++) {
			lines[i] = new char[n_cols];
		}

		// map 2차원 배열 동적할당
		map = new int* [n_rows];
		for (int i = 0; i < n_rows; i++) {
			map[i] = new int[n_cols];
		}

		Borland::Initialize();
		randomMines();	// 랜덤 위치에 지뢰 설치
		//clear();
	}

	virtual ~Screen() { delete[] lines; }

	void clear()
	{
		
		//Borland::GotoXY(0, 0);
		
		/*for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {
				
				if (lines[i][j] == Close) {
					lines[i][j] = 'C';
				}
				else if (lines[i][j] == Flag) {
					lines[i][j] = 'F';
				}
				else if (lines[i][j] == Open) {
					lines[i][j] = 'O';
				}

				
			}
		}*/
		//lines[pos.x][pos.y] = state;
	}

	 const Position& draw(const Position& pos, char shape, bool isFlag)
	{
		//char state = lines[pos.x][pos.y];
		//clear();
		/*if (lines[pos.x][pos.y]) {
			lines[pos.x][pos.y] = shape;
		}*/
		//initializeArray();
		//lines[pos.x][pos.y] = shape;
		//if (isFlag)	setFlag(pos.x, pos.y);
		//if (isFlag)	setFlag(pos.x, pos.y,isFlag);
		if(isFlag)	setFlag(pos.x, pos.y, isFlag);
		Borland::GotoXY(0, 8);
		printf("lines[%d][[%d]: %c\n", pos.x, pos.y, lines[pos.x][pos.y]);
		printf("lines[%d][[%d]: %c\n", pos.x + 1, pos.y, lines[pos.x + 1][pos.y]);
		printf("lines[%d][[%d]: %c\n", pos.x,pos.y+1,lines[pos.x][pos.y+1]);

		return pos;
	}

	void render(const Position& pos,char shape)
	{

		Borland::GotoXY(0, 0);
		// 화면 출력
		for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {
				if (i == pos.x && j == pos.y) {
					printf("%c", shape);
				}
				else {
					printf("%c", lines[i][j]);
				}
			}
			printf("\n");
		}

		//Borland::GotoXY(0, 13);
		////printf("%s", lines);
		//for (int i = 0; i < n_cols; i++) {
		//	for (int j = 0; j < n_rows; j++) {
		//		printf("%d", map[i][j]);
		//	}
		//	printf("\n");
		//}
	}

	char getlines() {
		return lines[n_rows][n_cols];
	}

	void setFlag(int x, int y,bool isFlag){
		/*if(isFlag){
		}*/
		//if (lines[x][y] == Close)		lines[x][y] = Flag;
		//if(lines[x][y] == Flag)	lines[x][y] = Close;
		Borland::GotoXY(0, 11);
		printf("isFlag: %d\n", isFlag);
		printf("lines[x][y]: %d\n", isFlag);
		printf("lines[%d][[%d]: %c\n", x, y, lines[x][y]);


		if (lines[x][y] == Open)	return;
		if (lines[x][y] == Close)		lines[x][y] = Flag;
		else							lines[x][y] = Close;

		/*if (isFlag)	lines[x][y] = Flag;
		else if (!isFlag)	lines[x][y] = Close;*/
	}

	void initializeArray() {
		// lines 배열 초기화
		for (int i = 0; i < n_rows; i++) {
			memset(lines[i], Close, sizeof(char) * n_cols);
		}

		// map 배열 초기화
		for (int i = 0; i < n_rows; i++) {
			memset(map[i], 0, sizeof(int) * n_cols);
		}
	}

	void randomMines() {
		initializeArray();

		srand((unsigned)time(NULL));
		int mines = n_mines;
		int col = 0;
		int row = 0;

		// 랜덤 위치에 지뢰 설치
		do {
			col = rand() % n_cols;
			row = rand() % n_rows;

			if (!map[col][row]) {
				map[col][row] = 1;
				mines--;
			}
		} while (mines > 0);
		return;
	}
};

