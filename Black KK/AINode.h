#pragma once
#include<memory>
class Boss;
class FirstSession;
class SecondSession;
class AINode
{
private:
	int CurrentFrame;//��ǰ֡��
	std::shared_ptr<Boss> ControllGoal;
	std::shared_ptr<FirstSession> TheFirstSession;//��һ�׶�
	std::shared_ptr<SecondSession> TheSecondSession;//�ڶ��׶�
	void exeleft();
	void exeright();
	void Trace();//׷��
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

