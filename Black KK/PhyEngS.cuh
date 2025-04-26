#include"physis.h"
#include"BulletPhy.h"
#include"Terr.h"
#include"ScreenDraw.h"
#include<vector>
#include<memory>
class PhyEngS
{
	std::vector<std::shared_ptr<BulletPhy>> BossBullet;
	std::vector<std::shared_ptr<BulletPhy>> PlayerBullet;
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
public:
	PhyEngS(std::shared_ptr<Terr> TheTerr);
	std::shared_ptr<ScreenDraw> TheScreenDraw;
	~PhyEngS();
	PhyEngS() = delete;
	bool GetIfBossBulletValid(int index)
	{
		return IfBossBulletValid[index];
	}
	void spawnBossBullet(int x, int y, int Thespeedx, int Thespeedy)
	{
		BossBullet[currentBossIndex] = std::make_shared<BulletPhy>(currentBossIndex);
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
	void spawnPlayerBullet(int x, int y, int Thespeedx, int Thespeedy)
	{
		PlayerBullet[currentPlayerIndex] = std::make_shared<BulletPhy>(currentPlayerIndex);
		speedx[currentPlayerIndex] = Thespeedx;
		speedy[currentPlayerIndex] = Thespeedy;
		NowX[currentPlayerIndex] = x;
		NowY[currentPlayerIndex] = y;
		IfPlayerBulletValid[currentPlayerIndex] = 1;
		++currentPlayerIndex;
	}
	void UpDateBullet();
};