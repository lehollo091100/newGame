#include "QuestionBrick.h"
#include "debug.h"
void QuestionBrick::Render()
{

	int	aniId = ID_ANI_NORMAL;
	if (state==QBSTATE_NOTHING)
	{
		aniId = ID_ANI_NOTHING;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}
void QuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//vy = 0.03f;
	if (state == QBSTATE_NOTHING)
		return;
	y += vy*dt;
	if (state == QBSTATE_MOVING)
	{
		if (vy == 0)
		{
			vy = -VY;
			if (item->type == OBJECT_TYPE_COINITEM)
			{
				item->SetState(COIN_STATE_APPEAR);
			}
		}
		if (this->y < initY - RANGE_UP)
		{
			vy = VY;
		}
		if (this->y >= initY && vy>0)
		{
			vy = 0;
			if (item->type == OBJECT_TYPE_MUSHROOM)
			{
				item->SetState(MUSHROOM_STATE_UP);
				item->nx=this->nx;
			}
			if (item->type == OBJECT_TYPE_LEAF) {
				item->SetState(LEAF_STATE_MOVING);
			}
			y = initY;
			this->SetState(QBSTATE_NOTHING);
		}
	}
	/*x += vx*dt;*/
	CGameObject::Update(dt, coObjects);

}
void QuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QBRICK_WIDTH / 2;
	t = y - QBRICK_WIDTH / 2;
	r = l + QBRICK_WIDTH;
	b = t + QBRICK_WIDTH;
}
void QuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QBSTATE_NORMAL: {
		break;
	}
	case QBSTATE_MOVING: {
		
		break;
	}
	case QBSTATE_UP:{
		break;
	}
	case QBSTATE_NOTHING: {

		break;
	}
	}
}