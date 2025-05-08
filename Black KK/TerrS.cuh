#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<memory>
#include<vector>
class TerrS
{
private:
	std::shared_ptr<std::vector<std::vector<int>>> TheTerrS;
	std::vector<float> NoiseNum;
	float Freq;
	int Width;
	int Height;
	void initThep();
	void SpawnTerr(std::vector<float> vectorNoise);
	bool IfValid(int x,int y)
	{
		return x >= 0 && x < Width && y >= 0 && y < Height;
	}
public:
	float GetNoise(int x) { return NoiseNum[x]; }
	int GetWidth() { return Width; }
	int GetHeight() { return Height; }
	std::shared_ptr< std::vector<std::vector<int>>> GetGrid()
	{
		return TheTerrS;
	}
	bool IfCanMove(int x, int y)//为0就可以走，为1就不能走，可以走为true
	{
		return IfValid(x, y) && (*TheTerrS)[y][x] == 0;
	}
	TerrS(int wid,int hei);
	~TerrS();
};
