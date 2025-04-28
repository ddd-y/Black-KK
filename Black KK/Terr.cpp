#include "Terr.h"
#include<Windows.h>
#include<iostream>
#include<ctime>
#include<chrono>
#include"TheNode.h"
#include"ScreenDraw.h"
void Terr::GenerateSquareObstacles()
{
	std::srand(static_cast<unsigned int>(time(0)));
	int ObstacleCount = 14; // 障碍物数量
	for (int i = 0; i < ObstacleCount; ++i) {
		int x = std::rand() % TheWidth;
		int y = std::rand() % TheHeight;
		int size = std::rand() % 8 + 1; // 障碍物大小
		for (int j = 0; j < size; ++j) {
			for (int k = 0; k < size; ++k) {
				if (x + j < TheWidth && y + k < TheHeight) {
					(*TheGrid)[y + k][x + j] = 1; // 设置为障碍物
				}
			}
		}
	}
}
Terr::Terr()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		TheWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		TheHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	}
	else {
		std::cerr << "Error: Unable to get console buffer size." << std::endl;
		TheWidth = 0;
		TheHeight = 0;
	}
	TheGrid = std::make_shared<std::vector<std::vector<int>>>(TheHeight, std::vector<int>(TheWidth, 0));
	GenerateSquareObstacles();
}
