#include "FireRedPlant.h"
#include "debug.h"
void FireRedPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIREREDPLANT_WIDTH / 2;
	top = y - FIREREDPLANT_HEIGHT / 2;
	right = left + FIREREDPLANT_WIDTH;
	bottom = top + FIREREDPLANT_HEIGHT;
}

void FireRedPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == FIREREDPLANT_STATE_START)
	{
		if (GetTickCount64() - time >= FIREREDPLANT_TIME)
		{
			SetState(FIREREDPLANT_STATE_MOVINGUP);
		}
	}
	else if (state == FIREREDPLANT_STATE_MOVINGUP) {
		if (initY - y >= FIREREDPLANT_RANGE)
		{
			
			SetState(FIREREDPLANT_STATE_STOP);
			item->startX = x;
			item->startY = y;
		}
	}
	else if (state == FIREREDPLANT_STATE_STOP)
	{
		if (GetTickCount64() - time >= FIREREDPLANT_TIME /2)
		{
			if (item->state != FIREPLANT_MOVING) {
				item->SetPosition(x, y);
				if (mario->x>x)
				{
					if (mario->y >= y) {
						if (mario->x > x + FIRERANGE) {
							item->SetPosition(x, y);
							item->SetState(FIREPLANT_MOVING);
							item->vx = FIRE_VX;
							item->vy = FIRE_VYFAR;
						}
						else
						{
							item->SetPosition(x, y);
							item->SetState(FIREPLANT_MOVING);
							item->vx = FIRE_VX;
							item->vy = FIRE_VYNEAR;
						}
					}
					else
					{
						if (mario->x > x + FIRERANGE) {
							item->SetPosition(x, y);
							item->SetState(FIREPLANT_MOVING);
							item->vx = FIRE_VX;
							item->vy = -FIRE_VYFAR;
						}
						else
						{
							item->SetPosition(x, y);
							item->SetState(FIREPLANT_MOVING);
							item->vx = FIRE_VX;
							item->vy = -FIRE_VYNEAR;
						}
					}
				}
				else
				{
					if (mario->y >= y) {
						if (mario->x < x - FIRERANGE) {
							item->SetState(FIREPLANT_MOVING);
							item->vx = -FIRE_VX;
							item->vy = FIRE_VYFAR;
						}
						else
						{
							item->SetPosition(x, y);
							item->SetState(FIREPLANT_MOVING);
							item->vx = -FIRE_VX;
							item->vy = FIRE_VYNEAR;
						}
					}
					else
					{
						if (mario->x < x - FIRERANGE) {
							item->SetPosition(x, y);
							item->SetState(FIREPLANT_MOVING);
							item->vx = -FIRE_VX;
							item->vy = -FIRE_VYFAR;
						}
						else
						{
							item->SetPosition(x, y);
							item->SetState(FIREPLANT_MOVING);
							item->vx = -FIRE_VX;
							item->vy = -FIRE_VYNEAR;
						}
					}
				}

			}

		}
		if (GetTickCount64() - time >= FIREREDPLANT_TIME)
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
		time = DWORD(GetTickCount64());
		vy = 0;
		break;
	}
	case FIREREDPLANT_STATE_MOVINGUP: {
		time = 0;
		vy = -FIREREDPLANT_VY;
		break;
	}
	case FIREREDPLANT_STATE_STOP:{
		time = DWORD(GetTickCount64());
		vy = 0;
		break;
	}
	case FIREREDPLANT_STATE_MOVINGDOWN: {
		time = 0;
		vy = FIREREDPLANT_VY;
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
