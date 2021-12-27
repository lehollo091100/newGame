#include "Tail.h"
#include "Goomba.h"
#include "Koopas.h"
#include "QuestionBrick.h"
#include "Mario.h"
#include "Debris.h"
#include "ShinningBrick.h"
#include "PBrick.h"
#include "FireGreenPlant.h"
#include "FireRedPlant.h"
#include "GreenPlant.h"
#include "Redgoomba.h"
void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TAIL_WIDTH / 2;
	top = y - TAIL_HEIGHT / 2;
	right = left + TAIL_WIDTH;
	bottom = top + TAIL_HEIGHT;
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (vx > 0)
	{
		if (x - initX >= 26)
		{
			nx = -1;
			vx = -TAIL_SPEED;
		}
	}
	else if(vx<0)
	{
		if ( initX-x >= 26)
		{
			nx = 1;
			vx = TAIL_SPEED;
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Tail::Render()
{
	//RenderBoundingBox();
}

void Tail::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TAIL_STATE_NOTHING: {
		break;
	}
	case TAIL_STATE_ATTACK: {
		initX = x;
		if (nx>=0)
		{
			vx = TAIL_SPEED;
		}
		else
		{
			vx = -TAIL_SPEED;
		}
		break;
	}
	default:
		break;
	}
}

void Tail::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Tail::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	if (state == TAIL_STATE_ATTACK)
	{
		if (dynamic_cast<Mushroom*>(e->obj)) return;
		if (dynamic_cast<Redgoomba*>(e->obj)) {
			Redgoomba* red = dynamic_cast<Redgoomba*>(e->obj);
			red->SetState(REDGOOMBA_STATE_DIEUP);
		}
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
		if (dynamic_cast<PBrick*>(e->obj)) {
			PBrick* pbrick = dynamic_cast<PBrick*>(e->obj);
			if (pbrick->state != PBRICK_STATE_NOTHING)
			{
				pbrick->SetState(PBRICK_STATE_MOVING);
			}
		}
		if (dynamic_cast<ShinningBrick*>(e->obj)) {
			ShinningBrick* sbrick = dynamic_cast<ShinningBrick*>(e->obj);
			sbrick->d1->SetState(DEBRIS_STATE_MOVING);
			sbrick->d2->SetState(DEBRIS_STATE_MOVING);
			sbrick->d3->SetState(DEBRIS_STATE_MOVING);
			sbrick->d4->SetState(DEBRIS_STATE_MOVING);
			/*sbrick->d1->SetState(DEBRIS_STATE_MOVING);
			sbrick->d1->SetState(DEBRIS_STATE_MOVING);*/
			sbrick->Delete();

		}
		if (dynamic_cast<CGoomba*>(e->obj)) {
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			goomba->SetState(GOOMBA_STATE_DIEUP);
		};
		if (dynamic_cast<Koopas*>(e->obj)) {
			Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
			if (nx >= 0)
			{
				koopas->nx = 1;
			}
			else
			{
				koopas->nx = -1;
			}
			if (koopas->state==KOOPAS_STATE_DEFENDDOWN)
			{
				koopas->vx = koopas->nx * KOOPAS_VX;
				koopas->vy= -KOOPAS_DEFENDUP_BOUNDING_SPEED;
				koopas->SetState(KOOPAS_STATE_DEFENDUP);
			}
			else
			{
				koopas->SetState(KOOPAS_STATE_DEFENDUP);
				koopas->vx = koopas->nx * KOOPAS_VX;
				koopas->vy = -KOOPAS_DEFENDUP_BOUNDING_SPEED;
			}
			//DebugOut(L"koopas right:%f THIS.Y:%f\n", koopas->y+KOOPAS_WIDTH/2,this->y);
		};
		if (dynamic_cast<FireGreenPlant*>(e->obj) || dynamic_cast<FireRedPlant*>(e->obj) || dynamic_cast<GreenPlant*>(e->obj))
		{
			if (dynamic_cast<FireGreenPlant*>(e->obj))
			{
				FireGreenPlant* firegreenplant = dynamic_cast<FireGreenPlant*>(e->obj);
				firegreenplant->item->Delete();
				firegreenplant->Delete();
			}
			if (dynamic_cast<FireRedPlant*>(e->obj))
			{
				FireRedPlant* fireredplant = dynamic_cast<FireRedPlant*>(e->obj);
				fireredplant->item->Delete();
				fireredplant->Delete();
			}
			if (dynamic_cast<GreenPlant*>(e->obj))
			{
				GreenPlant* greenplant = dynamic_cast<GreenPlant*>(e->obj);
				greenplant->Delete();
			}
		}
	}
}

