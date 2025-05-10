#pragma once
#include"PhyEngS.cuh"
#include"physis.h"
#include<list>
class Player
{
	std::shared_ptr<physis> ThePhysis;//����ʵ��
	int maxhealth;//�������ֵ
	int health;//����ֵ
	int attack;//������
	int defense;//������
	int CurrentDx, CurrentDy;
	char LeftChar, RightChar;
	char CurrentChar;
	int currentColor;
	void PlayerChangeSlow(int dx, int dy)
	{
		ThePhysis->PlayerChaneSlow(dx, dy);
	}
	void ChangeHealth(int Change)
	{
		health += Change;
		if (health > maxhealth)
			health = maxhealth;
		else if (health < 0)
			health = 0;
	}
public:
	int GetDefense() {
		return defense;
	}
	int GetHealth() {
		return health;
	}
	int GetAttack()
	{
		return attack;
	}
	int GetX() {
		return ThePhysis->GetPlayerX();
	}
	int GetY() {
		return ThePhysis->GetPlayerY();
	}
	Player() = delete;
	Player(std::shared_ptr<physis> aphysis) :
		ThePhysis(aphysis), maxhealth(100), health(100), attack(5), defense(1), CurrentDx(0), CurrentDy(-1), LeftChar('q'), 
		RightChar('p'),CurrentChar('p'),currentColor(0x0007)
	{
		ThePhysis->ChangePlayerAttack(attack);
	}
	void PlayerAttack();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void PlayerBeHitted();
};

