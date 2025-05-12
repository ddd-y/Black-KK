#pragma once
#include"TerrS.cuh"
#include"ScreenDraw.h"
#include<vector>
#include<array>
#include<memory>
#include<mutex>
#include<queue>
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
	std::mutex BossBulletMu;
	std::mutex PlayerBulletMu;
	std::mutex BossMessage;
	std::mutex PlayerMessage;
	int currentBossIndex;
	int currentPlayerIndex;
	const int maxBossIndex;
	const int maxPlayerIndex;
	int PlayerX, PlayerY;
	int BossX, BossY;
	int BossAttack=0;
	int PlayerAttack = 0;
	int PlayerAttackCount = 0;
	int DealPlayerCount = 0;
	cudaStream_t stream1, stream2, stream3;
	char BossChar ='b';
	int BossColor = 0x7;
	char PlayerChar = 'p';
	int PlayerColor = 0x7;
	std::queue<std::array<int,2>> Bossqueue;
	std::queue<std::array<int, 2>> Playerqueue;
	std::queue<std::array<int, 4>> BossBulletqueue;
	std::queue<std::array<int, 4>> PlayerBulletqueue;
	void PrePrepare();
	void AfterCollision();
	void DealBossqueue();
	void DealPlayerqueue();
	void DealBossBullet();
	void DealPlayerBullet();
public:
	void DrawTerrS();
	void GmaeRuleShow();
	void GameOver(std::wstring& overstring);
	void SetBossCharAndColor(char cc,int bbs)
	{
		BossChar = cc;
		BossColor = bbs;
	}
	void SetPlayerCharAndColor(char cc, int bbs)
	{
		PlayerChar = cc;
		PlayerColor = bbs;
	}
	void SetBarrier(std::shared_ptr<MyBarrier> &abb) { Tosy = abb; }
	int GetPlayerX() 
	{
		std::lock_guard<std::mutex> abs(PlayerMessage);
		return PlayerX; 
	}
	int GetPlayerY() 
	{
		std::lock_guard<std::mutex> abs(PlayerMessage);
		return PlayerY; 
	}
	int GetBossX() 
	{
		return BossX; 
	}
	int GetBossY() 
	{ 
		return BossY; 
	}
	int GetBossAttack() { return BossAttack; }
	int GetPlayerAttack(){ return PlayerAttack;}
	void ChangeBossAttack(int thenew) { BossAttack = thenew; }
	void ChangePlayerAttack(int Thenew) { PlayerAttack = Thenew; }
	std::mutex* GetBossBulletMu() { return &BossBulletMu; }
	std::mutex* GetPlayerBulletMu() { return &PlayerBulletMu; }
	bool WantToChangeBossLocation(int aBossX, int aBossY)//false表示不可以移动
	{
		if (!TheTerr->IfCanMove(aBossX, aBossY))
			return false;
		std::lock_guard<std::mutex> Themu(BossMessage);
		Bossqueue.push({ aBossX,aBossY });
		return true;
	}
	void WantToChangePlayerLocation(int aPlayerX, int aPlayerY)
	{
		if (!TheTerr->IfCanMove(aPlayerX, aPlayerY))
			return;
		std::lock_guard<std::mutex> Themu(PlayerMessage);
		if (Playerqueue.size() > 3)
			return;
		Playerqueue.push({ aPlayerX,aPlayerY });
	}
	PhyEngS();
	void Innitialization(std::shared_ptr<Boss> aBoss, std::shared_ptr<Player> aPlayer);//不要忘记初始化这两个东西
	std::shared_ptr<MyScreenDraw> TheScreenDraw;
	~PhyEngS();
	void spawnPlayerBullet(int x, int y, int Thespeedx, int Thespeedy)
	{
		PlayerBulletqueue.push({ x,y,Thespeedx,Thespeedy });
	}
	void spawnBossBullet(int x, int y, int Thespeedx, int Thespeedy)
	{
		BossBulletqueue.push({ x,y,Thespeedx,Thespeedy });
	}
	void UpDateBullet();
	void Draw();
	std::shared_ptr<std::vector<std::vector<int>>> GetGrid() { return TheTerr->GetGrid(); }
	int GetPlayerBeHittedTime();
	int GetBossBeHittedTime();
};