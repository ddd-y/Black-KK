#pragma once
#include<memory>
class Boss;
class FirstSession;
class SecondSession;
class AINode
{
private:
	std::shared_ptr<Boss> ControllGoal;
	std::shared_ptr<FirstSession> TheFirstSession;//��һ�׶�
	std::shared_ptr<SecondSession> TheSecondSession;//�ڶ��׶�
	void exeleft();
	void exeright();
	void Trace();//׷��
public:
	AINode(std::shared_ptr<Boss> ControllGoal);
	void Execute();
};

