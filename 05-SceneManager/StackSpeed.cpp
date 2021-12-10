#include "StackSpeed.h"

void StackSpeed::Render1(float x, float y, int s)
{
	CAnimations* animations = CAnimations::GetInstance();
	for (int i = 0; i < s; i++)
	{
		if (s != 1) {
			animations->Get(ID_ANI_ARROW)->Render(x+i*11,y);
		}
	}
}
