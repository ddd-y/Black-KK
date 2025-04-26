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
	std::shared_ptr<physis> ThePhysis;//����ʵ��
	std::array<std::array<int, 2>, 4> NextStep;//��һ��Ŀ���,����������BOSS��������ƶ�,0Ϊx��1Ϊy
	int x, y;
	int maxhealth;//�������ֵ
	int health;//����ֵ
	int attack;//������
	int defense;//������
	int currentmovechoice=0;//��ǰ�ƶ�ѡ��
	bool CanMoVeSlow;//�Ƿ����һ�����ƶ���������Ϊ��
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
	bool IfDistance();//�ж��Ƿ���Ҫ�����ƶ�׷��
	bool GetCanMoVeSlow()
	{
		return CanMoVeSlow;
	}
	void GetNextStep();
	void MoveSlow();
	bool IfSecond()//trueΪ�ڶ��׶�
	{
		return health * 2 < maxhealth;
	}
	void Flash();//����
	Boss();
	~Boss();
};

