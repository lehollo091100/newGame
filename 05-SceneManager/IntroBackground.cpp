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
		vx = 0; vy = 0;
		break;
	}
	case INTROBACKGROUND_STATE_MOVING: {
		vy = INTROBACKGROUND_VY;
		break;
	}
	case INTROBACKGROUND_STATE_CHANGE: {
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

		animations->Get(ID_ANI_INTROBACKGROUND_NORMAL)->Render(x, y);
	}
	else
	{
		animations->Get(ID_ANI_CHANGE)->Render(x, y);

	}
}

void IntroBackGround::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	//DebugOut(L"intro bg y:%f\n", vy);
	if (state == INTROBACKGROUND_STATE_MOVING) {
		if (vy != 0)
		{
			if (y>=  CGame::GetInstance()->GetBackBufferHeight()/2.3)
			{
				timechange =DWORD(GetTickCount64());
				vy = 0;
			}
		}
		if (vy == 0)
		{
			if (y  >=CGame::GetInstance()->GetBackBufferHeight()/2.3)
			{
				if (GetTickCount64() - timechange >= TIMECHANGE)
				{
					SetState(INTROBACKGROUND_STATE_CHANGE);
				}
			}
		}
	}
}

void IntroBackGround::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
