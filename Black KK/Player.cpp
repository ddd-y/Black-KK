#include "Player.h"

void Player::PlayerAttack()
{
	ThePhysis->PlayerSpawnBullet(CurrentDx, CurrentDy);
}

void Player::MoveUp()
{
	CurrentDx = 0;
	CurrentDy = -1;
	PlayerChangeSlow(CurrentDx, CurrentDy);
}

void Player::MoveDown()
{
	CurrentDx = 0;
	CurrentDy = 1;
	PlayerChangeSlow(CurrentDx, CurrentDy);
}

void Player::MoveLeft()
{
	CurrentDx = -1;
	CurrentDy = 0;
	PlayerChangeSlow(CurrentDx, CurrentDy);
}

void Player::MoveRight()
{
	CurrentDx = 1;
	CurrentDy = 0;
	PlayerChangeSlow(CurrentDx, CurrentDy);
}

void Player::PlayerBeHitted()
{
	for (int i = 0, j = ThePhysis->GetPlayerBeHittedTime(); i < j; ++i) 
	{
		int hurt = ThePhysis->GetBossAttack();
		hurt /= -defense;
		ChangeHealth(hurt);
	}
}
