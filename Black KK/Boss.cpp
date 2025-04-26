#include "Boss.h"
#include "Player.h"
inline bool ChuiZhi(std::shared_ptr<TheNode> a)
{
	return (a->x != 0) != (a->y != 0);
}
bool Boss::IfDistance()//true表示要继续追击
{
	int distance = abs(x - ThePlayer->GetX()) + abs(y - ThePlayer->GetY());
	return distance > 8;
}
void Boss::GetNextStep()
{
	auto Goal = std::make_shared<TheNode>(ThePlayer->GetX(), ThePlayer->GetY(), nullptr, nullptr);
	auto current = std::make_shared<TheNode>(x, y, nullptr, nullptr);
	auto ThePath = JPSRoad(current, Goal, *TheGrid);
	if (ThePath->size() > 1)
	{
		if (ChuiZhi((*ThePath)[0]))
		{
			NextStep[0][0] = (*ThePath)[0]->x;
			NextStep[0][1] = (*ThePath)[0]->y;
			if (ChuiZhi((*ThePath)[1]))
			{
				NextStep[1][0] = (*ThePath)[1]->x;
				NextStep[1][1] = (*ThePath)[1]->y;
			}
			else
			{
				NextStep[1][0] = (*ThePath)[1]->x;
				NextStep[1][1] = 0;
			}
		}
		else
		{
			NextStep[0][0] = (*ThePath)[1]->x;
			NextStep[0][1] = 0;
			NextStep[1][0] = 0;
			NextStep[1][1] = (*ThePath)[1]->y;
		}
		CanMoVeSlow = true;
	}
	else if (ThePath->size() == 1)
	{
		if (ChuiZhi((*ThePath)[0]))
		{
			NextStep[0][0] = (*ThePath)[0]->x;
			NextStep[0][1] = (*ThePath)[0]->y;
			NextStep[1][0] = 0;
			NextStep[1][1] = 0;
		}
		else
		{
			NextStep[0][0] = 0;
			NextStep[0][1] = (*ThePath)[0]->y;
			NextStep[1][0] = (*ThePath)[0]->x;
			NextStep[1][1] = 0;
		}
		CanMoVeSlow = true;
	}
	else
	{
		CanMoVeSlow = false;
	}
}
void Boss::MoveSlow()
{
	if (currentmovechoice == 0)
	{
		currentmovechoice = 1;
	}
	else
	{
		currentmovechoice = 0;
	}
	int dx = NextStep[currentmovechoice][0];
	int dy = NextStep[currentmovechoice][1];
	if (!Move(dx, dy))
		Flash();
}
void Boss::Flash()
{
	int playerX = ThePlayer->GetX();
	int playerY = ThePlayer->GetY();
	int goalX, goalY;
	bool validPosition = false;
	std::srand(std::time(nullptr));
	while (!validPosition) {
		int dx = (std::rand() % 17) - 8; 
		int dy = 8 - abs(dx);
		if (std::rand() % 2 == 0) {
			dy = -dy;
		}
		goalX = playerX + dx;
		goalY = playerY + dy;
		if (goalX >= 0 && goalX < TheGrid->at(0).size() && goalY >= 0 && goalY < TheGrid->size() &&TheGrid->at(goalY).at(goalX) != 1) {
			validPosition = true;
		}
	}
	ChangeLocation(goalX, goalY);
	CanMoVeSlow = true;
}
Boss::Boss()
{
}

Boss::~Boss()
{
}
