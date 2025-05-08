#pragma once
#include<memory>
#include"FirstSession.h"
#include"SecondSession.h"
#include"Boss.h"
#include"MyBarrier.h"
class AINode
{
private:
	int CurrentFrame;//当前帧数
	std::shared_ptr<Boss> ControllGoal;
	std::shared_ptr<FirstSession> TheFirstSession;//第一阶段
	std::shared_ptr<SecondSession> TheSecondSession;//第二阶段
	std::shared_ptr<MyBarrier> Tosy=nullptr;
	void exeleft();
	void exeright();
	void Trace();//追击
	void PlusFrame()
	{
		CurrentFrame = (CurrentFrame + 1) % 2048;
	}
public:
	AINode(std::shared_ptr<Boss> ControllGoal);
	void SetBarrier(std::shared_ptr<MyBarrier> abb);
	void Execute();
	~AINode() = default;
};

