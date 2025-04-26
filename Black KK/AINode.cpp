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
}

AINode::AINode(std::shared_ptr<Boss> TheControllGoal) :ControllGoal(TheControllGoal)
{
	TheFirstSession = std::make_shared<FirstSession>(TheControllGoal);
	TheSecondSession = std::make_shared<SecondSession>(TheControllGoal);
}

void AINode::Execute()
{
	Trace();
	if (ControllGoal->IfSecond())
		exeright();
	else
		exeleft();
}
