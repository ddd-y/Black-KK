#include "TheNode.h"
#include <iostream>
struct CompareTheNode {
	bool operator()(const std::shared_ptr<TheNode>& lhs, const std::shared_ptr<TheNode>& rhs) const {
		return *lhs > *rhs;
	}
};
struct CompareSharedPtr {
	bool operator()(const std::shared_ptr<TheNode>& lhs, const std::shared_ptr<TheNode>& rhs) const {
		return lhs->x < rhs->x || (lhs->x == rhs->x && lhs->y < rhs->y);
	}
};
bool operator<(const std::shared_ptr<TheNode>& lhs, const std::shared_ptr<TheNode>& rhs) {
	return lhs->x < rhs->x || (lhs->x == rhs->x && lhs->y < rhs->y);
}
bool IsValid(int thex, int they, const std::vector<std::vector<int>>& TheGrid) {//判断坐标是否合法
	return (thex >= 0 && they >= 0 && thex < TheGrid[0].size() && they < TheGrid.size());
}
bool IsWalkable(int thex, int they, const std::vector<std::vector<int>>& TheGrid) {//判断坐标是否可走,0表示可走，1表示不可走
	return (TheGrid[they][thex] == 0);
}
bool HasForcedNeighbor(const std::shared_ptr<TheNode>& node, const std::vector<std::vector<int>>& TheGrid, int dx, int dy) {
	if (!node)
		return false;
	if (dy == 0 && dx != 0)
	{
		if (dx > 0)
		{
			if (IsValid(node->x, node->y - 1, TheGrid) && !IsWalkable(node->x, node->y - 1, TheGrid))
			{
				if (IsValid(node->x + 1, node->y - 1, TheGrid) && IsWalkable(node->x + 1, node->y - 1, TheGrid))
				{
					return true;
				}
			}
			if (IsValid(node->x, node->y + 1, TheGrid) && !IsWalkable(node->x, node->y + 1, TheGrid))
			{
				if (IsValid(node->x + 1, node->y + 1, TheGrid) && IsWalkable(node->x + 1, node->y + 1, TheGrid))
				{
					return true;
				}
			}
		}
		else
		{
			if (IsValid(node->x, node->y - 1, TheGrid) && !IsWalkable(node->x, node->y - 1, TheGrid))
			{
				if (IsValid(node->x - 1, node->y - 1, TheGrid) && IsWalkable(node->x - 1, node->y - 1, TheGrid))
				{
					return true;
				}
			}
			if (IsValid(node->x, node->y + 1, TheGrid) && !IsWalkable(node->x, node->y + 1, TheGrid))
			{
				if (IsValid(node->x - 1, node->y + 1, TheGrid) && IsWalkable(node->x - 1, node->y + 1, TheGrid))
				{
					return true;
				}
			}
		}
	}
	else if (dx == 0 && dy != 0)
	{
		if (dy > 0)
		{
			if (IsValid(node->x - 1, node->y, TheGrid) && !IsWalkable(node->x - 1, node->y, TheGrid))
			{
				if (IsValid(node->x - 1, node->y + 1, TheGrid) && IsWalkable(node->x - 1, node->y + 1, TheGrid))
				{
					return true;
				}
			}
			if (IsValid(node->x + 1, node->y, TheGrid) && !IsWalkable(node->x + 1, node->y, TheGrid))
			{
				if (IsValid(node->x + 1, node->y + 1, TheGrid) && IsWalkable(node->x + 1, node->y + 1, TheGrid))
				{
					return true;
				}
			}
		}
		else
		{
			if (IsValid(node->x - 1, node->y, TheGrid) && !IsWalkable(node->x - 1, node->y, TheGrid))
			{
				if (IsValid(node->x - 1, node->y - 1, TheGrid) && IsWalkable(node->x - 1, node->y - 1, TheGrid))
				{
					return true;
				}
			}
			if (IsValid(node->x + 1, node->y, TheGrid) && !IsWalkable(node->x + 1, node->y, TheGrid))
			{
				if (IsValid(node->x + 1, node->y - 1, TheGrid) && IsWalkable(node->x + 1, node->y - 1, TheGrid))
				{
					return true;
				}
			}
		}
	}
	else if (dx != 0 && dy != 0)
	{
		int thedx = dx > 0 ? 1 : -1;
		int thedy = dy > 0 ? 1 : -1;
		if (IsValid(node->x - thedx, node->y, TheGrid) && !IsWalkable(node->x - thedx, node->y, TheGrid))
		{
			if (IsValid(node->x - thedx, node->y + thedy, TheGrid) && IsWalkable(node->x - thedx, node->y + thedy, TheGrid))
			{
				return true;
			}
		}
		if (IsValid(node->x, node->y - thedy, TheGrid) && !IsWalkable(node->x, node->y - thedy, TheGrid))
		{
			if (IsValid(node->x + thedx, node->y - thedy, TheGrid) && IsWalkable(node->x + thedx, node->y - thedy, TheGrid))
			{
				return true;
			}
		}
	}
	return false;
}
bool IsTheGoal(std::shared_ptr<TheNode>& current, std::shared_ptr<TheNode>& Goal) {
	if (!current || !Goal)
		return false;
	return (current->x == Goal->x && current->y == Goal->y);
}
std::shared_ptr<TheNode> FindJumpRight(int dx, std::shared_ptr<TheNode> current, std::shared_ptr<TheNode> Goal, const std::vector<std::vector<int>>& TheGrid)
{
	int thedx = dx > 0 ? 1 : -1;
	current = std::make_shared<TheNode>(current->x + thedx, current->y, Goal, current);
	while (IsValid(current->x, current->y, TheGrid) && IsWalkable(current->x, current->y, TheGrid))
	{
		if (HasForcedNeighbor(current, TheGrid, thedx, 0) || IsTheGoal(current, Goal))
		{
			return current;
		}
		current = std::make_shared<TheNode>(current->x + thedx, current->y, Goal, current);
	}
	return nullptr;
}
std::shared_ptr<TheNode> FindJumpUp(int dy, std::shared_ptr<TheNode> current, std::shared_ptr<TheNode> Goal, const std::vector<std::vector<int>>& TheGrid)
{
	int thedy = dy > 0 ? 1 : -1;
	current = std::make_shared<TheNode>(current->x, current->y + thedy, Goal, current);
	while (IsValid(current->x, current->y, TheGrid) && IsWalkable(current->x, current->y, TheGrid))
	{
		if (HasForcedNeighbor(current, TheGrid, 0, thedy) || IsTheGoal(current, Goal))
		{
			return current;
		}
		current = std::make_shared<TheNode>(current->x, current->y + thedy, Goal, current);
	}
	return nullptr;
}
std::shared_ptr<TheNode> FindJumpDiagonal(int dx, int dy, std::shared_ptr<TheNode> current, std::shared_ptr<TheNode> Goal, const std::vector<std::vector<int>>& TheGrid, std::map<std::shared_ptr<TheNode>, double, CompareSharedPtr>& TheClosedSet, std::priority_queue<std::shared_ptr<TheNode>, std::vector<std::shared_ptr<TheNode>>, CompareTheNode>& TheOpenSet)
{
	int thedx = dx > 0 ? 1 : -1;
	int thedy = dy > 0 ? 1 : -1;
	void AddToOpenSet(std::shared_ptr<TheNode>&current, std::priority_queue<std::shared_ptr<TheNode>, std::vector<std::shared_ptr<TheNode>>, CompareTheNode>&TheOpenSet, std::map<std::shared_ptr<TheNode>, double, CompareSharedPtr>&TheClosedSet);
	auto Help = FindJumpRight(thedx, current, Goal, TheGrid);
	if (Help)
		AddToOpenSet(Help, TheOpenSet, TheClosedSet);
	Help = FindJumpUp(thedy, current, Goal, TheGrid);
	if (Help)
		AddToOpenSet(Help, TheOpenSet, TheClosedSet);
	current = std::make_shared<TheNode>(current->x + thedx, current->y + thedy, Goal, current);
	while (IsValid(current->x, current->y, TheGrid) && IsWalkable(current->x, current->y, TheGrid))
	{
		if (FindJumpRight(thedx, current, Goal, TheGrid) != nullptr)
			return current;
		if (FindJumpUp(thedy, current, Goal, TheGrid) != nullptr)
			return current;
		if (HasForcedNeighbor(current, TheGrid, thedx, thedy) || IsTheGoal(current, Goal))
			return current;
		current = std::make_shared<TheNode>(current->x + thedx, current->y + thedy, Goal, current);
	}
	return nullptr;
}
void AddToOpenSet(std::shared_ptr<TheNode>& current, std::priority_queue<std::shared_ptr<TheNode>, std::vector<std::shared_ptr<TheNode>>, CompareTheNode>& TheOpenSet, std::map<std::shared_ptr<TheNode>, double, CompareSharedPtr>& TheClosedSet) {
	if (TheClosedSet.find(current) == TheClosedSet.end() || current->Thef < TheClosedSet[current]) {
		{
			TheOpenSet.push(current);
			TheClosedSet[current] = current->Thef;
		}
	}
}
std::shared_ptr<std::vector<std::shared_ptr<TheNode>>> JPSRoad(std::shared_ptr<TheNode> current, std::shared_ptr<TheNode> Goal, const std::vector<std::vector<int>>& TheGrid)
{
	current = std::make_shared<TheNode>(current->x, current->y, Goal, nullptr);
	std::map<std::shared_ptr<TheNode>, double, CompareSharedPtr> TheClosedSet;
	std::priority_queue<std::shared_ptr<TheNode>, std::vector<std::shared_ptr<TheNode>>, CompareTheNode> TheOpenSet;
	TheOpenSet.push(current);
	static const int directions[4][2] = {
	{1, -1},
	{1, 1},
	{-1, 1},
	{-1, -1},
	};
	while (!TheOpenSet.empty())
	{
		auto TheCurrent = TheOpenSet.top();
		TheOpenSet.pop();
		TheClosedSet[TheCurrent] = TheCurrent->Thef;
		if (IsTheGoal(TheCurrent, Goal))
		{
			auto ThePath = std::make_shared<std::vector<std::shared_ptr<TheNode>>>();
			while (TheCurrent != nullptr)
			{
				ThePath->push_back(TheCurrent);
				TheCurrent = TheCurrent->TheLast;
			}
			std::reverse(ThePath->begin(), ThePath->end());
			return ThePath;
		}
		for (int i = 0; i < 4; i++)
		{
			int dx = directions[i][0];
			int dy = directions[i][1];
			std::shared_ptr<TheNode> TheJumpedNode;
			TheJumpedNode = FindJumpDiagonal(dx, dy, TheCurrent, Goal, TheGrid, TheClosedSet, TheOpenSet);
			if (TheJumpedNode != nullptr)
			{
				AddToOpenSet(TheJumpedNode, TheOpenSet, TheClosedSet);
			}
		}
	}
	return std::make_shared<std::vector<std::shared_ptr<TheNode>>>();
}

