#pragma once
#include<memory>
#include<vector>
class physis
{
protected:
	int TheID;//ID���ڸ���������
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
	void ChangeLocation(int TheNewX, int TheNewY);//���֣�����Boss�ļ���
	void ChangeSlow(int dx, int dy);//һ������,���ж���ײ����ײ�ж���������ʵ��
	bool IfHit(int dx, int dy);//�ж��Ƿ���ײ��true��ʾ��ײ��false��ʾû����ײ
};

