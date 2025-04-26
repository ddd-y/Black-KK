#pragma once
#include <memory>
class Boss;
class SecondSession
{
private:
	std::shared_ptr<Boss> ControllGoal;
public:
	SecondSession();
	SecondSession(std::shared_ptr<Boss> TheControllGoal) :
		ControllGoal(TheControllGoal)
	{
	}
	~SecondSession();
	void exenew();
};

