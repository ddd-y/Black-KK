#include "FirstSession.h"
#include "Boss.h"
FirstSession::FirstSession()
{
}

FirstSession::~FirstSession()
{
}

void FirstSession::exenew()
{
	if(ControllGoal->IfDistance())
	ControllGoal->Skillfirst();
}
