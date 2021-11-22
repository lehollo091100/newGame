#include "Tail.h"

void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - WIDTH / 2;
	top = y - HEIGHT / 2;
	right = left + WIDTH;
	bottom = top + HEIGHT;
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Tail::Render()
{
	RenderBoundingBox();
}

void Tail::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TAIL_STATE_NOTHING: {
		break;
	}
	case TAIL_STATE_ATTACK: {
		break;
	}
	default:
		break;
	}
}

void Tail::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Tail::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
}
