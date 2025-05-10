#include "Boss.h"
void Boss::BossBeHitted()
{
	int PlayerAtt = ThePhysis->GetPlayerAttack();
	int Hurt = ( - PlayerAtt) / defense;
	for (int i = 0, j = ThePhysis->GetBossBeHitterTime(); i < j; ++i)
	{
		ChangeHealth(Hurt);
	}
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
	int distance = abs( GetMeX() - ThePhysis->GetPlayerX()) + abs(GetMeY() - ThePhysis->GetPlayerY());
	return distance < 16;
}
void Boss::GetNextStep()
{
	auto Goal = std::make_shared<TheNode>(ThePhysis->GetPlayerX(), ThePhysis->GetPlayerY(), nullptr, nullptr);
	auto current = std::make_shared<TheNode>(GetMeX(), GetMeY(), nullptr, nullptr);
	auto ThePath = JPSRoad(current, Goal, *TheGrid);
	int NewX1 = 0, NewY1 = 0;
	if (ThePath->size() > 1)
	{
		NewX1 = (*ThePath)[1]->x;
		NewY1 = (*ThePath)[1]->y;
	}
	else
		return;
	int currentX = ThePhysis->GetBossX();
	int currentY = ThePhysis->GetBossY();
	bool valid = (currentX != NewX1)&&(currentY != NewY1);
	if (valid)
		NewY1 = currentY;
	if (ChangeLocation(NewX1, NewY1))
	{
		return;
	}
	NewY1 = (*ThePath)[1]->y;
	NewX1 = currentX;
	ChangeLocation(NewX1, NewY1);
}
void Boss::Flash()
{
	int playerX = ThePhysis->GetPlayerX();
	int playerY = ThePhysis->GetPlayerY();
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
}
Boss::~Boss()
{
}
