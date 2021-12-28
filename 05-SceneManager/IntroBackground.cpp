#include "IntroBackground.h"

void IntroBackGround::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void IntroBackGround::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
}

void IntroBackGround::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case INTROBACKGROUND_STATE_NORMAL: {
		break;
	}
	case INTROBACKGROUND_STATE_MOVING: {
		break;
	}
	default:
		break;
	}
}

void IntroBackGround::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state != INTROBACKGROUND_STATE_CHANGE)
	{

		animations->Get(ID_ANI_CURTAIN)->Render(x, y);
	}
	else
	{

	}
}

void IntroBackGround::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void IntroBackGround::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
