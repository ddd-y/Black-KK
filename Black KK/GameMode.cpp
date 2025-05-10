#include "GameMode.h"
#include"MyBarrier.h"
#include"AINode.h"
#include"PhyEngS.cuh"
#include"Player.h"
#include"PlayerController.h"
#include<thread>
#include"Boss.h"
void GameMode::SetBarrier()
{
	TheEngine->SetBarrier(Tosy);
	MyPlayerController->SetBarrier(Tosy);
	TheNode->SetBarrier(Tosy);
}
void GameMode::GameIfOver()
{
	int PlayerHealthNow=ThePlayer->GetHealth();
	int BossHealthNow = TheNode->GetBossHealth();
	if(BossHealthNow==0)
	{
		GameContinue = false;
		IfWin = true;
		return;
	}
	if (PlayerHealthNow == 0)
	{
		GameContinue = false;
		IfWin = false;
	}
}
GameMode::GameMode()
{
	TheEngine = std::make_shared<PhyEngS>();
	auto Playerphysis = std::make_shared<physis>(TheEngine);
	ThePlayer = std::make_shared<Player>(Playerphysis);
	MyPlayerController = std::make_shared<PlayerController>();
	MyPlayerController->SetControllGoal(ThePlayer);
	auto Bossphysis = std::make_shared<physis>(TheEngine);
	auto TheBs = std::make_shared<Boss>(Bossphysis);
	TheNode = std::make_shared<AINode>(TheBs);
	TheEngine->Innitialization(TheBs, ThePlayer);
	Tosy = std::make_shared<MyBarrier>(5);
	SetBarrier();
}

void GameMode::GameStart()
{
	std::thread thread1([&] {
		while (true) {
			TheNode->Execute();
			GameIfOver();
			if (!GameContinue)
				break;
		}
		});
	std::thread thread2([&] {
		while (true) {
			TheEngine->UpDateBullet();
			GameIfOver();
			if (!GameContinue)
				break;
		}
		});
	std::thread thread3([&] {
		while (true) {
			MyPlayerController->GameIng();
			GameIfOver();
			if (!GameContinue)
				break;
		}
		});
	std::thread thread4([&] {
		while (true) {
			Sleep(8);
			Tosy->Wait();
			GameIfOver();
			if (!GameContinue)
				break;
		}
		});
	while (true)
	{
		Tosy->Wait();
		GameIfOver();
		if (!GameContinue)
			break;
	}
}
int main()
{
	auto ag = std::make_shared<GameMode>();
	ag->GameStart();
}
