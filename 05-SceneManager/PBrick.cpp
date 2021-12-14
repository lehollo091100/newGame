#include "PBrick.h"

void PBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PBRICK_STATE_NORMAL: {
		vx = 0; vy = 0;
		break;
	}
	case PBRICK_STATE_NOTHING: {
		break;
	}
	case PBRICK_STATE_MOVING: {
		vy = -PBRICK_VY;
		button->SetState(PBUTTON_STATE_MOVING);
		break;
	}
	default:
		break;
	}
}

void PBrick::Render()
{
	int	aniId = 0;
	if (state == PBRICK_STATE_NOTHING)
	{
		aniId = ID_ANI_NOTHING;
	}
	else
	{
		aniId = ID_ANI_NORMAL;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void PBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;

	if (state == PBRICK_STATE_NOTHING)
	{
		return;
	}
	if (state == PBRICK_STATE_MOVING) {
		if (initY - y >= PBRICK_WIDTH / 2)
		{
			vy = PBRICK_VY;
		}
		if (vy > 0)
		{
			if (y > initY) {
				vy = 0;
				y = initY;
				SetState(PBRICK_STATE_NOTHING);
			}
		}
	}
	CGameObject::Update(dt, coObjects);
	//CCollision::GetInstance()->Process(this, dt, coObjects);
}

void PBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PBRICK_WIDTH / 2;
	t = y - PBRICK_WIDTH / 2;
	r = l + PBRICK_WIDTH;
	b = t + PBRICK_WIDTH;
}
