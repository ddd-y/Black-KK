#pragma once
#include"PhyEng.h"
class Player
{
	std::shared_ptr<physis> ThePhysis;//����ʵ��
	int maxhealth;//�������ֵ
	int health;//����ֵ
	int attack;//������
	int defense;//������
	int x, y;//����
public:
	void ChangeHealth(int Change)
	{
		health += Change;
		if (health > maxhealth)
			health = maxhealth;
		else if (health < 0)
			health = 0;
	}
	int GetDefense() {
		return defense;
	}
	int GetHealth() {
		return health;
	}
	int GetX() {
		return x;
	}
	int GetY() {
		return y;
	}
};

