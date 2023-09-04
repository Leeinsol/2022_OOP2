#pragma once
#include <iostream>
#include "Utils.h"
#include "time.h"
#include "InputSystem.h"
#include <queue>

using namespace std;

// 스크린의 상태를 저장
enum ScreenState {
	Close = '@',
	Open = 'O',
	Flag = '*'
};

class MineSweeper
{
	char** state;		// 상태를 저장하는 배열
	int** minesMap;		// 지뢰의 위치를 저장하는 배열
	char** view;		// 스크린에 보이는 배열

	int n_rows;		// 행 수
	int n_cols;		// 열 수
	int n_mines;	// 지뢰의 수

public:
	MineSweeper(int n_rows, int n_cols)
		:n_rows(n_rows), n_cols(n_cols) 
	{
		n_mines = 10;	// 지뢰의 수 초기화
		
		// state 2차원 배열 동적할당
		state = new char* [n_rows];
		for (int i = 0; i < n_rows; i++) {
			state[i] = new char[n_cols];
		}

		// minesMap 2차원 배열 동적할당
		minesMap = new int* [n_rows];
		for (int i = 0; i < n_rows; i++) {
			minesMap[i] = new int[n_cols];
		}

		// view 2차원 배열 동적할당
		view = new char* [n_rows];
		for (int i = 0; i < n_rows; i++) {
			view[i] = new char[n_cols];
		}

		Borland::Initialize();
		clear();	// 초기화
		randomMines();	// 랜덤 위치에 지뢰 설치
	}

	virtual ~MineSweeper() {
		delete[] state;
		delete[] minesMap;
		delete[] view;
	}

	int getNrows() {
		return n_rows;
	}
	
	int getNcols() {
		return n_cols;
	}

	void clear() {
		// state 배열 초기화
		for (int i = 0; i < n_rows; i++) {
			memset(state[i], Close, sizeof(char) * n_cols);
		}

		// minesMap 배열 초기화
		for (int i = 0; i < n_rows; i++) {
			memset(minesMap[i], 0, sizeof(int) * n_cols);
		}

		printManual();
	}

	void clearView() {
		// view 배열 초기화
		for (int i = 0; i < n_rows; i++) {
			memset(view[i], Close, sizeof(char) * n_cols);
		}
	}

	void printManual() {
		Borland::GotoXY(20, 1);
		printf("★ 조작법 ★");

		Borland::GotoXY(20, 3);
		printf("깃발 세우기:	Q / 마우스 오른쪽 클릭");

		Borland::GotoXY(20, 4);
		printf("칸 열기:		SPACE / 마우스 왼쪽 클릭");
	}
	
	void draw(const Position& pos, bool isFlag, bool isOpen)
	{
		// Flag 버튼을 눌렀는지 확인
		if (isFlag)	setFlag(pos, isFlag);

		// Open 버튼을 눌렀는지 확인
		if (isOpen)	setOpen(pos);
	}

	void render(const Position& pos, char shape)
	{
		Borland::GotoXY(0, 0);

		// 상태를 스크린에 출력하는 배열에 저장
		for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {
				view[i][j] = state[i][j];
			}
		}

		// 화면 출력
		for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {

				// 커서를 출력
				if (i == pos.x && j == pos.y) {
					printf("%c", shape);
				}
				// Open일 때 주변의 지뢰 개수를 출력
				else if (view[i][j] == Open) {
					printf("%d", countAroundMines(i, j));
				}
				else {
					printf("%c", view[i][j]);
				}
			}
			printf("\n");
		}

		Borland::GotoXY(20, n_rows - 3);
		printf("남은 깃발의 수: %d \n", remainFlag());
	}

	int remainFlag() {
		int count = n_mines;

		// 배열 탐색
		for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {
				// 깃발이 있는 칸이 경우
				if (state[i][j] == Flag)
					count--;
			}
		}
		// 남은 깃발의 수를 반환
		return count;
	}

	void setFlag(const Position& pos, bool isFlag) {
		// 열려있는 칸일 경우 return
		if (state[pos.x][pos.y] == Open)	return;
		// 닫혀있는 칸일 경우 깃발 설치
		if (state[pos.x][pos.y] == Close)	state[pos.x][pos.y] = Flag;
		// 깃발이 있는 칸일 경우 깃발 없애기
		else								state[pos.x][pos.y] = Close;
	}

	void randomMines() {
		srand((unsigned)time(NULL));

		int mines = n_mines;
		int col = 0;
		int row = 0;

		// 랜덤 위치에 지뢰 설치
		do {
			// 난수 생성
			col = rand() % n_cols;
			row = rand() % n_rows;

			// 지뢰가 없는 칸에만 지뢰 넣기
			if (!minesMap[col][row]) {
				minesMap[col][row] = true;
				mines--;
			}
		} while (mines > 0);
		return;
	}

	void checkGame() {
		int n_correct = 0;	// 찾은 지뢰의 수

		// 배열 탐색
		for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {
				if (minesMap[i][j] && state[i][j] == Flag) {	// 지뢰의 위치와 깃발의 위치가 같을 때
					n_correct++;
				}
			}
		}

		// 모든 지뢰를 찾았을 때
		if (n_mines == n_correct) {
			Borland::GotoXY(20, n_rows - 1);
			printf("GAME CLEAR! \n\n\n");
			exit(1);
		}
	}

	void setOpen(const Position& pos) {
		// 칸 밖은 탐색하지 않음
		if (pos.x < 0 || pos.x >= n_rows || pos.y < 0 || pos.y >= n_cols) {
			return;
		}

		// 큐를 이용하여 BFS를 구현
		queue<Position> queue;
		queue.push(pos);

		while (!queue.empty()) {
			Position currentPos = queue.front();
			queue.pop();

			int count = countAroundMines(currentPos.x, currentPos.y);    // 주변 지뢰의 수

			// 열려있거나 깃발 칸일 경우 continue
			if (state[currentPos.x][currentPos.y] == Open || state[currentPos.x][currentPos.y] == Flag) {
				continue;
			}

			// 닫혀있는 칸일 경우 칸을 오픈
			state[currentPos.x][currentPos.y] = Open;

			// 지뢰 칸일 경우
			if (minesMap[currentPos.x][currentPos.y]) {
				Borland::GotoXY(20, n_rows - 1);
				cout << "GAME OVER" << endl << endl << endl;
				exit(1);
			}

			// 주변 지뢰의 수가 0일 때
			if (count == 0) {
				// 주변 9칸 탐색
				for (int row = -1; row < 2; row++) {
					for (int col = -1; col < 2; col++) {
						Position newPos(currentPos.x + row, currentPos.y + col);

						// 칸 밖은 탐색하지 않음
						if (newPos.x < 0 || newPos.x >= n_rows || newPos.y < 0 || newPos.y >= n_cols) {
							continue;
						}

						// 열려있거나 깃발 칸일 경우 continue
						if (state[newPos.x][newPos.y] == Open || state[newPos.x][newPos.y] == Flag) {
							continue;
						}

						queue.push(newPos);
					}
				}
			}
		}
	}


	// 주변의 지뢰 수
	int countAroundMines(int x, int y) {
		int count = 0;
		int posX, posY = 0;

		// 주변 9칸 탐색
		for (int row = -1; row < 2; row++) {
			for (int col = -1; col < 2; col++) {
				posX = x - row;
				posY = y - col;

				// 칸 밖은 체크하지 않음
				if (posX < 0 || posX >= n_rows 
					|| posY < 0 ||  posY >= n_cols) {
					continue;
				}
				if (minesMap[posX][posY]) {
					count++;
				}
			}
		}
		return count;
	}
};

