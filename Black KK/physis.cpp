#include "physis.h"
#include<mutex>
physis::~physis()
{
}
void physis::BossSpawnFourwayBullet()
{
	std::mutex* Temp = ThePhyEng->GetBossBulletMu();
	Temp->lock();
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 1, 0);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 0, 1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), -1, 0);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 0, -1);
	Temp->unlock();
}

void physis::BossSpawnEightwayBullet()
{
	std::mutex* Temp = ThePhyEng->GetBossBulletMu();
	Temp->lock();
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 1, 0);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 0, 1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), -1, 0);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 0, -1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 1, 1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), -1, 1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 1, -1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), -1, -1);
	Temp->unlock();
}

void physis::PlayerSpawnBullet(int dx, int dy)
{
	std::mutex* Temp = ThePhyEng->GetPlayerBulletMu();
	Temp->lock();
	ThePhyEng->spawnPlayerBullet(ThePhyEng->GetPlayerX(), ThePhyEng->GetPlayerY(), dx, dy);
	Temp->unlock();
}

