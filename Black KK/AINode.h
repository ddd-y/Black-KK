#pragma once
#include<memory>
class Boss;
class FirstSession;
class SecondSession;
class AINode
{
private:
	int CurrentFrame;//当前帧数
	std::shared_ptr<Boss> ControllGoal;
	std::shared_ptr<FirstSession> TheFirstSession;//第一阶段
	std::shared_ptr<SecondSession> TheSecondSession;//第二阶段
	void exeleft();
	void exeright();
	void Trace();//追击
	void PlusFrame()
	{
		++CurrentFrame;
		if (CurrentFrame >= 2048)
			CurrentFrame = 0;
	}
public:
	AINode(std::shared_ptr<Boss> ControllGoal);
	void Execute();
	~AINode() = default;
};

