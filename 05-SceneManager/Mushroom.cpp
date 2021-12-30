#include "Mushroom.h"
#include "debug.h"
#include "Pipe.h"
void Mushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Mushroom::OnCollisionWith(LPCOLLISIONEVENT e,DWORD dt)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<Mushroom*>(e->obj)) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		nx = -nx;
		vx = nx * vx;
	}
	//if (dynamic_cast<Pipe*>(e->obj))
	//{
	//	nx = -nx;
	//}
}

void Mushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_INVISIBLE:
		break;
	case MUSHROOM_STATE_UP:
	{
		break;
	}
	default:
		break;
	}
}

void Mushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state != MUSHROOM_STATE_INVISIBLE)
	{
		animations->Get(ID_ANI_MUSHROOM)->Render(x, y);
	}
	//RenderBoundingBox();
}

void Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (state!=MUSHROOM_STATE_INVISIBLE)
	{
		vy += MUSHROOM_AX * dt;
	}
	if (state==MUSHROOM_STATE_UP)
	{
		vy = -MUSHROOM_VY;
		if (y <= initY-MUSHROOM_RANGE)
		{
			vy = 0;
			SetState(MUSHROOM_STATE_MOVING);
		}
	}
	if (state == MUSHROOM_STATE_MOVING)
	{
		vx = nx*MUSHROOM_VX;
		//vy = VY;
	}
	if (x <= 8)
	{
		nx = -nx;
		vx = nx * MUSHROOM_VX;
	}
	//DebugOut(L"mushroom mario:%f\n", mario->x);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Mushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - 10 / 2;
	t = y - 16 / 2;
	r = l + 10;
	b = t + 16;
}
