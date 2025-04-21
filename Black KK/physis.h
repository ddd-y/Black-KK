#pragma once
#include<memory>
#include<vector>
class physis
{
private:
	std::shared_ptr<std::vector<std::vector<int>>> TheGrid;
	int TheX;
	int TheY;
	int TheID;//ID用于给物理引擎
	bool IfPlayer;//是否是玩家
	bool IfVolid(int theNewx,int theNewy)
	{
		if (theNewx < 0 || theNewy < 0 || theNewx >= (*TheGrid)[0].size() || theNewy >= TheGrid->size())
			return false;
		return (*TheGrid)[theNewy][theNewx] == 0;
	}
public:
	int GetTheID()
	{
		return TheID;
	}
	bool GetIfPlayer()
	{
		return IfPlayer;
	}
	virtual ~physis() = default;
	physis() = delete;
	virtual void AfterCollision() = 0;
	void ChangeLocation(int TheNewX, int TheNewY);//闪现，用于Boss的技能
	void ChangeSlow(int dx, int dy);//一步步走,不判断碰撞，碰撞判断由引擎来实现
	bool IfHit(int dx, int dy);//判断是否碰撞，true表示碰撞，false表示没有碰撞
};

