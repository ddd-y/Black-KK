#include "physis.h"
physis::~physis()
{
}
void physis::BossSpawnFourwayBullet()
{
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 1, 0);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 0, 1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), -1, 0);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 0, -1);
}

void physis::BossSpawnEightwayBullet()
{
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 1, 0);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 0, 1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), -1, 0);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 0, -1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 1, 1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), -1, 1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), 1, -1);
	ThePhyEng->spawnBossBullet(ThePhyEng->GetBossX(), ThePhyEng->GetBossY(), -1, -1);
}

void physis::PlayerSpawnBullet(int dx, int dy)
{
	ThePhyEng->spawnPlayerBullet(ThePhyEng->GetPlayerX(), ThePhyEng->GetPlayerY(), dx, dy);
}

