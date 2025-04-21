#pragma once
#include<memory>
class physis
{
private:
	int TheX;
	int TheY;
	int TheID;//ID用于给物理引擎
public:
	void ChangeLocation(int TheNewX,int TheNewY);
	int GetTheID()
	{
		return TheID;
	}
	virtual ~physis() = default;
	physis() : TheX(0), TheY(0), TheID(0) {}
	virtual void AfterCollision() = 0;
};

