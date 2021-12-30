#include "ShinningBrick.h"

void ShinningBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SBRICK_STATE_NORMAL: {
		break;
	}
	case SBRICK_STATE_COIN: {
		isCoin = true;
		time =DWORD(GetTickCount64());
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
		aniId = ID_ANI_SBRICK_NORMAL;
	}
	else
	{
		aniId = ID_ANI_COIN;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void ShinningBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isCoin) {
		if (GetTickCount64() - time >= TIME_TO_RETURN)
		{
			isCoin = false;
			SetState(SBRICK_STATE_NORMAL);
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void ShinningBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SBRICK_WIDTH / 2;
	t = y - SBRICK_WIDTH / 2;
	r = l + SBRICK_WIDTH;
	b = t + SBRICK_WIDTH;
}
