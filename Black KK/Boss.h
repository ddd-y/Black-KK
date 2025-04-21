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
	std::array<std::array<int, 2>, 2> NextStep;//��һ��Ŀ���,����������BOSS��������ƶ�,0Ϊx��1Ϊy
	int x, y;
	int maxhealth;//�������ֵ
	int health;//����ֵ
	int attack;//������
	int defense;//������
	bool CanMoVeSlow;//�Ƿ����һ�����ƶ���������Ϊ��
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

