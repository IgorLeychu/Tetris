// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

class BaseApp
{
private:
	HANDLE mConsole;
	HANDLE mConsoleIn;

	CHAR_INFO* mChiBuffer;
	COORD mDwBufferSize;
	COORD mDwBufferCoord;
	SMALL_RECT mLpWriteRegion;

	void Render();

public:
	//dimensions of the output area horizontally and vertically in characters
	const int X_SIZE;
	const int Y_SIZE;

	//arguments are the horizontal and vertical size of the output area in characters
	BaseApp(int xSize=100, int ySize=80);
	virtual ~BaseApp();

	//starts the game loop
	void Run();

	//it is possible to fill the x,y-character of the screen with a certain character, or to read it
	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);

	/* this function is called every game iteration, it can be overridden in the class inheritor.
	It receives deltaTime - the time difference between the previous iteration and this one, in seconds*/
	virtual void UpdateF (float deltaTime){}
	/*this function is called by pressing a key on the keyboard, it receives a key code - btnCode.
	If you use arrows or function keys, you will get a common number for them, e.g. 224., and
	next comes the code of the key itself (you can get it by calling getch() method).
	The KeyPressed method can also be overridden in the inheritor*/
	virtual void KeyPressed (int btnCode){}
};