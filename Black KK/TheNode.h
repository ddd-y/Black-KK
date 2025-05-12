#pragma once
#include<map>
#include<set>
#include<vector>
#include<cmath>
#include <queue>
#include<unordered_map>
#include<memory>
struct PreJumpNode
{
	int Distances[8];//向右为0，然后顺时针
	bool IsJump[8];//是因为跳点停止搜索还是不是因为跳点
	PreJumpNode() 
	{ 
		std::fill(std::begin(Distances), std::end(Distances), 0);
		std::fill(std::begin(IsJump), std::end(IsJump), false);
	}
};
struct TheNode//用于寻路用
{
	double TheG;//实际代价
	double Theh;//预估代价
	double Thef;//f=h+g
	std::shared_ptr<TheNode> TheLast;
	int x, y;
	TheNode() : TheG(10000), Theh(10000), Thef(20000), TheLast(nullptr), x(0), y(0) {
	}
	TheNode(int x, int y, std::shared_ptr<TheNode> Target, std::shared_ptr<TheNode> Last) : TheG(10000), Theh(10000),  x(x), y(y) {
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
		Thef = TheG + Theh;
	}
	void PreprocessJPSPlus(const std::vector<std::vector<int>>& TheGrid);//预处理
	static std::unordered_map<int, std::unordered_map<int, PreJumpNode>> preprocessData;
private:
	double heuristic(const TheNode& a, const TheNode& b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
	}
	double Distance(const std::shared_ptr<TheNode>& a, const std::shared_ptr<TheNode>& b) {
		int xDiff = std::abs(a->x - b->x);
		int yDiff = std::abs(a->y - b->y);
		return xDiff + yDiff;
	}
	bool IfHasForcedNeighbor(int x, int y, int dx, int dy,const std::vector<std::vector<int>>& TheGrid);
	bool IfWayXForced(int x, int y,int dx,const std::vector<std::vector<int>>& TheGrid);
	bool IfWayYForced(int x, int y, int dy, const std::vector<std::vector<int>>& TheGrid);
	bool IfDiaForced(int x, int y, int dx, int dy, const std::vector<std::vector<int>>& TheGrid);
};
std::shared_ptr<std::vector<std::shared_ptr<TheNode>>> JPSRoad(std::shared_ptr<TheNode> current, std::shared_ptr<TheNode> Goal, const std::vector<std::vector<int>>& TheGrid);
struct CompareTheNode {
	bool operator()(const std::shared_ptr<TheNode>& lhs, const std::shared_ptr<TheNode>& rhs) const {
		return lhs->Thef > rhs->Thef; // 改为比较 Thef 的小于关系
	}
};
struct CompareSharedPtr {
	bool operator()(const std::shared_ptr<TheNode>& lhs, const std::shared_ptr<TheNode>& rhs) const {
		return lhs->x < rhs->x || (lhs->x == rhs->x && lhs->y < rhs->y);
	}
};
