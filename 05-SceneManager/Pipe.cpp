#include "Pipe.h"

void Pipe::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int idAni = 0;
	if (ani == 1)
	{
		idAni = ID_ANI_1;
	}
	else if (ani == 2)
	{
		idAni = ID_ANI_2;
	}
	else if (ani == 3)
	{
		idAni = ID_ANI_3;
	}
	else if (ani == 4)
	{
		idAni = ID_ANI_4;
	}
	else if (ani == 5)
	{
		idAni = ID_ANI_5;
	}
	else {
		return;
	}
	animations->Get(idAni)->Render(x, y);
	//RenderBoundingBox();
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y-height/2;
	r = l + width;
	b = t + height;

}
