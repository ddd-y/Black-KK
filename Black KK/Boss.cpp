#include "Boss.h"
void Boss::BossBeHitted()
{
	bool IfBeHitted = false;
	int PlayerAtt = ThePhysis->GetPlayerAttack();
	int Hurt = ( - PlayerAtt) / defense;
	for (int i = 0, j = ThePhysis->GetBossBeHitterTime(); i < j; ++i)
	{
		ChangeHealth(Hurt);
		IfBeHitted = true;
	}
	if (IfBeHitted)
		Flash();
}
inline void Boss::ChangeHealth(int Change)
{
	health += Change;
	if (health >= maxhealth)
		health = maxhealth;
	if (health < 0)
		health = 0;
}
bool Boss::IfDistance()//true表示在攻击范围内
{
	int XDis = abs(GetMeX() - ThePhysis->GetPlayerX());
	int YDis = abs(GetMeY() - ThePhysis->GetPlayerY());
	return XDis<24&&YDis<24;
}
void Boss::GetNextStep()
{
	if (HaveNextStep)
	{
		ChangeLocation(NextStep[0], NextStep[1]);
		HaveNextStep = false;
		return;
	}
	auto Goal = std::make_shared<TheNode>(ThePhysis->GetPlayerX(), ThePhysis->GetPlayerY(), nullptr, nullptr);
	auto current = std::make_shared<TheNode>(GetMeX(), GetMeY(), nullptr, nullptr);
	auto ThePath = JPSRoad(current, Goal, *TheGrid);
	if (ThePath->size() > 1)
	{
		int currentX = ThePhysis->GetBossX();
		int currentY = ThePhysis->GetBossY();
		int nextX = (*ThePath)[1]->x;
		int nextY = (*ThePath)[1]->y;
		int tryX =currentX, tryY =currentY;
		if (currentX != nextX)
			tryX = currentX + ((nextX > currentX) ? 1 : -1);
		if (currentY != nextY)
			tryY = currentY + ((nextY > currentY) ? 1 : -1);
		if(tryX!=currentX&&tryY!=currentY)
		{
			if (ChangeLocation(tryX, currentY))
			{
				HaveNextStep = true;
				NextStep[0] = tryX;
				NextStep[1] = tryY;
			}
			else if(ChangeLocation(currentX,tryY))
			{
				HaveNextStep = true;
				NextStep[0] = tryX;
				NextStep[1] = tryY;
			}
		}
		else 
		{
			ChangeLocation(tryX, tryY);
		}
	}
	else
	{
		Flash();
		return;
	}
}
bool Boss::IfTrace()
{
	int XDis = abs(GetMeX() - ThePhysis->GetPlayerX());
	int YDis = abs(GetMeY() - ThePhysis->GetPlayerY());
	return XDis < 31 && YDis < 31;
}
void Boss::Flash()
{
	int playerX = ThePhysis->GetPlayerX();
	int playerY = ThePhysis->GetPlayerY();
	int goalX, goalY;
	bool validPosition = false;
	std::srand(std::time(nullptr));
	for(int i=0;i<40;++i)
	{
		int dx = (std::rand() % 37) - 18; 
		int dy = (std::rand() % 37) - 18; 
		goalX = playerX + dx;
		goalY = playerY + dy;
		if (goalX >= 0 && goalX < TheGrid->at(0).size() && goalY >= 0 && goalY < TheGrid->size() &&TheGrid->at(goalY).at(goalX) != 1) {
			validPosition = true;
		}
		if (validPosition)
			break;
	}
	if(validPosition)
	ChangeLocation(goalX, goalY);
}
Boss::~Boss()
{
}
