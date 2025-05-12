#include "TheNode.h"
#include <iostream>
std::unordered_map<int, std::unordered_map<int, PreJumpNode>> TheNode::preprocessData;
bool operator<(const std::shared_ptr<TheNode>& lhs, const std::shared_ptr<TheNode>& rhs) {
	return lhs->x < rhs->x || (lhs->x == rhs->x && lhs->y < rhs->y);
}

bool IsValid(int thex, int they, const std::vector<std::vector<int>>& TheGrid) {//判断坐标是否合法
	return (thex >= 0 && they >= 0 && thex < TheGrid[0].size() && they < TheGrid.size());
}

bool IsWalkable(int thex, int they, const std::vector<std::vector<int>>& TheGrid) {//判断坐标是否可走,0表示可走，1表示不可走
	return (TheGrid[they][thex] == 0);
}

bool IsSameWay(int dx,int dy,int x,int y,int Goalx,int Goaly)
{
	int dirX = Goalx - x;
	int dirY = Goaly - y;
	if (dirX != 0) 
		dirX = (dirX > 0) ? 1 : -1;
	if (dirY != 0) 
		dirY = (dirY > 0) ? 1 : -1;
	if (dx != 0) 
		dx = (dx > 0) ? 1 : -1;
	if (dy != 0) 
		dy = (dy > 0) ? 1 : -1;
	if (dx == 0 && dy == 0) return false;
	if (dirX == 0 && dirY == 0) return false;
	return (dx == dirX && dy == dirY);
}

bool IsTheGoal(std::shared_ptr<TheNode>& current, std::shared_ptr<TheNode>& Goal) {
	if (!current || !Goal)
		return false;
	return (current->x == Goal->x && current->y == Goal->y);
}
void AddToOpenSet(std::shared_ptr<TheNode>& current, std::priority_queue<std::shared_ptr<TheNode>, std::vector<std::shared_ptr<TheNode>>, CompareTheNode>& TheOpenSet, 
	std::map<std::shared_ptr<TheNode>, double, CompareSharedPtr>& TheClosedSet) 
{
	if (TheClosedSet.find(current) == TheClosedSet.end() || current->Thef < TheClosedSet[current]) {
		{
			TheOpenSet.push(current);
			TheClosedSet[current] = current->Thef;
		}
	}
}

void ProcessPreJumpNode(std::shared_ptr<TheNode> CurrentNode,std::shared_ptr<TheNode>GoalNode,
	const PreJumpNode& WaitNode, const std::vector<std::vector<int>>& TheGrid, 
	std::priority_queue<std::shared_ptr<TheNode>, std::vector<std::shared_ptr<TheNode>>, CompareTheNode>& TheOpenSet,
	std::map<std::shared_ptr<TheNode>, double, CompareSharedPtr>& TheClosedSet)
{
	int x = CurrentNode->x;
	int y = CurrentNode->y;
	int Goalx = GoalNode->x;
	int Goaly = GoalNode->y;
	const int dirs[8][2] = { {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1} };
	for (int i = 0; i < 8; ++i)
	{
		if (WaitNode.Distances[i] == 0) 
			continue;
		int dx = dirs[i][0] * WaitNode.Distances[i];
		int dy = dirs[i][1] * WaitNode.Distances[i];
		if (WaitNode.IsJump[i])
		{
			auto NewNode = std::make_shared<TheNode>(x + dx, y + dy, GoalNode, CurrentNode);
			AddToOpenSet(NewNode, TheOpenSet, TheClosedSet);
			continue;
		}
		if(IsSameWay(dx,dy,x,y,Goalx,Goaly))
		{
			int tx = x;
			int ty = y;
			int step_dx = dirs[i][0];
			int step_dy = dirs[i][1];
			int steps = (step_dx == 0 || step_dy == 0)? std::max(abs(Goalx - x), abs(Goaly - y)): std::min(abs(Goalx - x), abs(Goaly - y));
			if (steps > WaitNode.Distances[i])
				steps = WaitNode.Distances[i];
			for(int k=0;k<steps;++k)
			{
				tx += step_dx;
				ty += step_dy;
				if (!IsValid(tx, ty, TheGrid) || !IsWalkable(tx, ty, TheGrid))
					break;
				if (tx == Goalx && ty == Goaly)
					break;
			}
			if (tx == x && ty == y)
				continue;
			auto NewNode = std::make_shared<TheNode>(tx, ty, GoalNode, CurrentNode);
			AddToOpenSet(NewNode, TheOpenSet, TheClosedSet);
		}
	}
}

std::shared_ptr<std::vector<std::shared_ptr<TheNode>>> JPSRoad(std::shared_ptr<TheNode> current, std::shared_ptr<TheNode> Goal, const std::vector<std::vector<int>>& TheGrid)
{
	current = std::make_shared<TheNode>(current->x, current->y, Goal, nullptr);
	std::priority_queue<std::shared_ptr<TheNode>, std::vector<std::shared_ptr<TheNode>>, CompareTheNode> TheOpenSet;
	std::map<std::shared_ptr<TheNode>, double, CompareSharedPtr> TheClosedSet;
	TheOpenSet.push(current);
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
		auto PreNode = TheNode::preprocessData[TheCurrent->x][TheCurrent->y];
		ProcessPreJumpNode(TheCurrent, Goal, PreNode, TheGrid, TheOpenSet, TheClosedSet);
	}
	return std::make_shared<std::vector<std::shared_ptr<TheNode>>>();
}
void TheNode::PreprocessJPSPlus(const std::vector<std::vector<int>>& TheGrid)
{
	const int dirs[8][2] = { {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1} };
	for(int y=0,Height=TheGrid.size();y<Height;++y)
	{
		for (int x = 0, Width = TheGrid[0].size(); x < Width; ++x) 
		{
			if (!IsWalkable(x, y, TheGrid))
				continue;
			PreJumpNode data;
			for(int d=0;d<8;++d)
			{
				int dx = dirs[d][0];
				int dy = dirs[d][1];
				int distance = 0;
                if(dx==0||dy==0)
				{
					while (true)
					{
						int nx = (distance + 1) * dx + x;
						int ny = y + dy * (distance + 1);
						if (!IsValid(nx, ny, TheGrid) || !IsWalkable(nx, ny, TheGrid))
							break;
						if (IfHasForcedNeighbor(nx, ny, dx, dy, TheGrid))
						{
							++distance;
							data.IsJump[d] = true;
							break;
						}
						++distance;
					}
				}
				else
				{
				    while(true)
				    {
						int nx = (distance + 1) * dx + x;
						int ny = (distance + 1) * dy + y;
						if (!IsValid(nx, ny, TheGrid) || !IsWalkable(nx, ny, TheGrid))
							break;
						if(IfDiaForced(nx,ny,dx,dy,TheGrid))
						{
							++distance;
							data.IsJump[d] = true;
							break;
						}
						++distance;
				    }
				}
				data.Distances[d] = distance;
			}
			preprocessData[x][y] = data;
	    }
	}
}
bool TheNode::IfHasForcedNeighbor(int x, int y, int dx, int dy, const std::vector<std::vector<int>>& TheGrid)
{
	if (dy == 0 && dx != 0)
	{
		if (dx > 0)
		{
			if (IsValid(x, y - 1, TheGrid) && !IsWalkable(x, y - 1, TheGrid))
			{
				if (IsValid(x + 1, y - 1, TheGrid) && IsWalkable(x + 1, y - 1, TheGrid))
				{
					return true;
				}
			}
			if (IsValid(x, y + 1, TheGrid) && !IsWalkable(x, y + 1, TheGrid))
			{
				if (IsValid(x + 1,y + 1, TheGrid) && IsWalkable(x + 1, y + 1, TheGrid))
				{
					return true;
				}
			}
		}
		else
		{
			if (IsValid(x, y - 1, TheGrid) && !IsWalkable(x, y - 1, TheGrid))
			{
				if (IsValid(x - 1, y - 1, TheGrid) && IsWalkable(x - 1, y - 1, TheGrid))
				{
					return true;
				}
			}
			if (IsValid(x, y + 1, TheGrid) && !IsWalkable(x, y + 1, TheGrid))
			{
				if (IsValid(x - 1, y + 1, TheGrid) && IsWalkable(x - 1,y + 1, TheGrid))
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
			if (IsValid(x - 1, y, TheGrid) && !IsWalkable(x - 1, y, TheGrid))
			{
				if (IsValid(x - 1, y + 1, TheGrid) && IsWalkable(x - 1, y + 1, TheGrid))
				{
					return true;
				}
			}
			if (IsValid(x + 1, y, TheGrid) && !IsWalkable(x + 1, y, TheGrid))
			{
				if (IsValid(x + 1, y + 1, TheGrid) && IsWalkable(x + 1,y + 1, TheGrid))
				{
					return true;
				}
			}
		}
		else
		{
			if (IsValid(x - 1, y, TheGrid) && !IsWalkable(x - 1, y, TheGrid))
			{
				if (IsValid(x - 1, y - 1, TheGrid) && IsWalkable(x - 1, y - 1, TheGrid))
				{
					return true;
				}
			}
			if (IsValid(x + 1, y, TheGrid) && !IsWalkable(x + 1,y, TheGrid))
			{
				if (IsValid(x + 1,y - 1, TheGrid) && IsWalkable(x + 1, y - 1, TheGrid))
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
		if (IsValid(x - thedx, y, TheGrid) && !IsWalkable(x - thedx, y, TheGrid))
		{
			if (IsValid(x - thedx, y + thedy, TheGrid) && IsWalkable(x - thedx, y + thedy, TheGrid))
			{
				return true;
			}
		}
		if (IsValid(x, y - thedy, TheGrid) && !IsWalkable(x, y - thedy, TheGrid))
		{
			if (IsValid(x + thedx, y - thedy, TheGrid) && IsWalkable(x + thedx, y - thedy, TheGrid))
			{
				return true;
			}
		}
	}
	return false;
}

bool TheNode::IfWayXForced(int x, int y,int dx,const std::vector<std::vector<int>>& TheGrid)
{
	if (dx == 0)
		return false;
	dx = (dx > 0) ? 1 : -1;
	int nx = x + dx;
	int ny = y;
	while (IsValid(nx, ny, TheGrid) && IsWalkable(nx, ny, TheGrid)) 
	{
		if (IfHasForcedNeighbor(nx, ny, dx, 0, TheGrid))
			return true;
		nx = nx + dx;
	}
	return false;
}

bool TheNode::IfWayYForced(int x, int y, int dy, const std::vector<std::vector<int>>& TheGrid)
{
	if (dy == 0)
		return false;
	dy = (dy > 0) ? 1 : -1;
	int nx = x;
	int ny = y + dy;
	while(IsValid(nx,ny,TheGrid)&&IsWalkable(nx,ny,TheGrid))
	{
		if (IfHasForcedNeighbor(nx, ny, 0, dy, TheGrid))
			return true;
		ny = ny + dy;
	}
	return false;
}

bool TheNode::IfDiaForced(int x, int y, int dx, int dy, const std::vector<std::vector<int>>& TheGrid)
{
	if (dx == 0 || dy == 0)
		return false;
	dx = (dx > 0) ? 1 : -1;
	dy = (dy > 0) ? 1 : -1;
    if(IsValid(x,y,TheGrid)&&IsWalkable(x,y,TheGrid))
	{
		if (IfHasForcedNeighbor(x, y, dx, dy, TheGrid))
			return true;
		if (IfWayXForced(x, y, dx, TheGrid))
			return true;
		if (IfWayYForced(x, y, dy, TheGrid))
			return true;
	}
	return false;
}
