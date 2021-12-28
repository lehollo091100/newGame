#include "IntroOption.h"

void IntroOption::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void IntroOption::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
}

void IntroOption::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case INTROOPTION_STATE_UP: {
		break;
	}
	case INTROOPTION_STATE_DOWN: {
		break;
	}
	default:
		break;
	}
}

void IntroOption::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state==INTROOPTION_STATE_UP)
	{
		animations->Get(ID_ANI_INTROOPTION_UP)->Render(x, y);

	}
	if (state == INTROOPTION_STATE_DOWN)
	{
		animations->Get(ID_ANI_INTROOPTION_DOWN)->Render(x, y);

	}
}

void IntroOption::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void IntroOption::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
IntroOption* IntroOption::__instance = NULL;
IntroOption* IntroOption::GetInstance()
{
	if (__instance == NULL) __instance = new IntroOption(0, 0);
	return __instance;
}
