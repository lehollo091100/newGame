#include "Curtain.h"

void Curtain::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Curtain::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
}

void Curtain::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CURTAIN_STATE_NORMAL: {
		vx = 0; vy = 0;
		break;
	}
	case CURTAIN_STATE_MOVING: {
		vy = -CURTAIN_VY;
		break;
	}
	default:
		break;
	}
}

void Curtain::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_CURTAIN)->Render(x, y);
}

void Curtain::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y <= -CURTAIN_HEIGHT/2)
		Delete();
	//DebugOut(L"curtain y:%f\n", y);
	y += vy * dt;
}

void Curtain::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}
