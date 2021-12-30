#include "FireGreenPlant.h"

void FireGreenPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIREGREENPLANT_WIDTH / 2;
	top = y - FIREGREENPLANT_HEIGHT / 2;
	right = left + FIREGREENPLANT_WIDTH;
	bottom = top + FIREGREENPLANT_HEIGHT;

}

void FireGreenPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == FIREGREENPLANT_STATE_START)
	{
		if (GetTickCount64() - time >= FIREGREENPLANT_TIME)
		{
			SetState(FIREGREENPLANT_STATE_MOVINGUP);
		}
	}
	else if (state == FIREGREENPLANT_STATE_MOVINGUP) {
		if (initY - y >= FIREGREENPLANT_RANGE)
		{

			SetState(FIREGREENPLANT_STATE_STOP);
			item->startX = x;
			item->startY = y;
		}
	}
	else if (state == FIREGREENPLANT_STATE_STOP)
	{
		if (GetTickCount64() - time >= FIREGREENPLANT_TIME / 2)
		{
			if (item->state != FIREPLANT_MOVING) {
				item->SetPosition(x, y);
				if (mario->x > x)
				{
					if (mario->y >= y) {
						if (mario->x > x + FIRE_GREENPLANT_RANGE) {
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
						if (mario->x > x + FIRE_GREENPLANT_RANGE) {
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
						if (mario->x < x - FIRE_GREENPLANT_RANGE) {
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
						if (mario->x < x - FIRE_GREENPLANT_RANGE) {
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
		if (GetTickCount64() - time >= FIREGREENPLANT_TIME)
		{
			SetState(FIREGREENPLANT_STATE_MOVINGDOWN);
		}
	}
	else if (state == FIREGREENPLANT_STATE_MOVINGDOWN) {
		if (y >= initY) {
			vy = 0;
			y = initY;
			SetState(FIREGREENPLANT_STATE_START);
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void FireGreenPlant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int ani = 0;
	if (state == FIREGREENPLANT_STATE_START || state == FIREGREENPLANT_STATE_STOP)
	{
		if (mario->x > x)
		{
			if (mario->y < y)
			{
				ani = ID_ANI_FIREGREENPLANT_OPENUPRIGHT;
			}
			else
			{
				ani = ID_ANI_FIREGREENPLANT_OPENDOWNRIGHT;
			}
		}
		else {
			if (mario->y < y)
			{
				ani = ID_ANI_FIREGREENPLANT_OPENUPLEFT;
			}
			else
			{
				ani = ID_ANI_FIREGREENPLANT_OPENDOWNLEFT;
			}
		}
	}
	else if (state == FIREGREENPLANT_STATE_MOVINGUP || state == FIREGREENPLANT_STATE_MOVINGDOWN)
	{
		if (mario->x > x)
		{
			if (mario->y < y)
			{
				ani = ID_ANI_FIREGREENPLANT_MOVINGRIGHTUP;
			}
			else
			{
				ani = ID_ANI_FIREGREENPLANT_MOVINGRIGHTDOWN;
			}
		}
		else {
			if (mario->y < y)
			{
				ani = ID_ANI_FIREGREENPLANT_MOVINGLEFTUP;
			}
			else
			{
				ani = ID_ANI_FIREGREENPLANT_MOVINGLEFTDOWN;
			}
		}
	}
	animations->Get(ani)->Render(x, y);
	RenderBoundingBox();
}

void FireGreenPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREGREENPLANT_STATE_START: {
		time =DWORD( GetTickCount64());
		vy = 0;
		break;
	}
	case FIREGREENPLANT_STATE_MOVINGUP: {
		time = 0;
		vy = -FIREGREENPLANT_VY;
		break;
	}
	case FIREGREENPLANT_STATE_STOP: {
		time = DWORD(GetTickCount64());
		vy = 0;
		break;
	}
	case FIREGREENPLANT_STATE_MOVINGDOWN: {
		time = 0;
		vy = FIREGREENPLANT_VY;
		break;
	}
	default:
		break;
	}
}

void FireGreenPlant::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void FireGreenPlant::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	return;
}
