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
		vy = -PBUTTON_VY;
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
		aniId = ID_ANI_PBUTTON_NOTHING;
	}
	else
	{
		aniId = ID_ANI_PBUTTON_NORMAL;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void PButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	if (state == PBUTTON_STATE_MOVING)
	{
		if (y <= initY - PBUTTON_WIDTH)
		{
			vy = 0;
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void PButton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == PBUTTON_STATE_NOTHING) {
		l = x - PBUTTON_WIDTH / 2;
		t = y - PBUTTON_NOTHING_HEIGHT;
		r = l + PBUTTON_WIDTH;
		b = t + PBUTTON_NOTHING_HEIGHT;
	}
	else if(state==PBUTTON_STATE_MOVING){
		l = x - PBUTTON_WIDTH / 2;
		t = y - PBUTTON_WIDTH / 2;
		r = l + PBUTTON_WIDTH;
		b = t + PBUTTON_WIDTH;
	}
	else
	{
		l = x - PBUTTON_WIDTH / 2;
		t = y - PBUTTON_WIDTH / 2;
		r = l + PBUTTON_WIDTH;
		b = t + PBUTTON_WIDTH;
	}
}
