#pragma once
#include<map>
#include<set>
#include<vector>
#include<cmath>
#include <queue>
#include<memory>
struct TheNode//用于寻路用
{
	double TheG;//实际代价
	double Theh;//预估代价
	double Thef;//f=h+g
	std::shared_ptr<TheNode> TheLast;
	int x, y;
	bool operator>(const TheNode& other) const
	{
		return Thef > other.Thef;
	}
	bool operator==(const TheNode& other) const {
		return x == other.x && y == other.y;
	}
	TheNode() : TheG(10000), Theh(10000), Thef(20000), TheLast(nullptr), x(0), y(0) {
	}
	TheNode(int x, int y, std::shared_ptr<TheNode> Target,std::shared_ptr<TheNode> Last) : TheG(10000), Theh(10000), Thef(0), x(x), y(y) {
		if (Last)
		{
			TheLast = Last;
			TheG = Last->TheG + Distance(Last, std::make_shared<TheNode>(x, y, nullptr, nullptr));
		}
		else
			TheLast = nullptr;
		if (Target) {
			Theh = heuristic(*this, *Target);
		}
		else {
			Theh = 0; 
		}
		Thef = TheG + Theh;
	}
private:
	double heuristic(const TheNode& a, const TheNode& b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
	}
	double Distance(const std::shared_ptr<TheNode>& a, const std::shared_ptr<TheNode>& b) {
		int xDiff = std::abs(a->x - b->x);
		int yDiff = std::abs(a->y - b->y);
		return xDiff+yDiff;
	}
};
std::vector<std::shared_ptr<TheNode>> JPSRoad(std::shared_ptr<TheNode> current, std::shared_ptr<TheNode> Goal, const std::vector<std::vector<int>>& TheGrid);

