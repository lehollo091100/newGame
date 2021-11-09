#include "GreenPlant.h"

void GreenPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x -WIDTH / 2;
	top = y - HEIGHT / 2;
	right = left + WIDTH;
	bottom = top + HEIGHT;
}

void GreenPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GREENPLANT_STATE_START)
	{
		if (GetTickCount64()-time>= TIME)
		{
			SetState(GREENPLANT_STATE_MOVINGUP);
		}
	}
	else if (state == GREENPLANT_STATE_MOVINGUP) {
		if (initY-y>=RANGE)
		{
			SetState(GREENPLANT_STATE_STOP);
		}
	}
	else if (state == GREENPLANT_STATE_STOP)
	{
		if (GetTickCount64() - time >= TIME)
		{
			SetState(GREENPLANT_STATE_MOVINGDOWN);
		}
	}
	else if (state == GREENPLANT_STATE_MOVINGDOWN) {
		if (y >= initY) {
			vy = 0;
			y = initY;
			SetState(GREENPLANT_STATE_START);
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void GreenPlant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_NORMAL)->Render(x, y);
	RenderBoundingBox();
}

void GreenPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREENPLANT_STATE_START: {
		time = GetTickCount64();
		vy = 0;
		break;
	}
	case GREENPLANT_STATE_MOVINGUP: {
		time = 0;
		vy = -VY;
		break;
	}
	case GREENPLANT_STATE_STOP: {
		time = GetTickCount64();
		vy = 0;
		break;
	}
	case GREENPLANT_STATE_MOVINGDOWN: {
		time - 0;
		vy = VY;
		break;
	}
	default:
		break;
	}
}

void GreenPlant::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void GreenPlant::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{

}
