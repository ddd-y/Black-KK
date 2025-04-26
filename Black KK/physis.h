#pragma once
#include<memory>
#include<vector>
class physis
{
protected:
	int TheID;//ID用于给物理引擎
public:
	int GetTheID()
	{
		return TheID;
	}
	virtual ~physis() = default;
	physis() = delete;
	physis(int TheId) :
		TheID(TheId)
	{
	}
	virtual void AfterCollision();
	void ChangeLocation(int TheNewX, int TheNewY);//闪现，用于Boss的技能
	void ChangeSlow(int dx, int dy);//一步步走,不判断碰撞，碰撞判断由引擎来实现
	bool IfHit(int dx, int dy);//判断是否碰撞，true表示碰撞，false表示没有碰撞
};

