#include "Redgoomba.h"

void Redgoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - REDGOOMBA_WIDTH / 2;
	top = y - READGOOMBA_HEIGHT / 2;
	right = left + REDGOOMBA_WIDTH;
	bottom = top + READGOOMBA_HEIGHT;
	if (state == REDGOOMBA_STATE_DIE)
	{
		bottom = top + READGOOMBA_HEIGHT / 2;
	}
}

void Redgoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (x < 5)
	{
		nx = -nx;
		vx = nx * vx;
	}
	vy += AY * dt;
	if (state == REDGOOMBA_STATE_WALKING) {
		if (time == 0)
		{
			time = DWORD(GetTickCount64());
		}
		else if (GetTickCount64() - time > 3000)
		{
			time = 0;
			SetState(REDGOOMBA_STATE_JUMPING);
		}
	}
	if (state == REDGOOMBA_STATE_JUMPING) {
		if (time == 0)
		{
			time = DWORD(GetTickCount64());
			vy = -REDGOOMBA_JUMPVY;
		}
		else if (GetTickCount64() - time > 1500)
		{
			time = 0;
			SetState(REDGOOMBA_STATE_FLYING);
		}
	}
	if (state == REDGOOMBA_STATE_FLYING) {
		if (time == 0)
		{
			vy = -REDGOOMBA_FLYVY;
			time = DWORD(GetTickCount64());
		}
		else if (GetTickCount64() - time > 1000)
		{
			time = 0;
			SetState(REDGOOMBA_STATE_WALKING);
		}
	}
	if (state == REDGOOMBA_STATE_NORMAL) {
		time = 0;
	}
	if (state == REDGOOMBA_STATE_DIE)
	{
		if (GetTickCount64()-time>1000)
		{
			this->Delete();
		}
	}
	if (state == REDGOOMBA_STATE_DIEUP) {
		if (y - initY >= RANGE_TO_DIE) {
			Delete();
		}
	}
	CCollision::GetInstance()->Process(this,dt,coObjects);
}

void Redgoomba::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state==REDGOOMBA_STATE_WALKING)
	{
		animations->Get(ID_ANI_WALKINGWING)->Render(x, y);
		//animations->Get(ID_ANI_FLYING)->Render(x, y);
	}
	else if (state == REDGOOMBA_STATE_JUMPING)
	{
		animations->Get(ID_ANI_JUMPING)->Render(x, y);
	}
	else if (state == REDGOOMBA_STATE_FLYING)
	{
		animations->Get(ID_ANI_FLYING)->Render(x, y);
	}
	else if(state==REDGOOMBA_STATE_NORMAL)
	{
		animations->Get(ID_ANI_WALKING)->Render(x, y);

	}
	else if (state==REDGOOMBA_STATE_DIE)
	{
		animations->Get(ID_ANI_DIE)->Render(x, y);

	}
	else if (state == REDGOOMBA_STATE_DIEUP)
	{
		animations->Get(ID_ANI_DIE_UP)->Render(x, y);

	}
	//RenderBoundingBox();
}

void Redgoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case REDGOOMBA_STATE_WALKING:
	{
		vx = nx * VX;
		break;
	}
	case REDGOOMBA_STATE_NORMAL:{
		vx = nx * VX;
		break;
	}
	case REDGOOMBA_STATE_DIE: {
		time = DWORD(GetTickCount64());
		vx = 0;
		vy = 0;
		break;
	}
	case REDGOOMBA_STATE_DIEUP: {
		vy = -REDGOOMBA_FLYVY;
		initY = y;
		break;
	}
	default:
		break;
	}
}

void Redgoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Redgoomba::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	if (dynamic_cast<Mushroom*>(e->obj)) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (dynamic_cast<Koopas*>(e->obj)) return;
	if (dynamic_cast<Koopasitem*>(e->obj)) return;
	if (e->ny != 0)
	{
		if (state==REDGOOMBA_STATE_JUMPING)
		{
			vy = -REDGOOMBA_JUMPVY;
		}
		else if(state==REDGOOMBA_STATE_FLYING)
		{
			vy = -REDGOOMBA_FLYVY;
		}
		else
		{
			vy = 0;
		}
	}
	else if (e->nx != 0)
	{
		nx = -nx;
		vx = nx * vx;
	}
}
