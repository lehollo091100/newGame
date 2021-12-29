#include "Number3.h"

void Number3::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_NUMBER3)->Render(x, y);
}

void Number3::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void Number3::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
