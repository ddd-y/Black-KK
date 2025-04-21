#pragma once
#include"PhyEng.h"
class Player
{
	std::shared_ptr<physis> ThePhysis;//物理实体
	int maxhealth;//最大生命值
	int health;//生命值
	int attack;//攻击力
	int defense;//防御力
	int x, y;//坐标
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

