#include "GameMode.h"
#include"MyBarrier.h"
#include"AINode.h"
#include"PhyEngS.cuh"
#include"Player.h"
#include"PlayerController.h"
#include<thread>
#include"Boss.h"
#include<iostream>
void GameMode::SetBarrier()
{
	TheEngine->SetBarrier(Tosy);
	MyPlayerController->SetBarrier(Tosy);
	TheNode->SetBarrier(Tosy);
}
void GameMode::GameIfOver()
{
	int PlayerHealthNow = ThePlayer->GetHealth();
	int BossHealthNow = TheNode->GetBossHealth();
	if (BossHealthNow == 0 || PlayerHealthNow == 0) {
		GameContinue.store(false);       
		Tosy->ReleaseAll();             
		IfWin = (BossHealthNow == 0);    
	}
}
void GameMode::GameOver()
{
	if (IfWin)
	{
		std::wstring ssr = L"恭喜你获得胜利，作者微信号：cayyyds";
		TheEngine->GameOver(ssr);
	}
	else
	{
		std::wstring ssr = L"真可惜，再试试吧，作者微信号：cayyyds";
		TheEngine->GameOver(ssr);
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
	TheEngine->GmaeRuleShow();
	std::thread thread1([&] {
		while (GameContinue.load()) {
			TheNode->Execute();
		}
		Tosy->Wait();
		});
	std::thread thread2([&] {
		while (GameContinue.load()) {
			TheEngine->UpDateBullet();
		}
		Tosy->Wait();
		});
	std::thread thread3([&] {
		while (GameContinue.load()) {
			MyPlayerController->GameIng();
		}
		Tosy->Wait();
		});
	std::thread thread4([&] {
		while (GameContinue.load()) {
			Sleep(7);
			Tosy->Wait();
		}
		Tosy->Wait();
		});
	while (GameContinue.load())
	{
		GameIfOver();
		Tosy->Wait();
	}
	GameOver();
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
}
int main()
{
	auto ag = std::make_shared<GameMode>();
	ag->GameStart();
}
