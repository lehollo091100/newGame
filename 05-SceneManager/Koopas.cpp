#include "Koopas.h"
#include "Mario.h"
#include "Tail.h"
#include "QuestionBrick.h"
#define RANGE	10
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
	//DebugOut(L"state:%d\n", nx);
	/*if (isHold)
	{
		return;
	}*/
	vy += KOOPAS_AY * dt;
	if (state == KOOPAS_STATE_WALKING) {
		if (Isonplatform)
		{
			begin = 0;
			if (item->vy > ITEM_VY)
			{
				nx = -nx;
				vx = nx * vx;
				item->nx = nx;
				item->vx = item->nx * item->vx;
				if (vx > 0)
				{
					this->item->SetPosition(x + RANGE, y);
				}
				else if (vx < 0)
				{
					item->SetPosition(x - RANGE, y);
				}
			}
		}
	}
	if (state==KOOPAS_STATE_DEFENDDOWN || state==KOOPAS_STATE_DEFENDUP)
	{
		if (GetTickCount64() - begin > TIME_STANDUP) {
			y -= RANGE_STAND_UP;
			SetState(KOOPAS_STATE_WALKING);
		}
	}
	if (x<0)
	{
		this->item->Delete();
		this->Delete();
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
		if (item)
		{
			item->nx = nx;
			item->vx = item->nx * item->vx;
			if (vx > 0)
			{
				this->item->SetPosition(x + RANGE, y);
			}
			else if (vx < 0)
			{
				item->SetPosition(x - RANGE, y);
			}
			item->SetState(KOOPAS_STATE_WALKING);
		}
		//vx = 0;
		break;
	}
	case KOOPAS_STATE_FLYING: {
		vx = nx * KOOPAS_VX;
		if (item)
		{
			item->SetState(KOOPAS_STATE_WALKING);
		}
		//vx = 0;
		break;
	}
	case KOOPAS_STATE_DEFENDDOWN: {
		begin = GetTickCount64();
		item->SetState(ITEM_STATE_STOP);
		if (nx >= 0)
		{
			item->SetPosition(x + RANGE, y);
		}
		else 
		{
			item->SetPosition(x - RANGE, y);
		}
		vx = 0;
		break;
	}
	case KOOPAS_STATE_ATTACKDOWN: {
		vx = nx * KOOPAS_ATTACK_VX;
		item->SetState(ITEM_STATE_STOP);
		break;
	}
	case KOOPAS_STATE_DEFENDUP: {
		begin = GetTickCount64();
		item->SetState(ITEM_STATE_STOP);
		break;
	}
	case KOOPAS_STATE_ATTACKUP: {
		vx = nx * KOOPAS_ATTACK_VX;
		item->SetState(ITEM_STATE_STOP);
		break;
	}
	case KOOPAS_STATE_HOLDING: {
		vx = 0;
		item->SetState(ITEM_STATE_STOP);
		break;
	}
	case KOOPAS_STATE_DIE: {
		//item->Delete();
		vx = nx * KOOPAS_VX;
		vy = -KOOPAS_DEFENDUP_BOUNDING_SPEED;
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

	if (dynamic_cast<Tail*>(e->obj)) return;
	if (state != KOOPAS_STATE_ATTACKDOWN && state != KOOPAS_STATE_ATTACKUP)
	{
		if (dynamic_cast<Mushroom*>(e->obj)) return;
		if (dynamic_cast<CGoomba*>(e->obj)) {
			if (state == KOOPAS_STATE_DEFENDDOWN || state == KOOPAS_STATE_DEFENDUP)
			{
				OnCollisionWithGoomba(e, dt);
			}
			else
			{
				return;
			}
		};
		if (dynamic_cast<Redgoomba*>(e->obj)) {
			if (state == KOOPAS_STATE_DEFENDDOWN || state == KOOPAS_STATE_DEFENDUP)
			{
				OnCollisionWithRedGoomba(e, dt);
			}
			else
			{
				return;
			}
		}
		if (e->ny != 0)
		{
			Isonplatform = true;
			if (state==KOOPAS_STATE_FLYING)
			{
				vy = -KOOPAS_FLYING_SPEED;

			}
			else if (state == KOOPAS_STATE_DEFENDUP || state == KOOPAS_STATE_DEFENDDOWN)
			{
				vx = 0;
				vy = 0;

			}
			/*else if (state == KOOPAS_STATE_DIE) {
				item->Delete();
				Delete();
			}*/
			else
			{
				vy = 0;
				
			}
		}
		else if (e->nx != 0)
		{
			nx = -nx;
			vx = nx*vx;
			item->nx = nx;
			item->vx = item->nx * item->vx;
			if (vx > 0)
			{
					this->item->SetPosition(x + RANGE, y);
			}
			else if (vx < 0)
			{
					item->SetPosition(x - RANGE, y);
			}
		}
	}
	else if(state==KOOPAS_STATE_ATTACKDOWN || state==KOOPAS_STATE_ATTACKUP){
		if (dynamic_cast<CGoomba*>(e->obj))
			OnCollisionWithGoomba(e,dt);
		if (dynamic_cast<Redgoomba*>(e->obj))
			OnCollisionWithRedGoomba(e, dt);
		if (dynamic_cast<QuestionBrick*>(e->obj))
		{
			QuestionBrick* QBrick = dynamic_cast<QuestionBrick*>(e->obj);
			if (QBrick->GetState() == QBSTATE_NORMAL)
			{

				QBrick->nx = -this->nx;
				QBrick->SetState(QBSTATE_MOVING);
				if (QBrick->item->type == OBJECT_TYPE_COINITEM) {
					CMario::GetInstance()->coin++;
				}
			}
		}
		if (dynamic_cast<ShinningBrick*>(e->obj)) {
			nx = -nx;
			vx = nx * vx;
			ShinningBrick* sbrick = dynamic_cast<ShinningBrick*>(e->obj);
			sbrick->d1->SetState(DEBRIS_STATE_MOVING);
			sbrick->d2->SetState(DEBRIS_STATE_MOVING);
			sbrick->d3->SetState(DEBRIS_STATE_MOVING);
			sbrick->d4->SetState(DEBRIS_STATE_MOVING);
			/*sbrick->d1->SetState(DEBRIS_STATE_MOVING);
			sbrick->d1->SetState(DEBRIS_STATE_MOVING);*/
			sbrick->Delete();

		}
		if (e->ny != 0)
		{
			Isonplatform = true;
			vy = 0;
		}
		else if (e->nx != 0 )
		{
			if (e->obj->type == OBJECT_TYPE_GOOMBA && e->obj->state == GOOMBA_STATE_DIE)
			{
				return;
			}
			if (e->obj->type == OBJECT_TYPE_KOOPASITEM)
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
	if (isHold)
	{
		goomba->SetState(GOOMBA_STATE_DIEUP);
		SetState(KOOPAS_STATE_DIE);
	}
	else {
		if (state == KOOPAS_STATE_ATTACKDOWN || state == KOOPAS_STATE_ATTACKUP) {
			goomba->SetState(GOOMBA_STATE_DIEUP);
		}
	}
}

void Koopas::OnCollisionWithRedGoomba(LPCOLLISIONEVENT e, DWORD dt)
{
	Redgoomba* redgoomba = dynamic_cast<Redgoomba*>(e->obj);
	if (isHold)
	{
		redgoomba->SetState(REDGOOMBA_STATE_DIEUP);
		SetState(KOOPAS_STATE_DIE);
	}
	else {
		if (state == KOOPAS_STATE_ATTACKDOWN || state == KOOPAS_STATE_ATTACKUP) {
			redgoomba->SetState(REDGOOMBA_STATE_DIEUP);
		}
	}
}

void Koopas::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (Color==0)
	{
		if (state == KOOPAS_STATE_WALKING)
		{
			if (vx > 0)
			{
				animations->Get(ID_ANI_WALKINGRIGHT_RED)->Render(x, y);
			}
			if (vx < 0)
			{
				animations->Get(ID_ANI_WALKINGLEFT_RED)->Render(x, y);
			}
		}
		else if (state == KOOPAS_STATE_DEFENDDOWN) {
			animations->Get(ID_ANI_DEFENDDOWN_RED)->Render(x, y);

		}
		else if (state == KOOPAS_STATE_ATTACKDOWN) {
			animations->Get(ID_ANI_DOWNATTACK_RED)->Render(x, y);
		}
		else if (state == KOOPAS_STATE_DEFENDUP) {
			animations->Get(ID_ANI_DEFENDUP_RED)->Render(x, y);

		}
		else if (state == KOOPAS_STATE_ATTACKUP || state == KOOPAS_STATE_DIE) {
			animations->Get(ID_ANI_ATTACKUP_RED)->Render(x, y);
		}
	}
	else
	{
		if (state == KOOPAS_STATE_WALKING)
		{
			if (vx > 0)
			{
				animations->Get(ID_ANI_WALKINGRIGHT_GREEN)->Render(x, y);
			}
			if (vx < 0)
			{
				animations->Get(ID_ANI_WALKINGLEFT_GREEN)->Render(x, y);
			}
		}
		else if (state == KOOPAS_STATE_FLYING)
		{
			if (vx > 0)
			{
				animations->Get(ID_ANI_FLYINGRIGHT)->Render(x, y);
			}
			if (vx < 0)
			{
				animations->Get(ID_ANI_FLYINGLEFT)->Render(x, y);
			}
		}
		else if (state == KOOPAS_STATE_DEFENDDOWN) {
			animations->Get(ID_ANI_DEFENDDOWN_GREEN)->Render(x, y);

		}
		else if (state == KOOPAS_STATE_ATTACKDOWN) {
			animations->Get(ID_ANI_DOWNATTACK_GREEN)->Render(x, y);
		}
		else if (state == KOOPAS_STATE_DEFENDUP) {
			animations->Get(ID_ANI_DEFENDUP_GREEN)->Render(x, y);
		}
		else if (state == KOOPAS_STATE_ATTACKUP||state==KOOPAS_STATE_DIE) {
			animations->Get(ID_ANI_ATTACKUP_GREEN)->Render(x, y);
		}
	}
	//RenderBoundingBox();
}