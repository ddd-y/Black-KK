#pragma once
#include"Terr.h"
#include"ScreenDraw.h"
#include<vector>
#include<memory>
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
	std::shared_ptr<Terr> TheTerr;
	std::shared_ptr<Boss> abBoss;
	std::shared_ptr<Player> abPlayer;
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
	int currentBossIndex;
	int currentPlayerIndex;
	int maxBossIndex;
	int maxPlayerIndex;
	int PlayerX, PlayerY;
	int BossX, BossY;
	int BossAttack=0;
	int PlayerAttack = 0;
	char BossChar ='b';
	char PlayerChar = 'p';
	void PrePrepare();
	void AfterCollision();
public:
	int GetPlayerX() { return PlayerX; }
	int GetPlayerY() { return PlayerY; }
	int GetBossX() { return BossX; }
	int GetBossY() { return BossY; }
	int GetBossAttack() { return BossAttack; }
	int GetPlayerAttack(){ return PlayerAttack;}
	void ChangeBossAttack(int thenew) { BossAttack = thenew; }
	void ChangePlayerAttack(int Thenew) { PlayerAttack = Thenew; }
	bool WantToChangeBossLocation(int aBossX, int aBossY)//false��ʾ�������ƶ�
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
	PhyEngS(std::shared_ptr<Terr> TheTerr,int aPlayerX,int aPlayerY,int aBossX,int aBossY);
	void Innitialization(std::shared_ptr<Boss> aBoss, std::shared_ptr<Player> aPlayer);//��Ҫ���ǳ�ʼ������������
	std::shared_ptr<ScreenDraw> TheScreenDraw;
	~PhyEngS();
	PhyEngS() = delete;
	void spawnPlayerBullet(int x, int y, int Thespeedx, int Thespeedy)
	{
		if (IfPlayerBulletValid[currentPlayerIndex] == 1)
		{
			return;
		}
		speedxPlayer[currentPlayerIndex] = Thespeedx;
		speedyPlayer[currentPlayerIndex] = Thespeedy;
		NowXPlayer[currentPlayerIndex] = x;
		NowYPlayer[currentPlayerIndex] = y;
		IfPlayerBulletValid[currentPlayerIndex] = 1;
		++currentPlayerIndex;
		if (currentPlayerIndex >= maxPlayerIndex)
		{
			currentPlayerIndex = 0;
		}
	}
	void spawnBossBullet(int x, int y, int Thespeedx, int Thespeedy)
	{
		if (IfBossBulletValid[currentBossIndex] == 1)
		{
			return;
		}
		speedx[currentBossIndex] = Thespeedx;
		speedy[currentBossIndex] = Thespeedy;
		NowX[currentBossIndex] = x;
		NowY[currentBossIndex] = y;
		IfBossBulletValid[currentBossIndex] = 1;
		++currentBossIndex;
		if (currentBossIndex >= maxBossIndex)
		{
			currentBossIndex = 0;
		}
	}
	void UpDateBullet();
	void ReSetBullet()
	{
		for(int i=0;i<maxBossIndex;++i)
		{
			if (IfBossBulletValid[i] == 1)
			{
				TheScreenDraw->Draw(NowX[i], NowY[i], ' ');
				IfBossBulletValid[i] = 0;
			}
			BossHitPlayer[i] = 0;
		}
		for (int i = 0; i < maxPlayerIndex; ++i)
		{
			if (IfPlayerBulletValid[i] == 1)
			{
				TheScreenDraw->Draw(NowXPlayer[i], NowYPlayer[i], ' ');
				IfPlayerBulletValid[i] = 0;
			}
			PlayerHitBoss[i] = 0;
		}
		currentBossIndex = 0;
		currentPlayerIndex = 0;
	}
	void Draw();
	std::shared_ptr<std::vector<std::vector<int>>> GetGrid() { return TheTerr->GetGrid(); }
	int GetPlayerBeHittedTime();
	int GetBossBeHittedTime();
};