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
	std::shared_ptr<Terr> TheTerr;
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