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
	std::array<std::array<int, 2>, 4> NextStep;//下一个目标点,存两个，让BOSS慢于玩家移动,0为x，1为y
	int x, y;
	int maxhealth;//最大生命值
	int health;//生命值
	int attack;//攻击力
	int defense;//防御力
	int currentmovechoice=0;//当前移动选择
	bool CanMoVeSlow;//是否可以一步步移动，用于行为树
	bool Move(int dx, int dy) 
	{
		if (ThePhysis->IfHit(dx, dy))
		   return false;
		x += dx;
		y += dy;
		ThePhysis->ChangeLocation(x, y);
		return true;
	}
	void ChangeLocation(int TheNewX, int TheNewY)
	{
		x = TheNewX;
		y = TheNewY;
		ThePhysis->ChangeLocation(x, y);
	}
public:
	bool IfDistance();//判断是否需要继续移动追击
	bool GetCanMoVeSlow()
	{
		return CanMoVeSlow;
	}
	void GetNextStep();
	void MoveSlow();
	bool IfSecond()//true为第二阶段
	{
		return health * 2 < maxhealth;
	}
	void Flash();//闪现
	Boss();
	~Boss();
};

