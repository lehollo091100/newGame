#include "HUD.h"

void HUD::Update(DWORD dt)
{
	if (count >= 1000)
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
	}
}

void HUD::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_HUD)->Render(CGame::GetInstance()->GetCamX()+ CGame::GetInstance()->GetBackBufferWidth()/2, CGame::GetInstance()->GetCamY() +CGame::GetInstance()->GetBackBufferHeight()- HUD_HEIGHT/2);
}

HUD* HUD::__instance = NULL;
HUD* HUD::GetInstance()
{
	if (__instance == NULL) __instance = new HUD();
	return __instance;
}
