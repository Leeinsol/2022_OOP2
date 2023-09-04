#pragma once
#include <conio.h>
#include "Utils.h"
#include <fstream>

using namespace std;

enum class KeyState
{
	Idle,			 // 0
	Pressed,		 // 1
	Stay,			 // 2
	Released,		 // 3
	PressedReleased  // 4
};

typedef KeyState MouseState;

class InputSystem
{
	KeyState keys[256];
	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	INPUT_RECORD irInBuf[128];

	MouseState mouse[2];

	COORD mousePosition;

	VOID ErrorExit(LPCSTR lpszMessage)
	{
		fprintf(stderr, "%s\n", lpszMessage);

		// Restore input mode on exit.
		SetConsoleMode(hStdin, fdwSaveOldMode);
		ExitProcess(0);
	}

	VOID KeyEventProc(KEY_EVENT_RECORD ker)
	{
		switch (keys[ker.wVirtualKeyCode & 0x00ff])
		{
		case KeyState::Idle:
			if (ker.bKeyDown == TRUE)
				keys[ker.wVirtualKeyCode] = KeyState::Pressed;
			break;
		case KeyState::Pressed:
			if (ker.bKeyDown == FALSE)
				keys[ker.wVirtualKeyCode] = KeyState::PressedReleased;
			else 
				keys[ker.wVirtualKeyCode] = KeyState::Stay;
			break;
		case KeyState::Stay:
			if (ker.bKeyDown == FALSE)
				keys[ker.wVirtualKeyCode] = KeyState::Released;
			break;
		case KeyState::Released: 
		case KeyState::PressedReleased:
			keys[ker.wVirtualKeyCode] = KeyState::Idle;
			break;
		}
	}

	VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
	{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif

		switch (mer.dwEventFlags)
		{
		case 0:
			
			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				switch (mouse[0]) {
				case MouseState::Idle:
					mouse[0] = MouseState::Pressed;
					mousePosition = mer.dwMousePosition;
					break;
				case MouseState::Pressed:
					mouse[0] = MouseState::Stay;
					mousePosition = mer.dwMousePosition;
					break;
				}
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				switch (mouse[1]) {
				case MouseState::Idle:
					mouse[1] = MouseState::Pressed;
					mousePosition = mer.dwMousePosition;
					break;
				case MouseState::Pressed:
					mouse[1] = MouseState::Stay;
					mousePosition = mer.dwMousePosition;
					break;
				}
				mousePosition = mer.dwMousePosition;
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					switch (mouse[i]) {
					case MouseState::Stay:
						mouse[i] = MouseState::Released;
						mousePosition = mer.dwMousePosition;
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}

public:
	InputSystem() {
		DWORD fdwMode;

		for (int i = 0; i < 256; i++)
			keys[i] = KeyState::Idle;

		mouse[0] = MouseState::Idle;
		mouse[1] = MouseState::Idle;
		mousePosition = { 0, 0 };

		memset(irInBuf, 0, sizeof(INPUT_RECORD) * 128);

		// Get the standard input handle.

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
			ErrorExit("GetStdHandle");

		// Save the current input mode, to be restored on exit.

		if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
			ErrorExit("GetConsoleMode");

		// Enable the window and mouse input events.

		fdwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT) & ~ENABLE_QUICK_EDIT_MODE;
		if (!SetConsoleMode(hStdin, fdwMode))
			ErrorExit("SetConsoleMode");
	}


	virtual ~InputSystem() {
		SetConsoleMode(hStdin, fdwSaveOldMode);
	}

	void readEveryFrame() {
		DWORD cNumRead = 0;

		if (GetNumberOfConsoleInputEvents(hStdin, &cNumRead) == 0)
		{
			printf("Input Reading Failure %d\n", GetLastError() );
			return;
		}

		for (int i = 0; i < 256; i++) {
			switch (keys[i]) {
			case KeyState::Released: 
			case KeyState::PressedReleased:
				keys[i] = KeyState::Idle; 
				break;
			case KeyState::Pressed:
				keys[i] = KeyState::Stay; 
				break;
			}
		}

		for (int i = 0; i < 2; i++) {
			switch (mouse[i]) {
			case MouseState::Released:
				mouse[i] = MouseState::Idle;
				break;
			case MouseState::Pressed:
				mouse[i] = MouseState::Stay;
				break;
			}
		}
	
		if (!ReadConsoleInput(
			hStdin,      // input buffer handle
			irInBuf,     // buffer to read into
			128,         // size of read buffer
			&cNumRead)) // number of records read
			ErrorExit("ReadConsoleInput");
		
		for (int i = 0; i < (int)cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input
				KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: // mouse input
				MouseEventProc(irInBuf[i].Event.MouseEvent);
				break;
			}
		}
	}

	auto getKeyDown(WORD keyCode) const
	{
		return keys[keyCode & 0x00ff] == KeyState::Pressed 
			|| keys[keyCode & 0x00ff] == KeyState::PressedReleased;
	}

	auto getMouseButtonDown(int button) const
	{
		//printf("mouse state down [%x]\n", mouse[button]);
		return mouse[button] == MouseState::Pressed;
	}

	Position getMousePosition() const 
	{
		return { mousePosition.X, mousePosition.Y };
	}
};

