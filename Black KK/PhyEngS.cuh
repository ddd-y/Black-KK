#pragma once
#include"TerrS.cuh"
#include"ScreenDraw.h"
#include<vector>
#include<memory>
#include<mutex>
#include"MyBarrier.h"
class Boss;
class Player;
class PhyEngS
{
	std::vector<int> PlayerHitBoss;//玩家是否击中Boss
	std::vector<int> BossHitPlayer;//Boss是否击中玩家
	std::vector<int> speedx;//子弹速度x
	std::vector<int> speedy;//子弹速度y
	std::vector<int> NowX;//子弹坐标x
	std::vector<int> NowY;//子弹坐标y
	std::vector<int> speedxPlayer;//玩家子弹速度x
	std::vector<int> speedyPlayer;//玩家子弹速度y
	std::vector<int> NowXPlayer;//玩家子弹坐标x
	std::vector<int> NowYPlayer;//玩家子弹坐标y
	std::vector<int> IfBossBulletValid;//子弹是否有效
	std::vector<int> IfPlayerBulletValid;//玩家子弹是否有效
	std::shared_ptr<TerrS> TheTerr;
	std::shared_ptr<Boss> abBoss;
	std::shared_ptr<Player> abPlayer;
	std::shared_ptr<MyBarrier> Tosy;
	int* D_PlayerHitBoss;//设备端的玩家是否击中Boss
	int* D_BossHitPlayer;//设备端的Boss是否击中玩家
	int* D_TheGrid;//设备端的地图
	int* D_speedx;//设备端的子弹速度x
	int* D_speedy;//设备端的子弹速度y
	int* D_NowX;//设备端的子弹坐标x
	int* D_NowY;//设备端的子弹坐标y
	int* D_IfBossBulletValid;//设备端的子弹是否有效
	int* D_IfPlayerBulletValid;//设备端的玩家子弹是否有效
	int* D_speedxPlayer;//设备端的玩家子弹速度x
	int* D_speedyPlayer;//设备端的玩家子弹速度y
	int* D_NowXPlayer;//设备端的玩家子弹坐标x
	int* D_NowYPlayer;//设备端的玩家子弹坐标y
	std::mutex PlayerBulletMutex;
	std::mutex BossBulletMutex;
	int currentBossIndex;
	int currentPlayerIndex;
	int maxBossIndex;
	int maxPlayerIndex;
	int PlayerX, PlayerY;
	int BossX, BossY;
	int BossAttack=0;
	int PlayerAttack = 0;
	cudaStream_t stream1, stream2, stream3;
	char BossChar ='b';
	char PlayerChar = 'p';
	void PrePrepare();
	void AfterCollision();
public:
	void SetBarrier(std::shared_ptr<MyBarrier> &abb) { Tosy = abb; }
	int GetPlayerX() { return PlayerX; }
	int GetPlayerY() { return PlayerY; }
	int GetBossX() { return BossX; }
	int GetBossY() { return BossY; }
	int GetBossAttack() { return BossAttack; }
	int GetPlayerAttack(){ return PlayerAttack;}
	void ChangeBossAttack(int thenew) { BossAttack = thenew; }
	void ChangePlayerAttack(int Thenew) { PlayerAttack = Thenew; }
	bool WantToChangeBossLocation(int aBossX, int aBossY)//false表示不可以移动
	{
		if (!TheTerr->IfCanMove(aBossX, aBossY))
			return false;
		if (aBossX == PlayerX && aBossY == PlayerY)
			return false;
		TheScreenDraw->Draw(BossX, BossY, ' ');
		BossX = aBossX;
		BossY = aBossY;
		return true;
	}
	void WantToChangePlayerLocation(int aPlayerX, int aPlayerY)
	{
		if (!TheTerr->IfCanMove(aPlayerX, aPlayerY))
			return;
		if (aPlayerX == BossX && aPlayerY == BossY)
			return;
		TheScreenDraw->Draw(PlayerX, PlayerY, ' ');
		PlayerX = aPlayerX;
		PlayerY = aPlayerY;
	}
	PhyEngS(int aPlayerX,int aPlayerY,int aBossX,int aBossY);
	void Innitialization(std::shared_ptr<Boss> aBoss, std::shared_ptr<Player> aPlayer);//不要忘记初始化这两个东西
	std::shared_ptr<MyScreenDraw> TheScreenDraw;
	~PhyEngS();
	PhyEngS() = delete;
	void spawnPlayerBullet(int x, int y, int Thespeedx, int Thespeedy)
	{
		std::lock_guard<std::mutex> abs(PlayerBulletMutex);
		if (IfPlayerBulletValid[currentPlayerIndex] == 1)
		{
			return;
		}
		speedxPlayer[currentPlayerIndex] = Thespeedx;
		speedyPlayer[currentPlayerIndex] = Thespeedy;
		NowXPlayer[currentPlayerIndex] = x;
		NowYPlayer[currentPlayerIndex] = y;
		IfPlayerBulletValid[currentPlayerIndex] = 1;
		currentPlayerIndex=(currentPlayerIndex+1)%maxPlayerIndex;
	}
	void spawnBossBullet(int x, int y, int Thespeedx, int Thespeedy)
	{
		std::lock_guard<std::mutex> abs(BossBulletMutex);
		if (IfBossBulletValid[currentBossIndex] == 1)
		{
			return;
		}
		speedx[currentBossIndex] = Thespeedx;
		speedy[currentBossIndex] = Thespeedy;
		NowX[currentBossIndex] = x;
		NowY[currentBossIndex] = y;
		IfBossBulletValid[currentBossIndex] = 1;
		currentBossIndex = (currentBossIndex + 1) % maxBossIndex;
	}
	void UpDateBullet();
	void Draw();
	std::shared_ptr<std::vector<std::vector<int>>> GetGrid() { return TheTerr->GetGrid(); }
	void UseRender()
	{
		TheScreenDraw->Render();
	}
	int GetPlayerBeHittedTime();
	int GetBossBeHittedTime();
};