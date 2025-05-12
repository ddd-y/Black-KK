#pragma once
#include<memory>
#include<atomic>
class PhyEngS;
class AINode;
class Player;
class MyBarrier;
class PlayerController;
class GameMode
{
private:
	std::shared_ptr<PhyEngS> TheEngine;
	std::shared_ptr<AINode> TheNode;
	std::shared_ptr<Player> ThePlayer;
	std::shared_ptr<PlayerController> MyPlayerController;
	std::shared_ptr<MyBarrier> Tosy;
	std::atomic<bool> GameContinue{ true };
	bool IfWin = false;//true表示赢了，flase表示输了
	void SetBarrier();
	void GameIfOver();
	void GameOver();
public:
	GameMode();
	~GameMode(){}
	void GameStart();
};

