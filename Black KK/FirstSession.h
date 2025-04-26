#pragma once
#include <memory>
class Boss;
class FirstSession
{
private:
	std::shared_ptr<Boss> ControllGoal;
public:
	FirstSession();
	FirstSession(std::shared_ptr<Boss> TheControllGoal) :
		ControllGoal(TheControllGoal)
	{
	}
	~FirstSession();
	void exenew();
};

