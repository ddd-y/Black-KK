#pragma once
#include"TheNode.h"
class Player;
class Boss
{
private:
	int x, y;
	std::shared_ptr<Player> ThePlayer;
	std::shared_ptr<std::vector<std::vector<int>>> TheMap;
	void Move(int dx, int dy) 
	{
		x += dx;
		y += dy;
	}
	void FindRoad(std::shared_ptr<TheNode> start, std::shared_ptr<TheNode> goal, const std::vector<std::vector<int>>& TheGrid)
	{
		auto path = JPSRoad(start, goal, TheGrid);
	}
public:
	Boss();
	~Boss();
};

