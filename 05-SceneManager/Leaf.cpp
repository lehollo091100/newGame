#include "Leaf.h"

void Leaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Leaf::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	return;
}

void Leaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_INVISIBLE: {
		vx = 0; vy = 0;
		break;

	}
	case LEAF_STATE_MOVING: {
		vx = 0;
		vy = -MOVINGUP_VY;
		break;
	}
	default:
		break;
	}
}

void Leaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state != LEAF_STATE_INVISIBLE)
	{
		if (vx>=0)
		{
			animations->Get(ID_ANI_LEAF_MOVERIGHT)->Render(x, y);
		}
		else
		{
			animations->Get(ID_ANI_LEAF_MOVELEFT)->Render(x, y);
		}
	}
}

void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != LEAF_STATE_INVISIBLE) {
		//vy += 0.0001f * dt;
	}
	if (state == LEAF_STATE_MOVING) {
		if (vy < 0)
		{
			if (y <= initY-RANGE) {
				vy = 0;
				nx = 1;
				vx = nx*VX;
				vy = AY;
			}
		}
		else
		{
			if (vx >= 0) {
				if (x > initX + RANGEX) {
					nx = -1;
					vx = nx * VX;
				}
			}
			else
			{
				if (x < initX - RANGEX) {
					nx = 1;
					vx = nx * VX;
				}
			}
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Leaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_WIDTH / 2;
	t = y - LEAF_HEIGHT / 2;
	r = l + LEAF_WIDTH;
	b = t + LEAF_HEIGHT;
}
