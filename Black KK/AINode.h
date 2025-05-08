#pragma once
#include<memory>
#include"FirstSession.h"
#include"SecondSession.h"
#include"Boss.h"
#include"MyBarrier.h"
class AINode
{
private:
	int CurrentFrame;//��ǰ֡��
	std::shared_ptr<Boss> ControllGoal;
	std::shared_ptr<FirstSession> TheFirstSession;//��һ�׶�
	std::shared_ptr<SecondSession> TheSecondSession;//�ڶ��׶�
	std::shared_ptr<MyBarrier> Tosy=nullptr;
	void exeleft();
	void exeright();
	void Trace();//׷��
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

