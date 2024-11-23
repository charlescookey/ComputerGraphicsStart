#pragma once
#include <Windows.h>

#include <string>


class Window
{
public:
	HWND hwnd;
	HINSTANCE hinstance;

	std::string name;
	void init(const std::string , int , int);

	int width;
	int height;
	DWORD style;

	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];

	void updateMouse(int x, int y) {
		mousex = x;
		mousey = y;
	}

	void processMessages();
};

