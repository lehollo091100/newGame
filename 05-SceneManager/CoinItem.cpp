#include "CoinItem.h"
#include "debug.h"
void CoinItem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_NORMAL:
		break;
	case COIN_STATE_APPEAR:
		break;
	default:
		break;
	}
}

void CoinItem::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == COIN_STATE_APPEAR)
	{

	animations->Get(ID_ANI_COIN)->Render(x, y);
	}
	RenderBoundingBox();
}

void CoinItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
	if (state == COIN_STATE_APPEAR) {
		if (vy == 0) {
			vy = -VY;
		}
		if (vy < 0)
		{
			if (y < initY - RANGEY) {
				vy = VY;
			}
		}
		if (vy > 0)
		{
			if (y > initY)
			{
				this->Delete();
			}
		}
	}
	CGameObject::Update(dt,coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CoinItem::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CoinItem::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CoinItem*>(e->obj)) return;

	if (state == COIN_STATE_APPEAR && vy > 0)
	{
		if (e->ny != 0 )
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
		}
	}
}
void CoinItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}