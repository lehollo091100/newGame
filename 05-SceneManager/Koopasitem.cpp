#include "Koopasitem.h"
#include "debug.h"
void Koopasitem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPAS_WIDTH / 2;
	top = y - KOOPAS_HEIGHT / 2;
	right = left + KOOPAS_WIDTH/2;
	bottom = top + KOOPAS_HEIGHT;
}

void Koopasitem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += AY * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Koopasitem::Render()
{
	//RenderBoundingBox();
}

void Koopasitem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
	{
		vx = nx * KOOPAS_VX;
		break;
	}
	case ITEM_STATE_STOP:
	{
		vx = 0;
		break;
	}
	default:
		break;
	}
}

void Koopasitem::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Koopasitem::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	if (e->ny != 0)
	{
		vy = 0;
	}

}
