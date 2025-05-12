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
	std::shared_ptr<physis> ThePhysis;//物理实体
	std::array<int, 2> NextStep;
	int maxhealth;//最大生命值
	int health;//生命值
	int attack;//攻击力
	int defense;//防御力
	int FirstFlash;//第一次闪现概率
	int SecondFlash;//第二次闪现概率
	int currentFlash;//当前闪现概率
	int Speednow;//当前速度
	int SpeedFirst;//第一阶段速度
	int SpeedSecond;//第二阶段速度
	int AttackPing;//攻击频率
	int AttackFirst;//第一阶段攻击频率
	int AttackSecond;//第二阶段攻击频率
	bool HaveNextStep;//有下一步
	void ChangeHealth(int Change);
	bool ChangeLocation(int TheNewX, int TheNewY)//闪现，用于Boss的技能，该点是否能降落由其他来判断
	{
		if(!ThePhysis->ChangeLocation(TheNewX, TheNewY))
			return false;
		return true;
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
	bool IfDistance();//判断是否在攻击范围内
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
	bool IfSecond()//true为第二阶段
	{
		if (health * 2 < maxhealth)
		{
			currentFlash = SecondFlash;
			Speednow = SpeedSecond;
			AttackPing = AttackSecond;
			ThePhysis->BossChangeSecond();
			return true;
		}
		return false;
	}
	bool IfTrace();//是否在追击范围内
	void Flash();//闪现
	void BossBeHitted();//被子弹打到时的反应
	Boss() = delete;
	Boss(std::shared_ptr<physis> aphysis) :
		ThePhysis(aphysis),maxhealth(100), health(100), attack(5), defense(1),
		FirstFlash(0), SecondFlash(5), currentFlash(FirstFlash), SpeedFirst(15), SpeedSecond(8), AttackFirst(20), AttackSecond(15),HaveNextStep(false)
	{
		TheGrid = ThePhysis->GetTheGrid();
		Speednow = SpeedFirst;
		AttackPing = AttackFirst;
		ThePhysis->ChangeBossAttack(attack);
		auto aNode = std::make_shared<TheNode>();
		aNode->PreprocessJPSPlus(*TheGrid);
	}
	~Boss();
};

