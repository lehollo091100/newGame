#include "PButton.h"

void PButton::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PBUTTON_STATE_NORMAL: {
		break;
	}
	case PBUTTON_STATE_MOVING: {
		vy = -PBRICK_VY;
		break;
	}
	case PBUTTON_STATE_NOTHING: {
		y += PLUS_RANGE;
		break;

	}
	default:
		break;
	}
}

void PButton::Render()
{
	int	aniId = 0;
	if (state == PBUTTON_STATE_NOTHING)
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

void PButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	if (state == PBUTTON_STATE_MOVING)
	{
		if (y <= initY - WIDTH)
		{
			vy = 0;
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void PButton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == PBUTTON_STATE_NOTHING) {
		l = x - WIDTH / 2;
		t = y - 5;
		r = l + WIDTH;
		b = t + 5;
	}
	else if(state==PBUTTON_STATE_MOVING){
		l = x - WIDTH / 2;
		t = y - WIDTH / 2;
		r = l + WIDTH;
		b = t + WIDTH;
	}
	else
	{
		l = x - WIDTH / 2;
		t = y - SWIDTH / 2;
		r = l + WIDTH;
		b = t + SWIDTH;
	}
}
