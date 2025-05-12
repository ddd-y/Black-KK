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
	std::vector<int> PlayerHitBoss;//����Ƿ����Boss
	std::vector<int> BossHitPlayer;//Boss�Ƿ�������
	std::vector<int> speedx;//�ӵ��ٶ�x
	std::vector<int> speedy;//�ӵ��ٶ�y
	std::vector<int> NowX;//�ӵ�����x
	std::vector<int> NowY;//�ӵ�����y
	std::vector<int> speedxPlayer;//����ӵ��ٶ�x
	std::vector<int> speedyPlayer;//����ӵ��ٶ�y
	std::vector<int> NowXPlayer;//����ӵ�����x
	std::vector<int> NowYPlayer;//����ӵ�����y
	std::vector<int> IfBossBulletValid;//�ӵ��Ƿ���Ч
	std::vector<int> IfPlayerBulletValid;//����ӵ��Ƿ���Ч
	std::shared_ptr<TerrS> TheTerr;
	std::shared_ptr<Boss> abBoss;
	std::shared_ptr<Player> abPlayer;
	std::shared_ptr<MyBarrier> Tosy;
	int* D_PlayerHitBoss;//�豸�˵�����Ƿ����Boss
	int* D_BossHitPlayer;//�豸�˵�Boss�Ƿ�������
	int* D_TheGrid;//�豸�˵ĵ�ͼ
	int* D_speedx;//�豸�˵��ӵ��ٶ�x
	int* D_speedy;//�豸�˵��ӵ��ٶ�y
	int* D_NowX;//�豸�˵��ӵ�����x
	int* D_NowY;//�豸�˵��ӵ�����y
	int* D_IfBossBulletValid;//�豸�˵��ӵ��Ƿ���Ч
	int* D_IfPlayerBulletValid;//�豸�˵�����ӵ��Ƿ���Ч
	int* D_speedxPlayer;//�豸�˵�����ӵ��ٶ�x
	int* D_speedyPlayer;//�豸�˵�����ӵ��ٶ�y
	int* D_NowXPlayer;//�豸�˵�����ӵ�����x
	int* D_NowYPlayer;//�豸�˵�����ӵ�����y
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
	bool WantToChangeBossLocation(int aBossX, int aBossY)//false��ʾ�������ƶ�
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
	void Innitialization(std::shared_ptr<Boss> aBoss, std::shared_ptr<Player> aPlayer);//��Ҫ���ǳ�ʼ������������
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