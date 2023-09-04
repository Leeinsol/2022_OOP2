#pragma once
#include <iostream>
#include "Utils.h"
#include "time.h"
#include "InputSystem.h"
#include <queue>

using namespace std;

// ��ũ���� ���¸� ����
enum ScreenState {
	Close = '@',
	Open = 'O',
	Flag = '*'
};

class MineSweeper
{
	char** state;		// ���¸� �����ϴ� �迭
	int** minesMap;		// ������ ��ġ�� �����ϴ� �迭
	char** view;		// ��ũ���� ���̴� �迭

	int n_rows;		// �� ��
	int n_cols;		// �� ��
	int n_mines;	// ������ ��

public:
	MineSweeper(int n_rows, int n_cols)
		:n_rows(n_rows), n_cols(n_cols) 
	{
		n_mines = 10;	// ������ �� �ʱ�ȭ
		
		// state 2���� �迭 �����Ҵ�
		state = new char* [n_rows];
		for (int i = 0; i < n_rows; i++) {
			state[i] = new char[n_cols];
		}

		// minesMap 2���� �迭 �����Ҵ�
		minesMap = new int* [n_rows];
		for (int i = 0; i < n_rows; i++) {
			minesMap[i] = new int[n_cols];
		}

		// view 2���� �迭 �����Ҵ�
		view = new char* [n_rows];
		for (int i = 0; i < n_rows; i++) {
			view[i] = new char[n_cols];
		}

		Borland::Initialize();
		clear();	// �ʱ�ȭ
		randomMines();	// ���� ��ġ�� ���� ��ġ
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
		// state �迭 �ʱ�ȭ
		for (int i = 0; i < n_rows; i++) {
			memset(state[i], Close, sizeof(char) * n_cols);
		}

		// minesMap �迭 �ʱ�ȭ
		for (int i = 0; i < n_rows; i++) {
			memset(minesMap[i], 0, sizeof(int) * n_cols);
		}

		printManual();
	}

	void clearView() {
		// view �迭 �ʱ�ȭ
		for (int i = 0; i < n_rows; i++) {
			memset(view[i], Close, sizeof(char) * n_cols);
		}
	}

	void printManual() {
		Borland::GotoXY(20, 1);
		printf("�� ���۹� ��");

		Borland::GotoXY(20, 3);
		printf("��� �����:	Q / ���콺 ������ Ŭ��");

		Borland::GotoXY(20, 4);
		printf("ĭ ����:		SPACE / ���콺 ���� Ŭ��");
	}
	
	void draw(const Position& pos, bool isFlag, bool isOpen)
	{
		// Flag ��ư�� �������� Ȯ��
		if (isFlag)	setFlag(pos, isFlag);

		// Open ��ư�� �������� Ȯ��
		if (isOpen)	setOpen(pos);
	}

	void render(const Position& pos, char shape)
	{
		Borland::GotoXY(0, 0);

		// ���¸� ��ũ���� ����ϴ� �迭�� ����
		for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {
				view[i][j] = state[i][j];
			}
		}

		// ȭ�� ���
		for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {

				// Ŀ���� ���
				if (i == pos.x && j == pos.y) {
					printf("%c", shape);
				}
				// Open�� �� �ֺ��� ���� ������ ���
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
		printf("���� ����� ��: %d \n", remainFlag());
	}

	int remainFlag() {
		int count = n_mines;

		// �迭 Ž��
		for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {
				// ����� �ִ� ĭ�� ���
				if (state[i][j] == Flag)
					count--;
			}
		}
		// ���� ����� ���� ��ȯ
		return count;
	}

	void setFlag(const Position& pos, bool isFlag) {
		// �����ִ� ĭ�� ��� return
		if (state[pos.x][pos.y] == Open)	return;
		// �����ִ� ĭ�� ��� ��� ��ġ
		if (state[pos.x][pos.y] == Close)	state[pos.x][pos.y] = Flag;
		// ����� �ִ� ĭ�� ��� ��� ���ֱ�
		else								state[pos.x][pos.y] = Close;
	}

	void randomMines() {
		srand((unsigned)time(NULL));

		int mines = n_mines;
		int col = 0;
		int row = 0;

		// ���� ��ġ�� ���� ��ġ
		do {
			// ���� ����
			col = rand() % n_cols;
			row = rand() % n_rows;

			// ���ڰ� ���� ĭ���� ���� �ֱ�
			if (!minesMap[col][row]) {
				minesMap[col][row] = true;
				mines--;
			}
		} while (mines > 0);
		return;
	}

	void checkGame() {
		int n_correct = 0;	// ã�� ������ ��

		// �迭 Ž��
		for (int i = 0; i < n_cols; i++) {
			for (int j = 0; j < n_rows; j++) {
				if (minesMap[i][j] && state[i][j] == Flag) {	// ������ ��ġ�� ����� ��ġ�� ���� ��
					n_correct++;
				}
			}
		}

		// ��� ���ڸ� ã���� ��
		if (n_mines == n_correct) {
			Borland::GotoXY(20, n_rows - 1);
			printf("GAME CLEAR! \n\n\n");
			exit(1);
		}
	}

	void setOpen(const Position& pos) {
		// ĭ ���� Ž������ ����
		if (pos.x < 0 || pos.x >= n_rows || pos.y < 0 || pos.y >= n_cols) {
			return;
		}

		// ť�� �̿��Ͽ� BFS�� ����
		queue<Position> queue;
		queue.push(pos);

		while (!queue.empty()) {
			Position currentPos = queue.front();
			queue.pop();

			int count = countAroundMines(currentPos.x, currentPos.y);    // �ֺ� ������ ��

			// �����ְų� ��� ĭ�� ��� continue
			if (state[currentPos.x][currentPos.y] == Open || state[currentPos.x][currentPos.y] == Flag) {
				continue;
			}

			// �����ִ� ĭ�� ��� ĭ�� ����
			state[currentPos.x][currentPos.y] = Open;

			// ���� ĭ�� ���
			if (minesMap[currentPos.x][currentPos.y]) {
				Borland::GotoXY(20, n_rows - 1);
				cout << "GAME OVER" << endl << endl << endl;
				exit(1);
			}

			// �ֺ� ������ ���� 0�� ��
			if (count == 0) {
				// �ֺ� 9ĭ Ž��
				for (int row = -1; row < 2; row++) {
					for (int col = -1; col < 2; col++) {
						Position newPos(currentPos.x + row, currentPos.y + col);

						// ĭ ���� Ž������ ����
						if (newPos.x < 0 || newPos.x >= n_rows || newPos.y < 0 || newPos.y >= n_cols) {
							continue;
						}

						// �����ְų� ��� ĭ�� ��� continue
						if (state[newPos.x][newPos.y] == Open || state[newPos.x][newPos.y] == Flag) {
							continue;
						}

						queue.push(newPos);
					}
				}
			}
		}
	}


	// �ֺ��� ���� ��
	int countAroundMines(int x, int y) {
		int count = 0;
		int posX, posY = 0;

		// �ֺ� 9ĭ Ž��
		for (int row = -1; row < 2; row++) {
			for (int col = -1; col < 2; col++) {
				posX = x - row;
				posY = y - col;

				// ĭ ���� üũ���� ����
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

