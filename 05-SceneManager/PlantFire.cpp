#include "PlantFire.h"

void PlantFire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - WIDTH / 2;
	top = y - WIDTH / 2;
	right = left + WIDTH;
	bottom = top + WIDTH;
}

void PlantFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void PlantFire::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_NORMAL)->Render(x, y);
	RenderBoundingBox();
}

void PlantFire::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREPLANT_START: {
		vx = 0;
		vy = 0;
		break;
	}
	case FIREPLANT_MOVING: {

		break;
	}
	default:
		break;
	}
}

void PlantFire::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void PlantFire::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
}
