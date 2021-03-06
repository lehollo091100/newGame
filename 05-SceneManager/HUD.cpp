#include "HUD.h"

void HUD::Update(DWORD dt)
{
	//DebugOut(L"stack:%d", Stack);
	if ((RANGE1 <abs( mario->vx)) && (abs(mario->vx) <= RANGE2))
	{
		Stack = 1;
	}
	if ((RANGE2 < abs(mario->vx))&& (abs(mario->vx) <= RANGE3))
	{
		Stack = 2;
	}
	if ((RANGE3 < abs(mario->vx)) &&(abs(mario->vx) <= RANGE4))
	{
		Stack = 3;
	}
	if ((RANGE4 < abs(mario->vx))&&(abs(mario->vx) <= RANGE5))
	{
		Stack = 4;
	}
	if ((RANGE5 < abs(mario->vx)) && (abs(mario->vx) <= RANGE6))
	{
		Stack = 5;
	}
	if (RANGE6 < abs(mario->vx))
	{
		Stack = 6;
	}
	if(RANGE1 >= abs(mario->vx))
	{
		Stack = 0;
	}
	/*if (count >= 1000)
	{
		time -= 1;
		count = 0;
	}
	else {
		count += dt;
	}
	if (Point == 0)
	{
		p = "0000000";
	}
	if (Point < 1000 && Point>0)
	{
		p = "0000" + to_string(Point);
	}
	else if (Point >= 1000)
	{
		p = "000" + to_string(Point);
	}
	else if (Point >= 10000)
	{
		p = "00" + to_string(Point);
	}*/
}

void HUD::Render()
{
	float a = CGame::GetInstance()->GetCamX() + CGame::GetInstance()->GetBackBufferWidth() / 2;
	float b = CGame::GetInstance()->GetCamY() + CGame::GetInstance()->GetBackBufferHeight() - HUD_HEIGHT / 2;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_HUD)->Render(a, b);
	stack->Render1(CGame::GetInstance()->GetCamX()+76,b-5, Stack);
}

HUD* HUD::__instance = NULL;
HUD* HUD::GetInstance()
{
	if (__instance == NULL) __instance = new HUD();
	return __instance;
}
