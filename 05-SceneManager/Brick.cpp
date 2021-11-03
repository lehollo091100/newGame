#include "Brick.h"
#include "debug.h"
#include "Utils.h"
void CBrick::Render()
{
	//CAnimations* animations = CAnimations::GetInstance();
	//animations->Get(ID_ANI_BRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - width/2;
	t = y - height/2;
	r = l + width;
	b = t + height;
}