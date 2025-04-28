#pragma once
#include"TheNode.h"
#include"PhyEngS.cuh"
#include"physis.h"
#include<array>
#include<ctime>
class Player;
class Boss
{
private:
	std::shared_ptr<std::vector<std::vector<int>>> TheGrid;
	std::shared_ptr<physis> ThePhysis;//����ʵ��
	int maxhealth;//�������ֵ
	int health;//����ֵ
	int attack;//������
	int defense;//������
	int FirstFlash;//��һ�����ָ���
	int SecondFlash;//�ڶ������ָ���
	int currentFlash;//��ǰ���ָ���
	int Speednow;//��ǰ�ٶ�
	int SpeedFirst;//��һ�׶��ٶ�
	int SpeedSecond;//�ڶ��׶��ٶ�
	int AttackPing;//����Ƶ��
	int AttackFirst;//��һ�׶ι���Ƶ��
	int AttackSecond;//�ڶ��׶ι���Ƶ��
	void ChangeHealth(int Change);
	void ChangeLocation(int TheNewX, int TheNewY)//���֣�����Boss�ļ��ܣ��õ��Ƿ��ܽ������������ж�
	{
		ThePhysis->ChangeLocation(TheNewX, TheNewY);
	}
	int GetMeX()
	{
		return ThePhysis->GetBossX();
	}
	int GetMeY()
	{
		return ThePhysis->GetBossY();
	}
public:
	int GetHealthNow()
	{
		return health;
	}
	int GetSpeednow()
	{
		return Speednow;
	}
	int GetAttackPing()
	{
		return AttackPing;
	}
	bool IfDistance();//�ж��Ƿ��ڹ�����Χ��
	void Skillfirst()
	{
		ThePhysis->BossSpawnFourwayBullet();
	}
	void Skillsecond()
	{
		ThePhysis->BossSpawnEightwayBullet();
	}
	bool IfNeedToFlash() {
		std::srand(std::time(nullptr));
		int therandom = rand() % 100;
		return ThePhysis->IfBossNeedToFlash()||therandom<currentFlash;
	}
	void GetNextStep();
	bool IfSecond()//trueΪ�ڶ��׶�
	{
		if (health * 2 < maxhealth)
		{
			currentFlash = SecondFlash;
			Speednow = SpeedSecond;
			AttackPing = AttackSecond;
			return true;
		}
		return false;
	}
	void Flash();//����
	void BossBeHitted();//���ӵ���ʱ�ķ�Ӧ
	Boss() = delete;
	Boss(std::shared_ptr<physis> aphysis) :
		ThePhysis(aphysis),maxhealth(100), health(100), attack(3), defense(1),
		FirstFlash(10), SecondFlash(20), currentFlash(FirstFlash), SpeedFirst(3), SpeedSecond(2), AttackFirst(10), AttackSecond(5)
	{
		TheGrid = ThePhysis->GetTheGrid();
		Speednow = SpeedFirst;
		AttackPing = AttackFirst;
		ThePhysis->ChangeBossAttack(attack);
	}
	~Boss();
};

