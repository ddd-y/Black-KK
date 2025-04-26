#pragma once
#include<memory>
#include<vector>
class Terr
{
	std::shared_ptr<std::vector<std::vector<int>>> TheGrid;
	int TheHeight;
	int TheWidth;
	void GenerateSquareObstacles();
public:
	int GetWidth() { return TheWidth; }
	int GetHeight() { return TheHeight; }
	std::shared_ptr<std::vector<std::vector<int>>> GetGrid() { return TheGrid; }
	bool IfValid(int x,int y)
	{
		return x >= 0 && x < TheWidth && y >= 0 && y < TheHeight;
	}
	Terr();
	~Terr(){}
};

