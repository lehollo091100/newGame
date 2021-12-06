#include "ShinningBrick.h"

void ShinningBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SBRICK_STATE_NORMAL: {
		break;
	}
	default:
		break;
	}
}

void ShinningBrick::Render()
{
	int	aniId = 0;
	if (state == SBRICK_STATE_NORMAL)
	{
		aniId = ID_ANI_NORMAL;
	}
	else
	{
		aniId = ID_ANI_NORMAL;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void ShinningBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void ShinningBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SBRICK_WIDTH / 2;
	t = y - SBRICK_WIDTH / 2;
	r = l + SBRICK_WIDTH;
	b = t + SBRICK_WIDTH;
}
