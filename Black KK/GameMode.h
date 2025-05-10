#pragma once
#include<memory>
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
	bool GameContinue = true;//��Ϸ�Ƿ����
	bool IfWin = false;//true��ʾӮ�ˣ�flase��ʾ����
	void SetBarrier();
	void GameIfOver();
public:
	GameMode();
	~GameMode(){}
	void GameStart();
};

