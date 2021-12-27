#include "Curtain.h"

void Curtain::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_CURTAIN)->Render(x, y);
}

void Curtain::Update(DWORD dt)
{
	
}

void Curtain::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}
