#include "Debris.h"
#include "debug.h"
void Debris::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Debris::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	return;
}

void Debris::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case DEBRIS_STATE_NORMAL: {
		vx = 0;
		vy = 0;
		break;
	}
	case DEBRIS_STATE_MOVING:{
		vx = Vx * nx;
		vy = -Vy;
		break;
	}
	default:
		break;
	}
}

void Debris::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_DEBRIS)->Render(x, y);
	RenderBoundingBox();
}

void Debris::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != DEBRIS_STATE_NORMAL)
	{
		vy += AY * dt;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Debris::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - WIDTH / 2;
	t = y - WIDTH / 2;
	r = l + WIDTH;
	b = t + WIDTH;
}
