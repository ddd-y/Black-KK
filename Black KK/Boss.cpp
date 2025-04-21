#include "Boss.h"
#include "Player.h"
void Boss::GetNextStep()
{
	auto Goal = std::make_shared<TheNode>(ThePlayer->GetX(), ThePlayer->GetY(), nullptr, nullptr);
	auto current = std::make_shared<TheNode>(x, y, nullptr, nullptr);
	auto ThePath = JPSRoad(current, Goal, *TheGrid);
	if (ThePath.size() > 1)
	{
		NextStep[0][0] = ThePath[1]->x;
		NextStep[0][1] = ThePath[1]->y;
		NextStep[1][0] = ThePath[2]->x;
		NextStep[1][1] = ThePath[2]->y;
	}
	else if (ThePath.size() == 1)
	{
		NextStep[0][0] = ThePath[0]->x;
		NextStep[0][1] = ThePath[0]->y;
	}
	else
	{
		CanMoVeSlow = false;
	}
}
Boss::Boss()
{
}

Boss::~Boss()
{
}
