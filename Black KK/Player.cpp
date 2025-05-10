#include "Player.h"

void Player::PlayerAttack()
{
	ThePhysis->PlayerSpawnBullet(CurrentDx, CurrentDy);
}

void Player::MoveUp()
{
	CurrentDx = 0;
	CurrentDy = -1;
	ThePhysis->ChangePlayerWay(CurrentChar, currentColor);
	PlayerChangeSlow(CurrentDx, CurrentDy);
}

void Player::MoveDown()
{
	CurrentDx = 0;
	CurrentDy = 1;
	ThePhysis->ChangePlayerWay(CurrentChar, currentColor);
	PlayerChangeSlow(CurrentDx, CurrentDy);
}

void Player::MoveLeft()
{
	CurrentDx = -1;
	CurrentDy = 0;
	CurrentChar = LeftChar;
	ThePhysis->ChangePlayerWay(CurrentChar, currentColor);
	PlayerChangeSlow(CurrentDx, CurrentDy);
}

void Player::MoveRight()
{
	CurrentDx = 1;
	CurrentDy = 0;
	CurrentChar = RightChar;
	ThePhysis->ChangePlayerWay(CurrentChar, currentColor);
	PlayerChangeSlow(CurrentDx, CurrentDy);
}

void Player::PlayerBeHitted()
{
	int attack = ThePhysis->GetBossAttack();
	int hurt = (-attack) / defense;
	for (int i = 0, j = ThePhysis->GetPlayerBeHittedTime(); i < j; ++i) 
	{
		ChangeHealth(hurt);
	}
	if (health * 2 <= maxhealth)
		currentColor = 0x0001;
}
