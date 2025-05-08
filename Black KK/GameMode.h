#pragma once
#include<memory>
class PhyEngS;
class AINode;
class Player;
class MyBarrier;
class GameMode
{
private:
	std::shared_ptr<PhyEngS> TheEngine;
	std::shared_ptr<AINode> TheNode;
	std::shared_ptr<Player> ThePlayer;
	std::shared_ptr<MyBarrier> Tosy;
	void SetBarrier();
public:
	GameMode();
	~GameMode(){}
	void GameStart();
};

