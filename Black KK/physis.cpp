#include "physis.h"
void physis::ChangeLocation(int TheNewX, int TheNewY)
{
	TheX = TheNewX;
	TheY = TheNewY;
}

void physis::ChangeSlow(int dx, int dy)
{
	TheX += dx;
	TheY += dy;
}

bool physis::IfHit(int dx, int dy)
{
	int thex = TheX + dx;
	int they = TheY + dy;
	if (thex < 0 || they < 0 || thex >= (*TheGrid)[0].size() || they >= TheGrid->size())
		return true;
	return (*TheGrid)[they][thex] == 1;
}
