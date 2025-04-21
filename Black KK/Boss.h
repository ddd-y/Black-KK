#pragma once
#include"TheNode.h"
#include"PhyEng.h"
#include<array>
class Player;
class Boss
{
private:
	std::shared_ptr<Player> ThePlayer;
	std::shared_ptr<std::vector<std::vector<int>>> TheGrid;
	std::shared_ptr<physis> ThePhysis;//物理实体
	std::array<std::array<int, 2>, 2> NextStep;//下一个目标点,存两个，让BOSS慢于玩家移动,0为x，1为y
	int x, y;
	int maxhealth;//最大生命值
	int health;//生命值
	int attack;//攻击力
	int defense;//防御力
	bool CanMoVeSlow;//是否可以一步步移动，用于行为树
	void Move(int dx, int dy) 
	{
		if (ThePhysis->IfHit(dx, dy))
		   return;
		x += dx;
		y += dy;
	}
	void GetNextStep();
public:
	Boss();
	~Boss();
};

