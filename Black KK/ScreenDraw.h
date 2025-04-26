#pragma once
#include <iostream>
#include <windows.h>
#include<memory>
#include<vector>
class ScreenDraw
{
private:
    std::shared_ptr<char> screenBuffer;
	std::shared_ptr<std::vector<std::vector<int>>> TheGrid;
    int screenWidth;
    int screenHeight;
    HANDLE Hback;
    HANDLE Hfront;
    CONSOLE_CURSOR_INFO cci;
    DWORD bytes = 0;
    COORD coord;
public:
    std::shared_ptr<char> GetScreenBuffer() { return screenBuffer; }
    ScreenDraw(int width, int height, std::shared_ptr<std::vector<std::vector<int>>>Grid) : screenWidth(width), screenHeight(height),TheGrid(Grid)
    {
        Hfront = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
        );
        Hback = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
        );
        screenBuffer = std::shared_ptr<char>(new char[width * height], [](char* p) { delete[] p; });
        memset(screenBuffer.get(), ' ', width * height);
        SetConsoleActiveScreenBuffer(Hfront);
        cci.bVisible = 0;
        cci.dwSize = 1;
        SetConsoleCursorInfo(Hfront, &cci);
        SetConsoleCursorInfo(Hback, &cci);
        coord = { 0, 0 };
		for (int i = 0; i < screenHeight; i++)
		{
			for (int j = 0; j < screenWidth; j++)
			{
				if ((*TheGrid)[i][j] == 1)
					screenBuffer.get()[i * screenWidth + j] = '#';
				else
					screenBuffer.get()[i * screenWidth + j] = ' ';
			}
		}
    }

    ~ScreenDraw()
    {
        CloseHandle(Hfront);
        CloseHandle(Hback);
    }

    void Draw(int x, int y, char ch)
    {
        if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
			if ((*TheGrid)[y][x] == 0)
				screenBuffer.get()[y * screenWidth + x] = ch;
    }

    void Clear()
    {
        memset(screenBuffer.get(), ' ', screenWidth * screenHeight);
    }
    void Display()
    {
        WriteConsoleOutputCharacterA(Hback, screenBuffer.get(), screenWidth * screenHeight, coord, &bytes);
        SetConsoleActiveScreenBuffer(Hback);
        std::swap(Hfront, Hback);
    }
};

