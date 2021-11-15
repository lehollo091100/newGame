#include "FireRedPlant.h"
#include "debug.h"
void FireRedPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - WIDTH / 2;
	top = y - HEIGHT / 2;
	right = left + WIDTH;
	bottom = top + HEIGHT;
}

void FireRedPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == FIREREDPLANT_STATE_START)
	{
		if (GetTickCount64() - time >= TIME)
		{
			SetState(FIREREDPLANT_STATE_MOVINGUP);
		}
	}
	else if (state == FIREREDPLANT_STATE_MOVINGUP) {
		if (initY - y >= RANGE)
		{
			SetState(FIREREDPLANT_STATE_STOP);
		}
	}
	else if (state == FIREREDPLANT_STATE_STOP)
	{
		if (GetTickCount64() - time >= TIME)
		{
			SetState(FIREREDPLANT_STATE_MOVINGDOWN);
		}
	}
	else if (state == FIREREDPLANT_STATE_MOVINGDOWN) {
		if (y >= initY) {
			vy = 0;
			y = initY;
			SetState(FIREREDPLANT_STATE_START);
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void FireRedPlant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int ani=0;
	if (state == FIREREDPLANT_STATE_START || state==FIREREDPLANT_STATE_STOP)
	{
		if (mario->x > x)
		{
			if (mario->y < y)
			{
				ani = ID_ANI_OPENUPRIGHT;
			}
			else
			{
				ani = ID_ANI_OPENDOWNRIGHT;
			}
		}
		else {
			if (mario->y < y)
			{
				ani = ID_ANI_OPENUPLEFT;
			}
			else
			{
				ani = ID_ANI_OPENDOWNLEFT;
			}
		}
	}
	else if (state == FIREREDPLANT_STATE_MOVINGUP || state == FIREREDPLANT_STATE_MOVINGDOWN)
	{
		if (mario->x>x)
		{
			if (mario->y<y)
			{
				ani = ID_ANI_MOVINGRIGHTUP;
			}
			else
			{
				ani = ID_ANI_MOVINGRIGHTDOWN;
			}
		}
		else {
			if (mario->y < y)
			{
				ani = ID_ANI_MOVINGLEFTUP;
			}
			else
			{
				ani = ID_ANI_MOVINGLEFTDOWN;
			}
		}
	}
	animations->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void FireRedPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREREDPLANT_STATE_START:	{
		time = GetTickCount64();
		vy = 0;
		break;
	}
	case FIREREDPLANT_STATE_MOVINGUP: {
		time = 0;
		vy = -VY;
		break;
	}
	case FIREREDPLANT_STATE_STOP:{
		time = GetTickCount64();
		vy = 0;
		break;
	}
	case FIREREDPLANT_STATE_MOVINGDOWN: {
		time = 0;
		vy = VY;
		break;
	}
	default:
		break;
	}
}

void FireRedPlant::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void FireRedPlant::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
}