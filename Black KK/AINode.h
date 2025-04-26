#pragma once
#include<memory>
class Boss;
class FirstSession;
class SecondSession;
class AINode
{
private:
	std::shared_ptr<Boss> ControllGoal;
	std::shared_ptr<FirstSession> TheFirstSession;//第一阶段
	std::shared_ptr<SecondSession> TheSecondSession;//第二阶段
	void exeleft();
	void exeright();
	void Trace();//追击
public:
	AINode(std::shared_ptr<Boss> ControllGoal);
	void Execute();
};

