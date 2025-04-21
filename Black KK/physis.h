#pragma once
#include<memory>
#include<vector>
class physis
{
private:
	std::shared_ptr<std::vector<std::vector<int>>> TheGrid;
	int TheX;
	int TheY;
	int TheID;//ID���ڸ���������
	bool IfPlayer;//�Ƿ������
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
	void ChangeLocation(int TheNewX, int TheNewY);//���֣�����Boss�ļ���
	void ChangeSlow(int dx, int dy);//һ������,���ж���ײ����ײ�ж���������ʵ��
	bool IfHit(int dx, int dy);//�ж��Ƿ���ײ��true��ʾ��ײ��false��ʾû����ײ
};

