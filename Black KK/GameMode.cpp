#include "GameMode.h"
#include"MyBarrier.h"
#include"AINode.h"
#include"PhyEngS.cuh"
#include"Player.h"
#include<thread>
#include"Boss.h"
void GameMode::SetBarrier()
{
	TheEngine->SetBarrier(Tosy);
	ThePlayer->SetBarrier(Tosy);
	TheNode->SetBarrier(Tosy);
}
GameMode::GameMode()
{
	TheEngine = std::make_shared<PhyEngS>(0,40,0,15);
	auto Playerphysis = std::make_shared<physis>(TheEngine);
	ThePlayer = std::make_shared<Player>(Playerphysis);
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
		}
		});
	std::thread thread2([&] {
		while (true) {
			TheEngine->UpDateBullet();
		}
		});
	std::thread thread3([&] {
		while (true) {
			ThePlayer->DealWord();
		}
		});
	std::thread thread4([&] {
		while (true) {
			Sleep(40);
			Tosy->Wait();
		}
		});
	while (true)
	{
		Tosy->Wait();
	}
}
int main()
{
	auto ag = std::make_shared<GameMode>();
	ag->GameStart();
}
