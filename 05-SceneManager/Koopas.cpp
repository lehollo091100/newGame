#include "Koopas.h"

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state==KOOPAS_STATE_WALKING)
	{
		left = x - KOOPAS_WIDTH / 2;
		top = y - KOOPAS_HEIGHT / 2;
		right = left + KOOPAS_WIDTH;
		bottom = top + KOOPAS_HEIGHT;
	}
	else {
		left = x - KOOPAS_WIDTH / 2;
		top = y - KOOPAS_WIDTH / 2;
		right = left + KOOPAS_WIDTH;
		bottom = top + KOOPAS_WIDTH;
	}
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += AY * dt;
	if (state == KOOPAS_STATE_WALKING) {
		begin = 0;
		if (item->vy> 0.016f)
		{
			nx = -nx;
			vx = nx * vx;
			item->nx = nx;
			item->vx = item->nx * item->vx;
			if (vx > 0)
			{
				this->item->SetPosition(x + 10, y);
			}
			else if (vx < 0)
			{
				item->SetPosition(x - 10, y);
			}
		}
	}
	//DebugOut(L"state:%d\n", state);
	//if(item!=NULL)
	//	DebugOut(L"itemx:%f", item->x);
	if (state==KOOPAS_STATE_DEFENDDOWN)
	{
		if (GetTickCount64() - begin > 5000) {
			y -= RANGE_STAND_UP;
			SetState(KOOPAS_STATE_WALKING);
		}
	}
	CCollision::GetInstance()->Process(this,dt, coObjects);
}



void Koopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING: {
		vx = nx * KOOPAS_VX;
		//vx = 0;
		break;
	}
	case KOOPAS_STATE_DEFENDDOWN: {
		begin = GetTickCount64();
		vx = 0;
		break;
	}
	case KOOPAS_STATE_ATTACKDOWN: {
		vx = nx * KOOPAS_ATTACK_VX;
		break;
	}
	default:
		break;
	}
}


void Koopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Koopas::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{

	if (state != KOOPAS_STATE_ATTACKDOWN)
	{
		if (dynamic_cast<Mushroom*>(e->obj)) return;
		if (dynamic_cast<CGoomba*>(e->obj)) return;
		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			nx = -nx;
			vx = nx*vx;
			item->nx = nx;
			item->vx = item->nx * item->vx;
			if (vx > 0)
			{
					this->item->SetPosition(x + 10, y);
			}
			else if (vx < 0)
			{
					item->SetPosition(x - 10, y);
			}
		}
	}
	else if(state==KOOPAS_STATE_ATTACKDOWN){
		if (dynamic_cast<CGoomba*>(e->obj))
			OnCollisionWithGoomba(e,dt);
		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0 )
		{
			if (e->obj->type == OBJECT_TYPE_GOOMBA && e->obj->state == GOOMBA_STATE_DIE)
			{
				return;
			}
			nx = -nx;
			vx = nx * vx;
			
		}
	}

}

void Koopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e,DWORD dt)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	goomba->SetState(GOOMBA_STATE_DIE);
}

void Koopas::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx > 0)
		{
			animations->Get(ID_ANI_WALKINGRIGHT)->Render(x, y);
		}
		if (vx < 0)
		{
			animations->Get(ID_ANI_WALKINGLEFT)->Render(x, y);
		}
	}
	else if (state == KOOPAS_STATE_DEFENDDOWN) {
		animations->Get(ID_ANI_DEFENDDOWN)->Render(x, y);

	}
	else if (state == KOOPAS_STATE_ATTACKDOWN) {
		animations->Get(ID_ANI_DEFENDDOWN)->Render(x, y);
	}
	RenderBoundingBox();
}