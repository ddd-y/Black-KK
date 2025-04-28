#include "AINode.h"
#include"FirstSession.h"
#include"SecondSession.h"
#include"Boss.h"
void AINode::exeleft()
{
	TheFirstSession->exenew();
}

void AINode::exeright()
{
	TheSecondSession->exenew();
}

void AINode::Trace()
{
	if(ControllGoal->IfNeedToFlash())
	{
		ControllGoal->Flash();
		return;
	}
	ControllGoal->GetNextStep();
}

AINode::AINode(std::shared_ptr<Boss> TheControllGoal) :ControllGoal(TheControllGoal),CurrentFrame(0)
{
	TheFirstSession = std::make_shared<FirstSession>(TheControllGoal);
	TheSecondSession = std::make_shared<SecondSession>(TheControllGoal);
}

void AINode::Execute()
{
	if (CurrentFrame % ControllGoal->GetSpeednow() == 0)
		Trace();
	if (CurrentFrame % ControllGoal->GetAttackPing() == 1)
	{
		if (ControllGoal->IfSecond())
			exeright();
		else
			exeleft();
	}
	PlusFrame();
}
