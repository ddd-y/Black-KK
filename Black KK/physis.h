#pragma once
#include"PhyEngS.cuh"
#include<memory>
#include<vector>
class PhyEngS;
class physis
{
protected:
	int BossHitTime;//Boss撞墙的次数
	std::shared_ptr<PhyEngS> ThePhyEng;//物理引擎
public:
	void ChangeBossAttack(int NewAttack){
		ThePhyEng->ChangeBossAttack(NewAttack);
	}
	int GetBossAttack(){
		return ThePhyEng->GetBossAttack();
	}
	void ChangePlayerAttack(int NewAttack){
		ThePhyEng->ChangePlayerAttack(NewAttack);
	}
	int GetPlayerAttack() {
		return ThePhyEng->GetPlayerAttack();
	}
	std::shared_ptr<std::vector<std::vector<int>>> GetTheGrid()
	{
		return ThePhyEng->GetGrid();
	}
	int GetPlayerX()
	{
		return ThePhyEng->GetPlayerX();
	}
	int GetPlayerY()
	{
		return ThePhyEng->GetPlayerY();
	}
	int GetBossX() {
		return ThePhyEng->GetBossX();
	}
	int GetBossY() {
		return ThePhyEng->GetBossY();
	}
	virtual ~physis();
	physis() = delete;
	physis(std::shared_ptr<PhyEngS> Phyeng) :ThePhyEng(Phyeng),
		BossHitTime(0)
	{
	}
	bool ChangeLocation(int TheNewX, int TheNewY)
	{
		if (!ThePhyEng->WantToChangeBossLocation(TheNewX, TheNewY))
		{
			++BossHitTime;
			return false;
		}
		return true;
	}
	void PlayerChaneSlow(int dx,int dy)
	{
		int NewX = ThePhyEng->GetPlayerX() + dx;
		int NewY = ThePhyEng->GetPlayerY() + dy;
		ThePhyEng->WantToChangePlayerLocation(NewX, NewY);
	}
	bool IfBossNeedToFlash()//判断Boss是否需要闪现
	{
		if (BossHitTime > 4)
		{
			BossHitTime = 0;
			return true;
        }
		return false;
	}
	int GetPlayerBeHittedTime() {
		return ThePhyEng->GetPlayerBeHittedTime();
	}
	int GetBossBeHitterTime() {
		return ThePhyEng->GetBossBeHittedTime();
	}
	void BossSpawnFourwayBullet();
	void BossSpawnEightwayBullet();
	void PlayerSpawnBullet(int dx,int dy);
};

