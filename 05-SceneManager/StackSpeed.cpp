#include "StackSpeed.h"

void StackSpeed::Render1(float x, float y, int s)
{
	CAnimations* animations = CAnimations::GetInstance();
	for (int i = 0; i < s; i++)
	{
		animations->Get(861)->Render(x+i*11,y);
	}
}
